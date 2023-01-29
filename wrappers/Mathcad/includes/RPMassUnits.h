void RPMassUnits()
{
    // This routine sets the USER2 Units to be consistent with the units
    // returned by the Mathcad wrapper Legacy API functions.  These are
    // basically the "MASS SI" units ( P [MPa] & Energy [kJ/kg] ) with
    // the following two changes:
    //   * Thermal Conductivity, K, set to [W/m-K]
    //   * Surface Tension, N, set to [N/m]
    //
    // Then "USER2" units are used throughout the Mathcad wrapper so
    // that no unit conversions have to be performed outside of REFPROP
    // within the mathcad wrapper.
    // 
    // Although these are mass based units, composition still defaults to
    // a mole fraction basis, iMass = 0.
    //
    ierr = 0;
    char herr[errormessagelength];
    //                             T;P;D;H;S;W;I;E;TCX;N  ;M
    char hIn[refpropcharlength] = "0;0;0;0;0;0;0;0;552;601;0";  // Change only K and N from "common" units
    char hOut[refpropcharlength] = "UNITUSER2";
    char hUnits[refpropcharlength] = "USER2";
    double a =0.0, b = 0.0, c, q;

    // Use this pop-up window for debugging if needed
    //===============================================================================
    // msg = "Inside RPMassUnits().\nAttempting to set USER2 default units.";
    // MessageBox(hwndDlg, msg.c_str(), "NIST RefProp Add-In", 0);

    int iEnum = 1;    // Just search property units strings.
    GETENUMdll(&iEnum, hUnits, &iUnits, &ierr, herr, refpropcharlength, errormessagelength);
 
    if (ierr != 0)
    {
        // Pop-up window if GETENUM fails (it shouldn't)
        //===============================================================================
        msg = "Error getting iUnits code for \"USER2\".";
        MessageBox(hwndDlg, msg.c_str(), "NIST RefProp Add-In", 0);
    }
    else
    {
        // Use this pop-up window for debugging if needed
        //===============================================================================
        // msg = "iUnits set to USER2 ENUM = " + std::to_string(iUnits);
        // MessageBox(hwndDlg, msg.c_str(), "NIST RefProp Add-In", 0);

        REFPROP1dll(hIn, hOut, &iUnits, &iMass, &a, &b, &x[0], &c, &q, &ierr, herr, refpropcharlength, refpropcharlength, errormessagelength);

        if (ierr != 0)
        {
            // Pop-Up window if REFPROP failes to modify the USER2 Units for K and STN
            //===============================================================================
            msg = "Error setting USER2 Units to \"0;0;0;0;0;0;0;0;552;601;0\"";
            MessageBox(hwndDlg, msg.c_str(), "NIST RefProp Add-In", 0);
        }
        else
        {
            // msg = "USER2 units successfully modified to ";
            // msg.append(hIn);
            // MessageBox(hwndDlg, msg.c_str(), "NIST RefProp Add-In", 0);
        }

    }

    return;
}
