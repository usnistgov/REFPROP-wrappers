#define REFPROP_IMPLEMENTATION                      // only include in one file and not the header
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "refprop_v10.h"
#include "utils.h"

const char* kRefpropEnv = "RPprefix";               // environment variable holding REFPROP path
const char* kRefpropLib = "REFPRP64.DLL";           // REFPROP library

// Constants for C calls
const int kHerrLength = 255;                        // length of output error strings
const int kHunitsLength = 255;                      // length of output units
const int kHunitsArrayLength = 10000;               // length of output units array
const int kNumComp = 20;                            // max number of compositions in liquid or vapor phase
const int kNumOutputs = 200;                        // max number of output property values
const double kNothingCalculated = -9999990;         // value representing that nothing was calculated
const double kErrorOccurred = -9999970;             // value representing that an error occurred

RefpropV10::RefpropV10()
{
    // Get path to REFPROP
    const char* refprop_path = std::getenv(kRefpropEnv);
    if (refprop_path) {
        abs_path_dll_ = std::string(refprop_path) + '/' + std::string(kRefpropLib);
        std::replace(abs_path_dll_.begin(), abs_path_dll_.end(), '\\', '/');    // include only '/' in file path
    }
    else {
        throw std::runtime_error(std::string("REFPROP path is not in environment variable ") + kRefpropEnv);
    }

    // Verify path to REFPROP
    if (!file_exists(abs_path_dll_)) {
        throw std::runtime_error("REFPROP version 10 library path does not point to a file.");
    }
    
    // Wrap refprop library via manyso to manage memory
    lib_wrap_ = std::make_unique<NativeSharedLibraryWrapper>(abs_path_dll_, kLoadMethod_);

    // Set path to fluid data
    std::string dll_path_string = parent_path(abs_path_dll_);
    this->SETPATHdll(
        STR_TO_CHAR(dll_path_string),
        dll_path_string.length() + 1
    );
}

RefpropV10::LibOutputs RefpropV10::RefpropLib(RefpropV10::LibInputs inputs) {
    // NOTE: passing inputs by value as library expects non-const references

    // Strip const-ness of input conversions
    int iUnits = GetEnumLib(inputs.iUnits._to_string());
    int iMass = inputs.iMass._to_integral();

    // Declare outputs
    double Output[kNumOutputs];                             // array of properties
    char hUnits[kHunitsLength];                             // units for first property in Output
    int iUCode = -1;                                        // units of the first property in Output
    double x[kNumComp];                                     // composition of the liquid phase of each component
    double y[kNumComp];                                     // composition of the vapor phase of each component
    double x3[kNumComp];                                    // composition of a second liquid phase of each component
    double q = NaN<double>;                                 // vapor quality
    int ierr = -1;                                          // error flag
    char herr[kHerrLength];                                 // error string

    // Initialize the above C-style array outputs
    std::fill_n(Output, kNumOutputs, NaN<double>);
    std::fill_n(hUnits, kHunitsLength, '\0');
    std::fill_n(x, kNumComp, NaN<double>);
    std::fill_n(y, kNumComp, NaN<double>);
    std::fill_n(x3, kNumComp, NaN<double>);
    std::fill_n(herr, kHerrLength, '\0');

    // Call subroutine
    this->REFPROPdll(
        STR_TO_CHAR(inputs.hFld),                           // hFld
        STR_TO_CHAR(inputs.hIn),                            // hIn
        STR_TO_CHAR(inputs.hOut),                           // hOut
        iUnits,                                             // iUnits
        iMass,                                              // iMass
        inputs.iFlag,                                       // iFlag
        inputs.a,                                           // a
        inputs.b,                                           // b
        inputs.z.data(),                                    // z
        Output,                                             // Output
        hUnits,                                             // hUnits
        iUCode,                                             // iUCode
        x,                                                  // x
        y,                                                  // y
        x3,                                                 // x3
        q,                                                  // q
        ierr,                                               // ierr
        herr,                                               // herr
        inputs.hFld.length() + 1,                           // hFld_length, add one for null terminator character
        inputs.hIn.length() + 1,                            // hIn_length
        inputs.hOut.length() + 1,                           // hOut_length
        kHunitsLength,                                      // hUnits_length
        kHerrLength                                         // herr_length
    );

    std::string error = StripTrailingWhiteSpace(std::string(herr));
    if (ierr > 0) printf("Error calling subroutine: %d. Message: %s\n", ierr, error.c_str());

    // Cleanly format 'Output' values
    std::vector<double> output(std::begin(Output), std::end(Output));                       // convert Output array to vector
    while (!output.empty() && output.back() == kNothingCalculated) { output.pop_back(); }   // remove trailing not-calculated values
    std::replace(output.begin(), output.end(), kNothingCalculated, NaN<double>);            // replace other 'not-calculated' with NaN
    std::replace(output.begin(), output.end(), kErrorOccurred, -inf<double>);               // replace error values with -infinity

    // Populate output structure
    RefpropV10::LibOutputs outputs;
    outputs.output_props = inputs.hOut;
    outputs.Output = output;
    outputs.hUnits = StripTrailingWhiteSpace(std::string(hUnits));                          // strip trailing spaces
    outputs.iUCode = iUCode;
    int n_components = inputs.z.size();                                                     // output for actual number of components
    outputs.x.assign(std::begin(x), std::begin(x) + n_components);
    outputs.y.assign(std::begin(y), std::begin(y) + n_components);
    outputs.x3.assign(std::begin(x3), std::begin(x3) + n_components);
    outputs.q = q;
    outputs.ierr = ierr;
    outputs.herr = error;

    return outputs;
}

