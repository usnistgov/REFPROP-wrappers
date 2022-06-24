bool has_fractions_in_string(const std::string &fluid_string)
{
    // If can find both "[" and "]", it must have mole fractions encoded as string
    return (fluid_string.find("[") != std::string::npos && fluid_string.find("]") != std::string::npos);
}

bool has_components_in_string(const std::string &fluid_string)
{
    // If can find both "[" and "]", it must have mole fractions encoded as string
    return (fluid_string.find("&") != std::string::npos);
}


std::string extract_fractions(const std::string &fluid_string, std::vector<double> &fractions)
{
    if (has_fractions_in_string(fluid_string))
    {
        if (!has_components_in_string(fluid_string)) return "bad_mix";
        fractions.clear();
        std::vector<std::string> names;

        // Break up into pairs - like "Ethane[0.5]&Methane[0.5]" -> ("Ethane[0.5]","Methane[0.5]")
        std::vector<std::string> pairs = strsplit(fluid_string, '&');

        if (pairs.size() > 20) return "n>20";

        for (std::size_t i = 0; i < pairs.size(); ++i)
        {
            const std::string &fluid = pairs[i];

            // Must end with ']'
            // if (fluid[fluid.size() - 1] != ']')
            //     throw ValueError(format("Fluid entry [%s] must end with ']' character", pairs[i].c_str()));

            // Split at '[', but first remove the ']' from the end by taking a substring
            std::vector<std::string> name_fraction = strsplit(fluid.substr(0, fluid.size() - 1), '[');

            // if (name_fraction.size() != 2) { throw ValueError(format("Could not break [%s] into name/fraction", fluid.substr(0, fluid.size() - 1).c_str())); }

            // Convert fraction to a double
            char *pEnd;
            std::string name = name_fraction[0], fraction = name_fraction[1];
            double f = strtod(fraction.c_str(), &pEnd);
            // If pEnd points to the last character in the string, it wasn't able to do the conversion
            if (pEnd == &(fraction[fraction.size() - 1])) return "bad_num";

            // Append fluid with ".fld" if no extension
            if ((upper(name).find(".FLD") == name.npos) &&
                (upper(name).find(".PPF") == name.npos))
            {
                name.append(".fld");
            }


            if (f > 10 * DBL_EPSILON)  // Only push component if fraction is positive and non-zero
            {
                // And add to vector
                fractions.push_back(f);

                // Add name
                names.push_back(name);
            }
        }

        // Join fluids back together
        return strjoin(names, "|");
    }
    else
    {
        return fluid_string;
    }
}


