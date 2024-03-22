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

#define  HIST_PANEL                       1
#define  HIST_PANEL_HIST_FILTERED         2       /* control type: graph, callback function: (none) */
#define  HIST_PANEL_HIST_RAW              3       /* control type: graph, callback function: (none) */
#define  HIST_PANEL_BTN_CLOSE             4       /* control type: command, callback function: closeHistPanel */
#define  HIST_PANEL_BOX_RAW_MIN           5       /* control type: numeric, callback function: (none) */
#define  HIST_PANEL_BOX_RAW_DISP          6       /* control type: numeric, callback function: (none) */
#define  HIST_PANEL_BOX_RAW_ZERO          7       /* control type: numeric, callback function: (none) */
#define  HIST_PANEL_BOX_RAW_MEAN          8       /* control type: numeric, callback function: (none) */
#define  HIST_PANEL_BOX_RAW_MAX           9       /* control type: numeric, callback function: (none) */
#define  HIST_PANEL_BOX_FILTERED_MIN      10      /* control type: numeric, callback function: (none) */
#define  HIST_PANEL_BOX_FILTERED_MAX      11      /* control type: numeric, callback function: (none) */
#define  HIST_PANEL_BOX_FILTERED_ZERO     12      /* control type: numeric, callback function: (none) */
#define  HIST_PANEL_BOX_FILTERED_MEAN     13      /* control type: numeric, callback function: (none) */
#define  HIST_PANEL_BOX_FILTERED_DISP     14      /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK closeHistPanel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif