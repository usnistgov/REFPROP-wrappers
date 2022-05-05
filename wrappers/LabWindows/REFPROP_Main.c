/*
Questions about this software can be directed to: Damian.Lauria@nist.gov or Ian.Bell@nist.gov 
*/

#define SIZE_T_TYPE unsigned int
#include "REFPROP_lib.h"
#include "REFPROP_Interface.h"  

#include <stdlib.h>
#include <stdio.h>
#include <userint.h> 
#include <REFPROP_interface.h>  

#ifndef NOMINMAX
#define NOMINMAX
#include <windows.h>
#undef NOMINMAX
#else 
#include <windows.h>
#endif

static HINSTANCE RefpropdllInstance = NULL;
static REFPROPdll_POINTER REFPROPdll;
static SETPATHdll_POINTER SETPATHdll;

void* getFunctionPointer(const char* name) {
    return (void*)GetProcAddress(RefpropdllInstance, name);
}
void holder();

int main()
{
    // You may need to change this path to suit your installation
    // Note: forward-slashes are recommended.
    const char * abspath = "C:/Program Files (x86)/REFPROP/REFPROP.DLL";
    TCHAR refpropdllstring[MAX_PATH];
    strcpy((char*)refpropdllstring, abspath);
    RefpropdllInstance = LoadLibrary(refpropdllstring);

    int loaded_REFPROP = (RefpropdllInstance != NULL);
    printf("Loaded refprop (in Main.c): %s\n", loaded_REFPROP ? "true" : "false");
    if (!loaded_REFPROP) { return EXIT_FAILURE; }
    
    REFPROPdll = (REFPROPdll_POINTER)(getFunctionPointer("REFPROPdll"));
    SETPATHdll = (SETPATHdll_POINTER)(getFunctionPointer("SETPATHdll"));

    if (SETPATHdll == NULL) {
        printf("Could not load SETPATHdll from shared library\n");
        return EXIT_FAILURE; 
    }
 
    char* path = "C:/Program Files (x86)/REFPROP";
    SETPATHdll(path, 400);
	
	//**********  Start GUI **********
	int PanelHandle;
	
	if((PanelHandle = LoadPanel (0, "REFPROP_Interface.uir", PANEL)) < 0)		//creates a handle for the main panel
        return EXIT_FAILURE;	
		   
	
	DisplayPanel(PanelHandle);
	RunUserInterface (); //enters the interactive section of the program
		   
	//********************************/

//	holder();

    return EXIT_SUCCESS;
}


int CVICALLBACK Quit (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			QuitUserInterface (0);

		break;
	}
	return 0;
}


int CVICALLBACK RunREFPROP (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
		
	        int iUnits = 0;
			char hFld[10000] = "";
	        char hIn[256] = "";
	        char hOut[256] = "";
			int iMass = 0; 
	        int iFlag = 0;
	        
	        double a = 0;
	        double b = 0;
	        double z[20]; z[0] = .4;  z[1] = .6;

	        double Output[200];
	        char hUnits[256] = ""; 
	        int iUCode = -12345; 
	        double x[20], y[20], x3[20];
	        double q = -9999;
	        int ierr = 0;
	        char herr[256] = "";
			
			GetCtrlVal(panel, PANEL_HFLD, hFld);
			GetCtrlVal(panel, PANEL_HIN, hIn);  
			GetCtrlVal(panel, PANEL_HOUT, hOut);  
			GetCtrlVal(panel, PANEL_IUNITS, &iUnits);
			GetCtrlVal(panel, PANEL_IMASS, &iMass);
			GetCtrlVal(panel, PANEL_IFLAG, &iFlag); 
			GetCtrlVal(panel, PANEL_A, &a);
			GetCtrlVal(panel, PANEL_B, &b);
			GetCtrlVal(panel, PANEL_Z_0, &z[0]);
			GetCtrlVal(panel, PANEL_Z_1, &z[1]);
			GetCtrlVal(panel, PANEL_Z_2, &z[2]);
		
	        REFPROPdll(hFld, hIn, hOut, &iUnits, &iMass, &iFlag, &a, &b, z, Output, hUnits, &iUCode, x, y, x3, &q, &ierr, herr, 10000, 255, 255, 255, 255);
	        if (ierr > 0) printf("This ierr: %d herr: %s\n", ierr, herr);
    
			
			SetCtrlVal(panel, PANEL_OUTPUT_0, Output[0]);
			SetCtrlVal(panel, PANEL_OUTPUT_1, Output[1]);  
			SetCtrlVal(panel, PANEL_OUTPUT_2, Output[2]); 
			SetCtrlVal(panel, PANEL_OUTPUT_3, Output[3]); 
			SetCtrlVal(panel, PANEL_OUTPUT_4, Output[4]); 
			
			SetCtrlVal(panel, PANEL_HUNITS, hUnits);
			SetCtrlVal(panel, PANEL_IUCODE, iUCode);
			SetCtrlVal(panel, PANEL_Q, q);
			
	
		break;
	}
	return 0;
}
