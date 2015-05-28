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

#define  CONF_PANEL                       1       /* callback function: OnConfirmPanel */
#define  CONF_PANEL_YES                   2       /* control type: command, callback function: OnConfirmYes */
#define  CONF_PANEL_NO                    3       /* control type: command, callback function: OnConfirmNo */
#define  CONF_PANEL_MESSAGE               4       /* control type: textMsg, callback function: (none) */

#define  MAIN_PANEL                       2       /* callback function: OnMainPanel */
#define  MAIN_PANEL_RESULT_TABLE          2       /* control type: table, callback function: (none) */
#define  MAIN_PANEL_STOP_TEST             3       /* control type: command, callback function: OnStopTest */
#define  MAIN_PANEL_START_TEST            4       /* control type: command, callback function: OnStartTest */
#define  MAIN_PANEL_SERIAL_NUM            5       /* control type: string, callback function: (none) */
#define  MAIN_PANEL_DECORATION_2          6       /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_DECORATION_3          7       /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_STATUS_BAR            8       /* control type: textMsg, callback function: (none) */
#define  MAIN_PANEL_STOPONFAIL            9       /* control type: binary, callback function: (none) */
#define  MAIN_PANEL_TEST_RESULT           10      /* control type: string, callback function: (none) */
#define  MAIN_PANEL_DECORATION            11      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_TEXTMSG               12      /* control type: textMsg, callback function: (none) */
#define  MAIN_PANEL_OPTIONS_BTN           13      /* control type: command, callback function: OnSetupOptions */
#define  MAIN_PANEL_TEST_TIME             14      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_TIMER                 15      /* control type: timer, callback function: OnTimer */
#define  MAIN_PANEL_MODULE_LEVEL          16      /* control type: textMsg, callback function: (none) */
#define  MAIN_PANEL_DECORATION_5          17      /* control type: deco, callback function: (none) */
#define  MAIN_PANEL_BOARD_REV             18      /* control type: textMsg, callback function: (none) */
#define  MAIN_PANEL_DECORATION_4          19      /* control type: deco, callback function: (none) */

#define  MSG_PANEL                        3       /* callback function: OnMessagePanel */
#define  MSG_PANEL_MESSAGE                2       /* control type: textMsg, callback function: (none) */
#define  MSG_PANEL_OK                     3       /* control type: command, callback function: OnMessageOK */

#define  NFC_PANEL                        4       /* callback function: OnNfcPanel */
#define  NFC_PANEL_TEXTMSG                2       /* control type: textMsg, callback function: (none) */

#define  OPTIONS                          5       /* callback function: OnOptionsPanelCallback */
#define  OPTIONS_USE_RS232_PWR            2       /* control type: radioButton, callback function: (none) */
#define  OPTIONS_USE_PWR_SPLY             3       /* control type: radioButton, callback function: (none) */
#define  OPTIONS_DECORATION_2             4       /* control type: deco, callback function: (none) */
#define  OPTIONS_LOGFILE_APPEND           5       /* control type: radioButton, callback function: (none) */
#define  OPTIONS_DECORATION_4             6       /* control type: deco, callback function: (none) */
#define  OPTIONS_DECORATION_3             7       /* control type: deco, callback function: (none) */
#define  OPTIONS_DECORATION               8       /* control type: deco, callback function: (none) */
#define  OPTIONS_TEXTMSG_4                9       /* control type: textMsg, callback function: (none) */
#define  OPTIONS_TEXTMSG_3                10      /* control type: textMsg, callback function: (none) */
#define  OPTIONS_TEXTMSG                  11      /* control type: textMsg, callback function: (none) */
#define  OPTIONS_TEXTMSG_2                12      /* control type: textMsg, callback function: (none) */
#define  OPTIONS_CURRENT                  13      /* control type: numeric, callback function: (none) */
#define  OPTIONS_VOLTAGE                  14      /* control type: numeric, callback function: (none) */
#define  OPTIONS_GPS101_PORT              15      /* control type: numeric, callback function: (none) */
#define  OPTIONS_PWR_PORT                 16      /* control type: numeric, callback function: (none) */
#define  OPTIONS_PWR_ADDR                 17      /* control type: numeric, callback function: (none) */
#define  OPTIONS_BOARD_REV                18      /* control type: ring, callback function: OnBoardRevConfig */
#define  OPTIONS_STATION_CONFIG           19      /* control type: ring, callback function: OnStationConfig */

#define  SN_PANEL                         6       /* callback function: OnSnPanel */
#define  SN_PANEL_SERIAL_NUM              2       /* control type: string, callback function: OnSerialNumber */
#define  SN_PANEL_OK                      3       /* control type: command, callback function: OnSerialNumOK */
#define  SN_PANEL_PICTURE                 4       /* control type: picture, callback function: (none) */
#define  SN_PANEL_WARNING                 5       /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnBoardRevConfig(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnConfirmNo(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnConfirmPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnConfirmYes(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnMainPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnMessageOK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnMessagePanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnNfcPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnOptionsPanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSerialNumber(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSerialNumOK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSetupOptions(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSnPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnStartTest(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnStationConfig(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnStopTest(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnTimer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
