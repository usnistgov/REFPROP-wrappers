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

    if (strFluid != LastFluid)
    {

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

        if (fluid_string.find("|") != fluid_string.npos)                  // *** Ad-Hoc mixture String ***
        {
            // *** strcpy(MixName, "N/A");
            // convert std::string to c-style string for DLL call
            std::copy(fluid_string.begin(), fluid_string.end(), hfld);    // Copy string fluid_string into c_str hfld. 
            // Pad the fluid string all the way to 10k characters with spaces to deal with string parsing bug in older REFPROP in SETUPdll
            for (int j = static_cast<int>(fluid_string.size()); j < componentstringlength; ++j) {
                hfld[j] = ' ';
            }

            // Use this pop-up window for debugging if needed
            //===============================================================================
            //std::string msg1;
            // msg1 = "NIST RefProp DLL Version: " + RPVersion_loaded;
            // msg1.append("\n\nFluid String = [");
            // msg1.append(fluid_string);
            // msg1.append("]\n\nChar* String = [");
            // msg1.append(hfld); msg.append("]");
            // MessageBox(hwndDlg, msg1.c_str(), "NIST RefProp Add-In", 0);

            SETUPdll(&ncomp, hfld, hhmx, href, &ierr, herr,
                componentstringlength, refpropcharlength, lengthofreference, errormessagelength);

            if (ierr > 0)
                return FLUID_NOT_FOUND;
        }
        else if (upper(strFluid).find(".MIX") != strFluid.npos)    // Does the file have a .MIX extension?
        {   // Read Mixture File
            fluid_string = strFluid;
            if (upper(strFluid).find("C\\") == strFluid.npos)      // If fluid string is NOT a full path string
            {
                // Check in Mixtures directory as well as the user's Virtual Store directory for the file
                std::string MixFileFull = fluidPath + 
                                          "\\mixtures\\" + 
                                          strFluid;                //   Full path if in REFPROP\MIXTURES
                std::ifstream mFile(MixFileFull);                  //   Open file if it exists
                if (mFile.fail())                                  //   If not,
                {
                    char* localAppData = getenv("LOCALAPPDATA");   //     get user's AppData path
                    std::string vStore = localAppData;             //     start with AppData path
                    vStore.append("\\VirtualStore");               //     append virtual store folder
                                                                   //     tack on full path sans "C:"
                    vStore.append(MixFileFull.begin() + 2, MixFileFull.end());
                    mFile.open(vStore);                            //     Open vStore file if it exists
                    if (mFile.fail())
                        return FLUID_NOT_FOUND;                    //     Not found in either location
                    else
                    {
                        fluid_string = vStore;
                        mFile.close();                               //     Found in Virtual Store, close it.
                    }
                }
                else mFile.close();                                //   Found in MIXTURES directory, close it.
            }                                                      // Otherwise, full path was given. Use it.

            // convert std::string to c-style string for DLL call
            char mix[filepathlength] = { 0 };                 // Initialize char array mix
            std::copy(fluid_string.begin(), fluid_string.end(), mix); // Copy string strFluid into c_str mix. 
            mix[fluid_string.size()] = '\0';                      // Append with a null character (not sure if needed)

            SETMIXdll(mix,             // Mixture name
                hhmx,                  // Mixture file
                href,                  // Reference type
                &ncomp,                // Number of components
                hfld,                  // long string of mixture file names (returned)
                &x[0],                 // mixture mole fractions
                &ierr,                 // returned error code
                herr,                  // error message
                255,                   // Mixture name length
                refpropcharlength,     // length of char string for mixture file 
                lengthofreference,     // reference type length
                componentstringlength, // returned component string length
                errormessagelength);   // error message length

            if (ierr == 101)
            {
                return FLUID_NOT_FOUND;
                // Use this pop-up window for debugging if needed
                //===============================================================================
                // std::string msg2;
                // msg2 = "Could not open file: " + strFluid;
                // MessageBox(hwndDlg, msg2.c_str(), "NIST RefProp Add-In", 0);
            }
            else if (ierr == -103)
                return X_SUM_NONUNITY;
            else 
                ierr = 0;
        }
        else // single fluid or pseudo-pure fluid
        {
            if ((upper(strFluid).find(".FLD") == strFluid.npos) &&       // If fluid string doesn't end
                (upper(strFluid).find(".PPF") == strFluid.npos))         // end with ".fld" or ".ppf" ...
            {
                strFluid.append(".fld");                                 //   ... assume it's a ".fld" file.
            }
            // convert std::string to c-style string for DLL call
            std::copy(strFluid.begin(), strFluid.end(), hfld);           // Copy string strFluid into c_str hfld. 
            hfld[strFluid.size()] = '\0';                                // Append with a null character (not sure if needed)

            SETUPdll(&ncomp, hfld, hhmx, href, &ierr, herr,
                componentstringlength, refpropcharlength, lengthofreference, errormessagelength);

            if (ierr > 0)
            {
                return FLUID_NOT_FOUND;
            }
        }

        LIMITSdll(htype, &x[0], &Tmin, &Tmax, &Dmax, &Pmax, lengthofreference);   // Get Tmin, Tmax, Dmzx, Pmax for this fluid/mixture
        WMOLdll(&x[0], &wmm);                                                     // Get molar mass [kg/kmol or g/mol]

        if (ncomp > 1)                                                            // This is a mixture.  Call SATSPLN
        {
            SATSPLNdll(&x[0], &ierr, herr, errormessagelength);
            // TODO: Maybe create a function that sets a flag to enable/disable this SATSPLN default call

            if (ierr > 0) return SATSPLN_FAILED;
        }
        LastFluid = strFluid;

    }
    return ierr;           // return the error flag from SETUP0

}           
    