int cSetup(std::string strFluid)
{
    int ierr = 0;
    double comp_sum = 0;
    char herr[errormessagelength], hhmx[] = "HMX.BNC", href[] = "DEF", hfld[componentstringlength];
    char htype[] = "EOS";

    x[0] = 1.0;
    for (int j = 1; j < 20; j++) { x[j] = 0.0; }  // initialize mole fractions to zero

    // Custom mixtures can be supplied as in the following format:
    // "fluid1[x1]&fluid2[x2]&......fluidN[xN]", parse it out, and then call
    // SETUPdll with the string "fluid1|fluid2|...|fluidN" and initialize
    // x[] with the extracted mole fractions.
    std::vector<double> fractions(1, 1.0);
    std::vector<std::string> name_vec;
    std::string fluid_string = extract_fractions(strFluid, fractions);
    if (fluid_string == "n>20") return TOO_MANY_COMPONENTS;
    if (fluid_string == "bad_mix") return BAD_MIX_STRING;
    if (fluid_string == "bad_num") return BAD_MOLE_FRACTION;

    ncomp = static_cast<int>(fractions.size());
    for (int i = 0; i < ncomp; i++) { 
        comp_sum += fractions[i];
        // initialize mole fraction array
        x[i] = fractions[i];
    }
    if (std::fabs(comp_sum - 1.0) > 10 * DBL_EPSILON) return X_SUM_NONUNITY;

    // Use this pop-up window for debugging if needed before processing the fluid string
    //==================================================================================
    //std::string msg;
    //msg = "NIST RefProp DLL Version: " + RPVersion_loaded;
    //msg.append("\n\nFluid String = [");
    //msg.append(fluid_string);
    //msg.append("]\n\nChar* String = [");
    //msg.append(hfld); msg.append("]");
    //MessageBox(hwndDlg, msg.c_str(), "NIST RefProp Add-In", 0);


    if (fluid_string.find("|") != fluid_string.npos)
    {
        strcpy(MixName, "N/A");
        MixFileLast = "";
        // convert std::string to c-style string for DLL call
        std::copy(fluid_string.begin(), fluid_string.end(), hfld);    // Copy string fluidPath into c_str hfld. 
        // hfld[fluid_string.size()] = '\0';                          // Append with a null character (not sure if needed)
        // Pad the fluid string all the way to 10k characters with spaces to deal with string parsing bug in REFPROP in SETUPdll
        for (int j = static_cast<int>(fluid_string.size()); j < componentstringlength; ++j) {
            hfld[j] = ' ';
        }

        // Use this pop-up window for debugging if needed
        //===============================================================================
        //std::string msg;
        //msg = "NIST RefProp DLL Version: " + RPVersion_loaded;
        //msg.append("\n\nFluid String = [");
        //msg.append(fluid_string);
        //msg.append("]\n\nChar* String = [");
        //msg.append(hfld); msg.append("]");
        //MessageBox(hwndDlg, msg.c_str(), "NIST RefProp Add-In", 0);

        SETUPdll(&ncomp, hfld, hhmx, href, &ierr, herr,
            componentstringlength, refpropcharlength, lengthofreference, errormessagelength);

        if (ierr != 0)
            return FLUID_NOT_FOUND;
    }
    else if (ncomp == 1 && upper(strFluid).find(".MIX") != strFluid.npos)
    {   // Read Mixture File
        // convert std::string to c-style string for DLL call
        char mix[255] = { 0 };                            // Initialize char array mypath
        std::copy(strFluid.begin(), strFluid.end(), mix); // Copy string fluidPath into c_str mix. 
        mix[strFluid.size()] = '\0';                      // Append with a null character (not sure if needed)
    
		SETMIXdll(mix,             // Mixture name
            hhmx,                  // Mixture file
            href,                  // Reference type
            &ncomp,                // Number of components
            hfld,                  // long string of mixture file names (returned)
            &(x[0]),               // mixture mole fractions
            &ierr,                 // returned error code
            herr,                  // error message
		    255,                   // Mixture name length
            refpropcharlength,     // length of char string for mixture file 
            lengthofreference,     // reference type length
            componentstringlength, // returned component string length
            errormessagelength);   // error message length

        if (ierr == 101)
            return FLUID_NOT_FOUND;
        else if (ierr == -103)
            return X_SUM_NONUNITY;
        else 
            ierr = 0;

        MixFileName = mix;
        if (MixFileName != MixFileLast)
        {
            std::string MixFileFull;
            MixFileFull = fluidPath + "\\" + MixFileName;
            std::ifstream mFile(MixFileFull);
            if (mFile.is_open())
            {
                mFile.getline(MixName, namelengthlong);
                mFile.close();
                MixFileLast = MixFileName;
            }
            else
            {
                MixFileFull = fluidPath + "\\mixtures\\" + MixFileName;
                std::ifstream mFile(MixFileFull);
                if (mFile.is_open())
                {
                    mFile.getline(MixName, namelengthlong);
                    mFile.close();
                    MixFileLast = MixFileName;
                }
                else
                {
                    // Use this pop-up window for debugging if needed
                    //===============================================================================
                    //std::string msg;
                    //msg = "Could not open file: " + fluidPath + "\\" + MixFileName;
                    //msg.append("\n            or file: " + fluidPath + "\\mixtures\\" + MixFileName);
                    //MessageBox(hwndDlg, msg.c_str(), "NIST RefProp Add-In", 0);
                }
           }
        }
	}
    else // single fluid or pseudo-pure fluid
    {
        strcpy(MixName, "N/A");
        MixFileLast = "";

        if ((upper(strFluid).find(".FLD") == strFluid.npos) &&       // If fluid string doesn't end
            (upper(strFluid).find(".PPF") == strFluid.npos))         // end with ".fld" or ".ppf" ...
        {
            strFluid.append(".fld");                                 //   ... assume it's a ".fld" file.
        }
        // convert std::string to c-style string for DLL call
        char hfld[10000];                                            // Initialize char array hfld
        std::copy(strFluid.begin(), strFluid.end(), hfld);           // Copy string fluidPath into c_str hfld. 
        hfld[strFluid.size()] = '\0';                                // Append with a null character (not sure if needed)

        SETUPdll(&ncomp,hfld,hhmx,href,&ierr,herr,
            componentstringlength,refpropcharlength,lengthofreference,errormessagelength);

        if (ierr != 0)
        {
            // Use this pop-up window for debugging if needed
            //===============================================================================
            //std::string msg;
            //msg = "Could not open file: " + fluidPath + "\\" + strFluid;
            //msg.append("\nhfld = [");
            //msg.append(hfld); msg.append("]");
            //msg.append(format("\n  ierr: %d",ierr));
            //MessageBox(hwndDlg, msg.c_str(), "NIST RefProp Add-In", 0);
            //===============================================================================
            return FLUID_NOT_FOUND;
        }
    }

    LIMITSdll(htype, &x[0], &Tmin, &Tmax, &Dmax, &Pmax, lengthofreference);   // Get Tmin, Tmax, Dmzx, Pmax for this fluid/mixture
    WMOLdll(&x[0], &wmm);                                                     // Get molar mass [kg/kmol or g/mol]

    return ierr;           // return the error flag from SETUP0

}           
    