LRESULT rp_Getpath( 
    LPMCSTRING          hstr,     // pointer to the results string
    LPCCOMPLEXSCALAR    idum   )  // pointer to the dummy parameter received from Mathcad
{
    // NOTE: we don't care what gets passed in, but this clears the unused variable warning.
    if (idum->imag != 0.0)
        return MAKELRESULT(MUST_BE_REAL, 2);

    // Return string should already be stored when REFPROP DLL is loaded...
    char * c = MathcadAllocate((unsigned int)fluidPath.size() + 1u);        // Create a c-string (pointer) with same size as s
                                                                            // This process avoids the const-cast type which would result from,
                                                                            //     instead, converting the string using s.c_str()
    std::copy(fluidPath.begin(), fluidPath.end(), c);                       // Copy s into c. 
    c[fluidPath.size()] = '\0';                                             // Append with the null character
    hstr->str = c;                                                          // Assign the string to the output parameter
    return 0;                                                               // normal return
}

FUNCTIONINFO    rp_getpath = 
{                                                               // These first three strings are not actually used!!!
    (char *)("rp_getpath"),                                     // Name by which Mathcad will recognize the function
    (char *)("idum"),                                           // rp_getpath will be called as rp_getpath(idum)
    (char *)("Returns the full path for the fluid directory"),  // description of rp_getpath(idum)					
    (LPCFUNCTION)rp_Getpath,                                    // pointer to the executable code
    MC_STRING,                                                  // the return type is an MC_STRING
    1,                                                          // the function takes one (1) dummy argument
    { COMPLEX_SCALAR }                                          // argument is a scalar
};

LRESULT rp_GetDLL(
    LPMCSTRING          hstr,     // pointer to the results string
    LPCCOMPLEXSCALAR    idum)  // pointer to the dummy parameter received from Mathcad
{
    // NOTE: we don't care what gets passed in, but this clears the unused variable warning.
    if (idum->imag != 0.0)
        return MAKELRESULT(MUST_BE_REAL, 2);

    // Return string should already be stored when REFPROP DLL is loaded...
    char * c = MathcadAllocate((unsigned int)RPPath_loaded.size() + 1u);    // Create a c-string (pointer) with same size as s
                                                                            // This process avoids the const-cast type which would result from,
                                                                            //     instead, converting the string using s.c_str()
    std::copy(RPPath_loaded.begin(), RPPath_loaded.end(), c);               // Copy s into c. 
    c[RPPath_loaded.size()] = '\0';                                         // Append with the null character
    hstr->str = c;                                                          // Assign the string to the output parameter
    return 0;                                                               // normal return
}

FUNCTIONINFO    rp_getDLL =
{                                                                 // These first three strings are not actually used!!!
    (char *)("rp_getDLL"),                                       // Name by which Mathcad will recognize the function
    (char *)("idum"),                                             // rp_getDLL will be called as rp_getDLL(idum)
    (char *)("Returns the full path to the REFPROP DLL Loaded"),  // description of rp_getDLL(idum)					
    (LPCFUNCTION)rp_GetDLL,                                       // pointer to the executable code
    MC_STRING,                                                    // the return type is an MC_STRING
    1,                                                            // the function takes one (1) dummy argument
    { COMPLEX_SCALAR }                                            // argument is a scalar
};

//******************************************************************************************************//
//**  This is a new wrapper function with the REFPROP 10 update to SET the fluid path                 **//
//******************************************************************************************************//
//  Takes a path string as a parameter to either the REFPROP DLL directory or the actual fluid          //
//      directory.  The Mathcad Prime file picker can be used to select the full path to the actual     //
//      REFPROP DLL file in the parent directory or a single fluid file in the and the path will be     //
//      extracted (or the user can do this manually once the string is returned from the file picker).  //
//                                                                                                      //
//  NOTE: The parent folder ABOVE the \FLUIDS and \MIXTURES folders should be selected.                 //
//        If the folder selected ends with \FLUIDS or \MIXTURES, the parent path will be set.           //
//******************************************************************************************************//

LRESULT rp_SetPath(
    LPMCSTRING          hstr,      // pointer to the results string
    LPCMCSTRING        hpath)      // pointer to the dummy parameter received from Mathcad
{
    // This function can actually be called in versions earlier than REFPROP 10, no need to check version
    std::string flstr = hpath->str;

    if (!flstr.empty())                                                           // Clean up the passed string parameter.
    {
        std::string ext,lastdir;
        size_t index;

        // See if string ends with .DLL or .FLD
        index = flstr.find_last_of(".");                               // Find the last '.' before the DLL name
        if (index != flstr.npos)                                       // File extension found
        {
            ext = upper(flstr.substr(index, flstr.length() - 1));
            if ((ext == ".FLD") || (ext == ".DLL") || 
                (ext == ".MIX") || (ext == ".PPF"))                    //     If extension was .FLD, .MIX, or .DLL
            {
                index = flstr.find_last_of("\\");
                flstr.resize(index);                                   //         strip file name off path
            }
            else                                                       //      otherwise
                return MAKELRESULT(BAD_FILE, 2);                       //         throw invalid file error
        }

        // See if path string ends with /FLUIDS or /MIXTURES
        index = flstr.find_last_of("\\");                              // Find the last '\' before the last subdirectory
        lastdir = upper(flstr.substr(index, flstr.length() - 1));
        if ((lastdir == "\\FLUIDS") || (lastdir == "\\MIXTURES"))      // If last directory is /FLUIDS or /MIXTURES
            flstr.resize(index);                                       //    Strip last directory off of path

        fluidPath = flstr;                                             // Set path to parent directory above /FLUIDS

        char cPath[255] = { 0 };                                       // Initialize char array cpath
        std::copy(fluidPath.begin(), fluidPath.end(), cPath);          // Copy string fluidPath into c_str cpath.
        cPath[fluidPath.size()] = '\0';                                // Append with a null character (not sure if needed)
        SETPATHdll(cPath, 255);                                        // Set path to fluids library

    }

    // Return string should already be stored when REFPROP DLL is loaded...
    char * c = MathcadAllocate((unsigned int)fluidPath.size() + 1u);        // Create a c-string (pointer) with same size as s
                                                                            // This process avoids the const-cast type which would result from,
                                                                            //     instead, converting the string using s.c_str()
    std::copy(fluidPath.begin(), fluidPath.end(), c);                       // Copy s into c. 
    c[fluidPath.size()] = '\0';                                             // Append with the null character
    hstr->str = c;                                                          // Assign the string to the output parameter
    return 0;                                                               // normal return
}

FUNCTIONINFO    rp_SETPATH =
{                                                               // These first three strings are not actually used!!!
    (char *)("rp_SETPATH"),                                     // Name by which Mathcad will recognize the function
    (char *)("hpath"),                                          // rp_SETPATH will be called as rp_SETPATH(hpath)
    (char *)("Sets the path to the \\FLUIDS directory"),        // description of rp_SETPATH(hpath)
    (LPCFUNCTION)rp_SetPath,                                    // pointer to the executable code
    MC_STRING,                                                  // the return type is an MC_STRING
    1,                                                          // the function takes one argument
    { MC_STRING }                                               // argument is a string
};
