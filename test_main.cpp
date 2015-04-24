#include "REFPROP.h"
#include <iostream>
#include <vector>

int main()
{
	SetupReturnStruct setup = Setup("Methane.fld|Ethane.fld");
	FlashReturnStruct flash = FlashMolar(PQ_INPUTS, 101.325, 1, std::vector<double>(2, 0.5));
    return EXIT_SUCCESS;
}