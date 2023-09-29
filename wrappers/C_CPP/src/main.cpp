#include "test_utils.h"
#include "refprop_v10.h"
#include "fluid_codes.h"

void TestGetEnumLib() {
    // Instantiate Refprop
    RefpropV10 refpropv10;

    // Call function and test outputs
    EXPECT_EQ(0, refpropv10.GetEnumLib("DEFAULT"));
    //EXPECT_EQ(-9999970, refpropv10.GetEnumLib("MOLE SI"));      // MOLE SI is currently not recognized
    EXPECT_EQ(2, refpropv10.GetEnumLib("MASS SI"));
    EXPECT_EQ(3, refpropv10.GetEnumLib("SI WITH C"));
    EXPECT_EQ(20, refpropv10.GetEnumLib("MOLAR BASE SI"));
    EXPECT_EQ(21, refpropv10.GetEnumLib("MASS BASE SI"));
    EXPECT_EQ(5, refpropv10.GetEnumLib("ENGLISH"));
    EXPECT_EQ(6, refpropv10.GetEnumLib("MOLAR ENGLISH"));
    EXPECT_EQ(7, refpropv10.GetEnumLib("MKS"));
    EXPECT_EQ(8, refpropv10.GetEnumLib("CGS"));
    EXPECT_EQ(9, refpropv10.GetEnumLib("MIXED"));
    EXPECT_EQ(10, refpropv10.GetEnumLib("MEUNITS"));
    EXPECT_EQ(11, refpropv10.GetEnumLib("USER"));
}

void TestSetFluidsLib() {
    const int SUCCESS = 0;
    
    // Instantiate Refprop
    RefpropV10 refpropv10;

    // Call function and test for success
    EXPECT_EQ(SUCCESS, refpropv10.SetFluidsLib("ARGON"));
    EXPECT_EQ(SUCCESS, refpropv10.SetFluidsLib("ARGON; NITROGEN"));
    EXPECT_NE(SUCCESS, refpropv10.SetFluidsLib("NOT_A_FLUID_NAME"));
}

void TestAbflshLib() {
    const double kRelTol = 0.001;    // 0.001 == 0.1 %

    // Instantiate Refprop
    RefpropV10 refpropv10;

    // Set fluid
    refpropv10.SetFluidsLib("NITROGEN;OXYGEN");

    // Define inputs
    RefpropV10::LibInputs inputs;
    inputs.hIn = "TP";
    inputs.a = 293;
    inputs.b = 101.325;
    inputs.z = { 0.5, 0.5 };
    inputs.iFlag = 0;

    // Call AbflshLib using inputs
    RefpropV10::LibOutputs outputs = refpropv10.AbflshLib(inputs);

    // Map outputs
    std::map<std::string, double> output_map = IndexVectorByLabels(outputs.Output, outputs.output_props);

    // Test outputs
    EXPECT_NEAR_REL(8516.9320996925780, output_map["h"], kRelTol);
}

void TestAllPropsLib() {
    const double kRelTol = 0.001;    // 0.001 == 0.1 %

    // Instantiate Refprop
    RefpropV10 refpropv10;

    // Set fluid
    int result = refpropv10.SetFluidsLib("NITROGEN;OXYGEN");

    // Define inputs
    RefpropV10::LibInputs inputs;
    inputs.hOut = "H;S";
    inputs.iUnits = Units::MOLAR_BASE_SI;
    inputs.iMass = Basis::MOLAR;
    inputs.iFlag = FlagAllPropsLib::WRITE_NO_UNITS;
    inputs.a = 293.;              // T [K]
    inputs.b = 41.613;            // D [mol/m3]
    inputs.z = { 0.5, 0.5 };

    // Call AllPropsLib using inputs
    RefpropV10::LibOutputs outputs = refpropv10.AllPropsLib(inputs);

    // Map and test outputs
    std::map<std::string, double> output_map = IndexVectorByLabels(outputs.Output, outputs.output_props);
    EXPECT_NEAR_REL(8516.9320480273145, output_map["H"], kRelTol);
}

void TestRefpropLib() {
    const double kRelTol = 0.001;    // 0.001 == 0.1 %

    // Instantiate Refprop
    RefpropV10 refpropv10;

    // Define inputs
    RefpropV10::LibInputs inputs;
    inputs.hFld = "WATER";
    inputs.hIn = "PQ";
    inputs.hOut = "T";
    inputs.iUnits = Units::DEFAULT;
    inputs.iMass = Basis::MOLAR;
    inputs.iFlag = FlagRefpropLib::NONE;
    inputs.a = 101.325;
    inputs.b = 0.;
    inputs.z = { 20., 0. };

    // Call Refprop using inputs
    RefpropV10::LibOutputs outputs = refpropv10.RefpropLib(inputs);

    // Map and test outputs
    std::map<std::string, double> output_map = IndexVectorByLabels(outputs.Output, outputs.output_props);
    EXPECT_NEAR_REL(373.1242958477, output_map["T"], kRelTol);
}

int main() {
    TestGetEnumLib();
    TestSetFluidsLib();
    TestAbflshLib();
    TestAllPropsLib();
    TestRefpropLib();

    std::cout << "Tests finished." << std::endl;
    return 0;
}
