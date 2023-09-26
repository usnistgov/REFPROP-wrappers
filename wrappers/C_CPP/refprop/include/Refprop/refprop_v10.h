#pragma once
#include <vector>
#include <string>
#include <map>
#include "manyso/native.h"
#include "REFPROP_lib.h"
#include "fluid_codes.h"
#include "enum.h"
#include "utils.h"

// Macro to cast a string to a const char*
#define STR_TO_CHAR(hrf) const_cast<char*>(hrf.c_str())

// Constants for C calls
const int kHerrLength = 255;                        // length of output error strings
const int kHunitsLength = 255;                      // length of output units
const int kHunitsArrayLength = 10000;               // length of output units array
const int kNumComp = 20;                            // max number of compositions in liquid or vapor phase
const int kNumOutputs = 200;                        // max number of output property values
const double kNothingCalculated = -9999990;         // value representing that nothing was calculated
const double kErrorOccurred = -9999970;             // value representing that an error occurred

class RefpropV10 {
public:
    RefpropV10();

    RefpropV10(const std::string& abs_path_dll);

    // Add many methods via variadic functions, each corresponding to a 1-to-1 wrapper of a function from the shared library (credit: manyso)
    #define X(name) template<class ...Args> void name(Args&&... args){ return lib_wrap_->getAddress<name ## _POINTER>(#name)(std::forward<Args>(args)...); };
        LIST_OF_REFPROP_FUNCTION_NAMES
    #undef X

    struct FluidData {
        int n_components;
        std::string fluid_files;
        std::string mixture_files;
        RefState ref_state;

        FluidData() : n_components(0), fluid_files(""), mixture_files(""), ref_state(RefState::_from_index(0)) {}
    };

    struct LibInputs {
        std::string hFld;
        std::string hIn;
        std::string hOut;
        Units iUnits;
        Basis iMass;
        int iFlag;
        double a;
        double b;
        std::vector<double> z;

        LibInputs() : hFld(""), hIn(""), hOut(""), iUnits(Units::_from_index(0)), iMass(Basis::_from_index(0)),
            iFlag(-1), a(NaN<double>), b(NaN<double>), z({}) {}
    };

    struct LibOutputs {
        std::string output_props;
        std::vector<double> Output;
        std::string hUnits;
        int iUCode;
        std::vector<int> iUCodeArray;
        double T;
        double P;
        double D;
        double Dl;
        double Dv;
        std::vector<double> x;
        std::vector<double> y;
        std::vector<double> x3;
        double q;
        double e;
        double h;
        double s;
        double Cv;
        double Cp;
        double w;
        int ierr;
        std::string herr;

        LibOutputs() : Output({}), hUnits(""), iUCode(-1), iUCodeArray({}), T(NaN<double>), P(NaN<double>), D(NaN<double>),
            Dl(NaN<double>), Dv(NaN<double>), x({}), y({}), x3({}), q(NaN<double>), e(NaN<double>),
            h(NaN<double>), s(NaN<double>), Cv(NaN<double>), Cp(NaN<double>), w(NaN<double>), ierr(-1), herr("") {}
    };

    RefpropV10::LibOutputs RefpropLib(RefpropV10::LibInputs inputs);
    RefpropV10::LibOutputs AbflshLib(RefpropV10::LibInputs inputs);
    RefpropV10::LibOutputs AllPropsLib(RefpropV10::LibInputs inputs);
    int FlagsLib(std::string option, int command=-999);
    int GetEnumLib(std::string units);
    int SetFluidsLib(const std::string& fluid_names);
    int SetupLib(const FluidData& fluid_data);

private:
    const AbstractSharedLibraryWrapper::load_method kLoadMethod_ = AbstractSharedLibraryWrapper::load_method::FROM_FILE;
    std::unique_ptr<NativeSharedLibraryWrapper> lib_wrap_;
    std::string abs_path_dll_;                         // path and file name
};

std::string ConstructErrorMessage(int error_code, const char* error_message);

std::map<std::string, double> IndexVectorByLabels(const std::vector<double>& values, const std::string& labels);
