/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1
#define  PANEL_QUIT_BUTTON                2       /* control type: command, callback function: Quit */
#define  PANEL_IUNITS                     3       /* control type: ring, callback function: (none) */
#define  PANEL_REFPROP                    4       /* control type: command, callback function: RunREFPROP */
#define  PANEL_HOUT                       5       /* control type: string, callback function: (none) */
#define  PANEL_HIN                        6       /* control type: string, callback function: (none) */
#define  PANEL_HFLD                       7       /* control type: string, callback function: (none) */
#define  PANEL_OUTPUT_4                   8       /* control type: numeric, callback function: (none) */
#define  PANEL_OUTPUT_3                   9       /* control type: numeric, callback function: (none) */
#define  PANEL_OUTPUT_2                   10      /* control type: numeric, callback function: (none) */
#define  PANEL_OUTPUT_1                   11      /* control type: numeric, callback function: (none) */
#define  PANEL_OUTPUT_0                   12      /* control type: numeric, callback function: (none) */
#define  PANEL_IMASS                      13      /* control type: numeric, callback function: (none) */
#define  PANEL_IFLAG                      14      /* control type: numeric, callback function: (none) */
#define  PANEL_Z_2                        15      /* control type: numeric, callback function: (none) */
#define  PANEL_Z_1                        16      /* control type: numeric, callback function: (none) */
#define  PANEL_Z_0                        17      /* control type: numeric, callback function: (none) */
#define  PANEL_B                          18      /* control type: numeric, callback function: (none) */
#define  PANEL_A                          19      /* control type: numeric, callback function: (none) */
#define  PANEL_HUNITS                     20      /* control type: string, callback function: (none) */
#define  PANEL_IUCODE                     21      /* control type: numeric, callback function: (none) */
#define  PANEL_Q                          22      /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

#define  CTRLARRAY                        1

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RunREFPROP(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
