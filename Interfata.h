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

#define  FREQ_PANEL                       1       /* callback function: OnFreqPanelCB */
#define  FREQ_PANEL_SW_FREQ               2       /* control type: binary, callback function: SwitchToFreq */
#define  FREQ_PANEL_FILT_DS               3       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_FILT_NO_DS            4       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_RAW_DS                5       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_RAW_NO_DS             6       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_SPECTRE_WINDOWS       7       /* control type: ring, callback function: (none) */
#define  FREQ_PANEL_SPECTRE_FILTERS       8       /* control type: ring, callback function: (none) */
#define  FREQ_PANEL_CTRL_NUM_OF_POINTS    9       /* control type: scale, callback function: getNpoints */
#define  FREQ_PANEL_FREQ_TIMER            10      /* control type: timer, callback function: updateSpecters */

#define  MAIN_PANEL                       2       /* callback function: OnPanel */
#define  MAIN_PANEL_GRAPH_FILTERED        2       /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_GRAPH_RAW             3       /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_BTN_SHOW_ENV          4       /* control type: command, callback function: showEnvelope */
#define  MAIN_PANEL_BTN_SHOW_FULL         5       /* control type: command, callback function: displayFullData */
#define  MAIN_PANEL_BTN_SAVE_DATA         6       /* control type: command, callback function: saveData */
#define  MAIN_PANEL_BTN_GETDATA           7       /* control type: command, callback function: getInfo */
#define  MAIN_PANEL_BTN_LOAD_HIST         8       /* control type: command, callback function: showHist */
#define  MAIN_PANEL_BTN_LOAD              9       /* control type: command, callback function: OnLoadButton */
#define  MAIN_PANEL_COMMANDBUTTON_2       10      /* control type: command, callback function: nextInt */
#define  MAIN_PANEL_COMMANDBUTTON         11      /* control type: command, callback function: prevInt */
#define  MAIN_PANEL_BOX_RIGHT_INT         12      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_BOX_LEFT_INT          13      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_BTN_UPD               14      /* control type: command, callback function: UpdateGraph */
#define  MAIN_PANEL_DECORATION            15      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_DECORATION_2          16      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_MENU_FILTERS          17      /* control type: ring, callback function: getFilterType */
#define  MAIN_PANEL_BOX_MEDIAN            18      /* control type: numeric, callback function: getElemForMedian */
#define  MAIN_PANEL_BOX_ALPHA             19      /* control type: numeric, callback function: getAlphaValue */
#define  MAIN_PANEL_DECORATION_5          20      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_DECORATION_4          21      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_TEXT_KURT             22      /* control type: textMsg, callback function: (none) */
#define  MAIN_PANEL_SW_FREQ               23      /* control type: binary, callback function: SwitchToFreq */
#define  MAIN_PANEL_TEXT_SKEW             24      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK displayFullData(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK getAlphaValue(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK getElemForMedian(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK getFilterType(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK getInfo(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK getNpoints(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK nextInt(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFreqPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK prevInt(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK saveData(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK showEnvelope(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK showHist(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SwitchToFreq(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK UpdateGraph(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK updateSpecters(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif