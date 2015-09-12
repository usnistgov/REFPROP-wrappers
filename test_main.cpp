#include "REFPROP.h"
#include <iostream>
#include <vector>

using namespace REFPROP;

int main()
{
	
	SetupReturnStruct setup = Setup("Methane.fld|Ethane.fld");
	FlashReturnStruct flash = FlashMolarRPSI(PQ_INPUTS, 101.325, 1, std::vector<double>(2, 0.5));

	double out_RP = REFPROP::REFPROP("Dmolar","Q",0,"P",101.325,"Water.fld");
	double out_SI = REFPROP::REFPROPSI("Dmolar","Q",0,"P",101325,"Water.fld");

    return EXIT_SUCCESS;
}