RefpropV10::LibOutputs RefpropV10::AbflshLib(RefpropV10::LibInputs inputs)
{
    // NOTE: passing inputs by value as library expects non-const references
    // TODO: combine enum flags
    
    // Declare outputs
    double T = NaN<double>;                                 // [K] temperature
    double P = NaN<double>;                                 // [kPa] pressure
    double D = NaN<double>;                                 // [mol/L or kg/m3] density
    double Dl = NaN<double>;                                // [mol/L or kg/m3] density of the liquid phase
    double Dv = NaN<double>;                                // [mol/L or kg/m3] density of the vapor phase
    double x[kNumComp];                                     // composition of the liquid phase of each component
    double y[kNumComp];                                     // composition of the vapor phase of each component
    double q = NaN<double>;                                 // vapor quality on a molar basis
    double e = NaN<double>;                                 // [J/mol or kJ/kg] internal energy
    double h = NaN<double>;                                 // [J/mol or kJ/kg] enthalpy
    double s = NaN<double>;                                 // [J/mol-K or kJ/kg-K] entropy
    double Cv = NaN<double>;                                // [J/mol-K or kJ/kg-K] isochoric heat capacity
    double Cp = NaN<double>;                                // [J/mol-K or kJ/kg-K] isobaric heat capacity
    double w = NaN<double>;                                 // [m/s] speed of sound
    int ierr = -1;                                          // error flag
    char herr[kHerrLength];                                 // error string

    // Initialize the above C-style array outputs
    std::fill_n(x, kNumComp, NaN<double>);
    std::fill_n(y, kNumComp, NaN<double>);
    std::fill_n(herr, kHerrLength, '\0');

    // Call subroutine
    this->ABFLSHdll(
        STR_TO_CHAR(inputs.hIn),                            // ab
        inputs.a,                                           // a
        inputs.b,                                           // b
        inputs.z.data(),                                    // z
        inputs.iFlag,                                       // iFlag
        T,
        P,
        D,
        Dl,
        Dv,
        x,
        y,
        q,
        e,
        h,
        s,
        Cv,
        Cp,
        w,
        ierr,
        herr,
        inputs.hIn.length() + 1,                            // ab_length
        kHerrLength                                         // herr_length
    );

    std::string error = StripTrailingWhiteSpace(std::string(herr));
    if (ierr > 0) printf("Error calling subroutine: %d. Message: %s\n", ierr, error.c_str());

    // Populate output structure
    RefpropV10::LibOutputs outputs;
    outputs.output_props = "T;P;D;Dl;Dv;q;e;h;s;Cv;Cp;w";
    outputs.Output = {T, P, D, Dl, Dv, q, e, h, s, Cv, Cp, w};
    int n_components = inputs.z.size();                                             // output for actual number of components
    outputs.T = T;
    outputs.P = P;
    outputs.D = D;
    outputs.Dl = Dl;
    outputs.Dv = Dv;
    outputs.x.assign(std::begin(x), std::begin(x) + n_components);
    outputs.y.assign(std::begin(y), std::begin(y) + n_components);
    outputs.q = q;                                                                  // molar basis
    outputs.e = e;
    outputs.h = h;
    outputs.s = s;
    outputs.Cv = Cv;
    outputs.Cp = Cp;
    outputs.w = w;
    outputs.ierr = ierr;
    outputs.herr = error;
    
    return outputs;
}

RefpropV10::LibOutputs RefpropV10::AllPropsLib(RefpropV10::LibInputs inputs)
{
    // NOTE: passing inputs by value as library expects non-const references

    // Strip const-ness of input conversions
    int units = GetEnumLib(inputs.iUnits._to_string());
    int basis = inputs.iMass._to_integral();

    // Declare outputs
    double Output[kNumOutputs];                             // array of properties
    char hUnitsArray[kHunitsArrayLength];                   // units for properties in Output
    int iUCodeArray[kNumOutputs];                           // units of the properties in Output
    int ierr = -1;                                          // error flag
    char herr[kHerrLength];                                 // error string

    // Initialize the above C-style array outputs
    std::fill_n(Output, kNumOutputs, NaN<double>);
    std::fill_n(hUnitsArray, kHunitsArrayLength, '\0');
    std::fill_n(iUCodeArray, kNumOutputs, -1);
    std::fill_n(herr, kHerrLength, '\0');

    // Call subroutine
    this->ALLPROPSdll(
        STR_TO_CHAR(inputs.hOut),                           // hOut
        units,                                              // iUnits
        basis,                                              // iMass
        inputs.iFlag,                                       // iFlag
        inputs.a,                                           // T
        inputs.b,                                           // D
        inputs.z.data(),                                    // z
        Output,                                             // Output
        hUnitsArray,                                        // hUnits
        iUCodeArray,                                        // iUCode
        ierr,                                               // ierr
        herr,                                               // herr
        inputs.hOut.length() + 1,                           // hOut_length
        kHunitsArrayLength,                                 // hUnitsArray_length
        kHerrLength                                         // herr_length
    );

    // Cleanly format 'Output' values
    std::vector<double> output(std::begin(Output), std::end(Output));                       // convert output array to vector
    while (!output.empty() && output.back() == kNothingCalculated) { output.pop_back(); }   // remove trailing not-calculated values
    std::replace(output.begin(), output.end(), kNothingCalculated, NaN<double>);            // replace other 'not-calculated' with NaN
    std::replace(output.begin(), output.end(), kErrorOccurred, -inf<double>);               // replace error values with -infinity

    // Populate output structure
    RefpropV10::LibOutputs outputs;
    outputs.output_props = inputs.hOut;
    outputs.Output = output;
    outputs.hUnits = StripTrailingWhiteSpaceAndPipes(std::string(hUnitsArray));
    outputs.iUCodeArray.assign(std::begin(iUCodeArray), std::begin(iUCodeArray) + output.size());
    outputs.ierr = ierr;
    outputs.herr = StripTrailingWhiteSpace(std::string(herr));

    return outputs;
}

int RefpropV10::GetEnumLib(std::string units)
{
    // Set flag to specify type of enumerated value to return
    int iFlag = 0;                                          // 0 = check all strings possible

    // Replace any underscores (which the enums have) to spaces
    std::replace(units.begin(), units.end(), '_', ' ');

    // Declare outputs and initialize
    int iEnum = -1;
    int ierr = -1;
    char herr[kHerrLength];
    std::fill_n(herr, kHerrLength, '\0');

    // Call subroutine
    this->GETENUMdll(
        iFlag,                                              // type of enumerated value to return
        STR_TO_CHAR(units),                                 // hEnum, string representation of the enum to return
        iEnum,                                              // the enum that is returned
        ierr,
        herr,
        units.length() + 1,
        kHerrLength
    );

    std::string error = StripTrailingWhiteSpace(std::string(herr));
    if (ierr > 0) printf("Error calling subroutine: %d. Message: %s\n", ierr, error.c_str());

    return iEnum;
}

int RefpropV10::FlagsLib(std::string option, int command)
{
    // Declare outputs and initialize
    int setting = -1;
    int ierr = -1;
    char herr[kHerrLength];
    std::fill_n(herr, kHerrLength, '\0');

    // Replace any underscores in option name with spaces
    std::replace(option.begin(), option.end(), '_', ' ');

    // Call subroutine
    this->FLAGSdll(
        STR_TO_CHAR(option),                                // hFlag, indicator for the option to set
        command,                                            // jFlag, command for the option
        setting,                                            // kFlag, current setting of the option
        ierr,
        herr,
        option.length() + 1,
        kHerrLength
    );

    std::string error = StripTrailingWhiteSpace(std::string(herr));
    if (ierr > 0) printf("Error calling subroutine: %d. Message: %s\n", ierr, error.c_str());

    return setting;
}

int RefpropV10::SetFluidsLib(const std::string& fluid_names)
{
    int ierr = -1;
    
    // Call subroutine
    this->SETFLUIDSdll(
        STR_TO_CHAR(fluid_names),                           // hFld
        ierr,
        fluid_names.length() + 1
    );

    return ierr;
}

int RefpropV10::SetupLib(const FluidData& fluid_data) {
    // TODO: verify fluid files exist using filesystem

    // Parameters:
    int ncomp = fluid_data.n_components;                    // number of components
    std::string hFiles = fluid_data.fluid_files;            // array of file names
    std::string hFmix = fluid_data.mixture_files;           // mixture coefficient file name
    std::string hrf = fluid_data.ref_state._to_string();    // reference state

    // Outputs:
    int ierr = 0;                                           // error flag
    char herr[kHerrLength];                                 // error string

    this->SETUPdll(
        ncomp,
        STR_TO_CHAR(hFiles),
        STR_TO_CHAR(hFmix),
        STR_TO_CHAR(hrf),
        ierr,
        herr,
        hFiles.length() + 1,                                // add one for null terminator character
        hFmix.length() + 1,
        hrf.length() + 1,
        kHerrLength
    );

    if (ierr != 0) {
        throw std::runtime_error(ConstructErrorMessage(ierr, herr));
    }

    return ierr;
}

std::string ConstructErrorMessage(int errorCode, const char* errorMessage) {
    std::stringstream ss;
    ss << "Error code: " << errorCode << ". Error message: " << errorMessage;
    return ss.str();
}

std::map<std::string, double> IndexVectorByLabels(const std::vector<double>& values, const std::string& labels) {
    std::map<std::string, double> result;
    std::istringstream label_stream(labels);
    std::string label;

    for (const double& value : values) {
        if (std::getline(label_stream, label, ';')) {
            result[label] = value;
        }
    }

    return result;
}
