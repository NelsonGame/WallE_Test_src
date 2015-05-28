#include "cvi_db.h"
#include <formatio.h>
//#include "animate.h"
#include "toolbox.h"
#include <utility.h>
#include <cvirte.h>		
#include <userint.h>
//#include "ActiveMindTest.h"
#include "WallE.h"
#include "TestCommon.h"

#include "DevComm.h"
#include "ansi_c.h"
#include "TestSequence.h"
#include "TestLog.h"

#include "PowerSupply.h"
#include "TestItems.h"
#include "toolbox.h"

//static int mainPanel;

int mainPanel; 
char factory_serial_number[256];
static char sernum[32]={0}; 

int g_overall_test_result=0;
int g_abort_test_flag=0;
int g_stop_on_fail=1;
int g_use_power_supply=1;
int g_use_rs232_power=0;
int g_gpib_pwr_addr=5;
//int g_pwr_serial_port=4;
int g_logfile_append_time;
int g_test_running;
int g_gps_101_port;
int g_dev_serial_port ;
//int g_use_gps_101;
int g_skip_firmware_loading;
int g_module_level_test;

ListType g_module_board_rev_list,g_pcba_board_rev_list;

char g_current_module_board_rev[256];
char g_current_module_board_file[256];

char g_current_pcba_board_rev[256];
char g_current_pcba_board_file[256];

double g_testStartTime;

double g_pwr_voltage=4.0,g_pwr_current=1.0;

int g_initial_x_pos,g_initial_y_pos;


int g_need_config_file=0;


char database_connector[50] ;
char initfilepath[50] ;
char station_name[50] ;
char location[50] ;


int database_hdl ;
int SQL_hdl ;

char TestDate[15] ;
char TestTime[10] ;

long long pcba_id, module_id ; 

#define TEST_RESULT_DIR "c:\\walle_test_results"

void InitTestItems(void);

#define TEST_SW_VERSION "v1.0.0"
#define TEST_SW_TITLE "Active Mind Technology - WallE Test Software "

char *get_test_sw_ver(void)
{
	return TEST_SW_VERSION;	
}


int main (int argc, char *argv[])
{
	int monitorID,height,width;
	char test_title[256];
	
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */

	InitTestItems();
	
	if (FileExists(TEST_RESULT_DIR,0)!=1) 
	{
		MakeDir(TEST_RESULT_DIR);	
	}
	
	FillBytes(g_current_pcba_board_rev,0,255,32) ;
	 g_current_pcba_board_rev[255] = '\0' ;
	 
	LoadOptions();
	
				
	
		
	if (g_need_config_file)
	{
		SaveOptions();
		Save_TestItems();
	}
	

	  if ( (mainPanel = LoadPanel (0, "WallE.uir", MAIN_PANEL)) < 0 )  
		return -1;
	


	LoadWindowOptions();
	
	
	if (g_module_level_test)
	{
		SetCtrlVal(mainPanel,MAIN_PANEL_MODULE_LEVEL,"Module Level Test");
		SetCtrlVal(mainPanel,MAIN_PANEL_BOARD_REV,g_current_module_board_rev);
	}
	else 
	{
		SetCtrlVal(mainPanel,MAIN_PANEL_MODULE_LEVEL,"PCBA Level Test");
		SetCtrlVal(mainPanel,MAIN_PANEL_BOARD_REV,g_current_pcba_board_rev);
	}
	
	
	
	sprintf(test_title,"%s%s",TEST_SW_TITLE,TEST_SW_VERSION);
	SetPanelAttribute (mainPanel, ATTR_TITLE, test_title);
	
	if (g_initial_y_pos>0) 
	{
		GetSystemAttribute (ATTR_FIRST_MONITOR, &monitorID);
		GetMonitorAttribute (monitorID, ATTR_HEIGHT,&height);
        GetMonitorAttribute (monitorID, ATTR_WIDTH, &width);
       	
		if ((g_initial_y_pos<height) && (g_initial_x_pos<width))
		{
			SetPanelAttribute(mainPanel,ATTR_TOP,g_initial_y_pos);
			SetPanelAttribute(mainPanel,ATTR_LEFT,g_initial_x_pos);
		}
	}
	
	DisplayPanel (mainPanel);
	
	SetActiveCtrl(mainPanel,MAIN_PANEL_START_TEST);
	
	RunUserInterface ();
	
	GetPanelAttribute(mainPanel,ATTR_TOP,&g_initial_y_pos);
	GetPanelAttribute(mainPanel,ATTR_LEFT,&g_initial_x_pos);
	
	SaveWindowOptions();
	
	DiscardPanel (mainPanel);
	
	if (g_use_power_supply)
	{
		set_output(0);
		close_power_supply();
	}
	
	if (g_module_board_rev_list) ListDispose(g_module_board_rev_list);
	if (g_pcba_board_rev_list) ListDispose(g_pcba_board_rev_list);
	
	return 0;
}

int CVICALLBACK OnMainPanel (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

void MakeTestLogData(int test_item)
{
	char test_name[64];
	char meas_val[64];
	char low_limit[64];
	char high_limit[64];
	char result[64];

	GetTableCellVal (mainPanel, MAIN_PANEL_RESULT_TABLE, MakePoint(1,test_item), test_name);
	GetTableCellVal (mainPanel, MAIN_PANEL_RESULT_TABLE, MakePoint(2,test_item), meas_val);
	GetTableCellVal (mainPanel, MAIN_PANEL_RESULT_TABLE, MakePoint(3,test_item), low_limit);
	GetTableCellVal (mainPanel, MAIN_PANEL_RESULT_TABLE, MakePoint(4,test_item), high_limit);
	GetTableCellVal (mainPanel, MAIN_PANEL_RESULT_TABLE, MakePoint(5,test_item), result);
	
	AddTestLog(test_name,meas_val,low_limit,high_limit,result);
}

int UpdateTestItem(int test_item,char *test_name,char *meas_val,char *low_limit,char *high_limit,char *result)
{
	int status;
	
	status = 0;
	
	if (test_name) 	status = SetTableCellVal (mainPanel, MAIN_PANEL_RESULT_TABLE, MakePoint(1,test_item), test_name);
	if (meas_val) 	status = SetTableCellVal (mainPanel, MAIN_PANEL_RESULT_TABLE, MakePoint(2,test_item), meas_val);
	if (low_limit) 	status = SetTableCellVal (mainPanel, MAIN_PANEL_RESULT_TABLE, MakePoint(3,test_item), low_limit);
	if (high_limit) status = SetTableCellVal (mainPanel, MAIN_PANEL_RESULT_TABLE, MakePoint(4,test_item), high_limit);
	if (result)
	{
		status = SetTableCellVal (mainPanel, MAIN_PANEL_RESULT_TABLE, MakePoint(5,test_item), result);
		if (strcmp(result,"FAIL")==0)
		{
			g_overall_test_result = FAIL;
		}
		
		MakeTestLogData(test_item);
	}
	

	return status;
}

 


/**************************************************************************************************************************
	int Insert_step(char *test_name, double meas_val, double low_limit, double high_limit, float testtime, char *result) 

	description:  inserting records of individual test steps result into the appropriate table of the database,
				  depending on whether the running test is at module or pcba level.

**************************************************************************************************************************/

int Insert_step(char *test_name, double meas_val, double low_limit, double high_limit, float testtime, char *result) 
{
		int status = 0 ;
	
	char sqlbuffer[500] ; 
	int testresult = 0 ;

	strcpy(sqlbuffer, " "); 
	
	 
	if (strcmp(result, "FAIL") == 0)
	  	testresult = 0 ;
 	else if (strcmp(result, "PASS") == 0) 
	 	testresult = 1;
	
	
	
	if (g_module_level_test)		 
	
		sprintf(sqlbuffer, "INSERT INTO walle.module_step_results"
		  "(module_uut_id, module_step_name,module_meas_value, module_low_limit,module_high_limit, module_step_time, module_step_pass)"
		  "VALUES('%lld','%s','%.6f','%.6f','%.6f','%.3f','%d')",module_id, test_name, meas_val, low_limit,high_limit,testtime, testresult);
		  
	else
		sprintf(sqlbuffer, "INSERT INTO walle.pcba_step_results"
		  "(pcba_uut_id, pcba_step_name,pcba_meas_value, pcba_low_limit,pcba_high_limit, pcba_step_time, pcba_step_pass)"
		  "VALUES('%lld','%s','%.6f','%.6f','%.6f','%.3f','%d')",pcba_id, test_name, meas_val, low_limit,high_limit,testtime, testresult);
		
		 
	SQL_hdl = DBActivateSQL (database_hdl, sqlbuffer); 
		  if (SQL_hdl <= 0)
		  	status = 1 ;
			
		  DBDeactivateSQL(SQL_hdl) ;
	 
	return status;
}   /* Insert_step() */

/**************************************************************************************************************************
	int Insert_step_char(char *test_name, char *meas_val, char *low_limit, char *high_limit, float testtime, char *result)  

	description:  inserting records of individual test steps result into the appropriate table of the database,
				  depending on whether the running test is at module or pcba level. This function works with measurement 
				  values that are not real numbers and with NULL measured values.

**************************************************************************************************************************/

int Insert_step_char(char *test_name, char *meas_val, char *low_limit, char *high_limit, float testtime, char *result)  
{
	char sqlbuffer[500] ;
	
	float fmeas_val, flow_limit, fhigh_limit ;
	int testresult = 0 ;
	int status = 0  ;
	strcpy(sqlbuffer, " "); 
	
	if (strcmp(result, "FAIL") == 0)
		testresult = 0 ;
	else if (strcmp(result, "PASS") == 0) 
		testresult = 1;
	
	if ( (meas_val) &&  (low_limit) &&  (high_limit) )
	{
		 fmeas_val = atof(meas_val) ; 
		 flow_limit = atof(low_limit) ; 
		 fhigh_limit = atof(high_limit) ;  
		 
		if (g_module_level_test)
		{
			sprintf(sqlbuffer, "INSERT INTO walle.module_step_results"
		  	"(module_uut_id, module_step_name,module_meas_value, module_low_limit,module__high_limit, module_step_time, module_step_pass)"
		  	"VALUES('%lld','%s','%.6f','%.6f','%.6f','%.3f','%d')",module_id, test_name, fmeas_val, flow_limit,fhigh_limit,testtime, testresult);
		}
		
		else
		{
			sprintf(sqlbuffer, "INSERT INTO walle.pcba_step_results"
		  "(pcba_uut_id, pcba_step_name,pcba_meas_value, pcba_low_limit,pcba_high_limit, pcba_step_time, pcba_step_pass)"
		  "VALUES('%lld','%s','%.6f','%.6f','%.6f','%.3f','%d')",pcba_id, test_name, fmeas_val, flow_limit,fhigh_limit,testtime, testresult);
			
		}
		  
	}  // if ( values NOT NULL)
	
	else    // (measurements are NULL values)
	{
		if (g_module_level_test)  
		{
			sprintf(sqlbuffer, "INSERT INTO walle.module_step_results"
		  	"(module_uut_id, module_step_name, module_step_time, module_step_pass)"
		  	"VALUES('%lld','%s','%.3f','%d')",module_id, test_name, testtime, testresult);
		}
		else
		{
			 sprintf(sqlbuffer, "INSERT INTO walle.pcba_step_results"
		  "(pcba_uut_id, pcba_step_name, pcba_step_time, pcba_step_pass)"
		  "VALUES('%lld','%s','%.3f','%d')",pcba_id, test_name, testtime, testresult);
		}
		
	}
	
	SQL_hdl = DBActivateSQL (database_hdl, sqlbuffer); 
		  if (SQL_hdl <= 0)
		  	status = 1 ;
			
		  DBDeactivateSQL(SQL_hdl) ;
	return status;
	
}  // Insert_step_char()



int InsertTestItem(char *test_name,char *meas_val,char *low_limit,char *high_limit,char *result)
{
	int status;
	int rows;
		
	status = InsertTableRows (mainPanel, MAIN_PANEL_RESULT_TABLE, -1, 1, VAL_USE_MASTER_CELL_TYPE);
	status = GetNumTableRows (mainPanel, MAIN_PANEL_RESULT_TABLE, &rows);
	
	if (test_name) 	status = SetTableCellVal (mainPanel, MAIN_PANEL_RESULT_TABLE, MakePoint(1,rows), test_name);
	if (meas_val) 	status = SetTableCellVal (mainPanel, MAIN_PANEL_RESULT_TABLE, MakePoint(2,rows), meas_val);
	if (low_limit) 	status = SetTableCellVal (mainPanel, MAIN_PANEL_RESULT_TABLE, MakePoint(3,rows), low_limit);
	if (high_limit) status = SetTableCellVal (mainPanel, MAIN_PANEL_RESULT_TABLE, MakePoint(4,rows), high_limit);
	if (result) status = SetTableCellVal (mainPanel, MAIN_PANEL_RESULT_TABLE, MakePoint(5,rows), result);
	
	status = SetActiveTableCell (mainPanel, MAIN_PANEL_RESULT_TABLE, MakePoint(1,rows));
	
	ProcessDrawEvents();
	
	return rows;
}

void SetStatusLine(char *text)
{
	SetCtrlVal(mainPanel,MAIN_PANEL_STATUS_BAR,text);
}


void DisplayTestResult(void)
{
	if (g_abort_test_flag)
	{
		SetCtrlVal(mainPanel,MAIN_PANEL_TEST_RESULT,"Abort");
	}
	else if (g_overall_test_result==FAIL)
	{
		SetCtrlVal(mainPanel,MAIN_PANEL_TEST_RESULT,"FAIL");
	}
	else if (g_overall_test_result==PASS)
	{
		SetCtrlVal(mainPanel,MAIN_PANEL_TEST_RESULT,"PASS");
	}
	
	
}

// static char sernum[64]={0};

void display_sernum(void)
{
	SetCtrlVal(mainPanel,MAIN_PANEL_SERIAL_NUM,sernum);
}

void display_factory_serial_number(void)
{
	SetCtrlVal(mainPanel,MAIN_PANEL_SERIAL_NUM,factory_serial_number);
}

unsigned long get_chip_id(void)
{
	return strtoul(sernum,0,0);
	
}


char *get_chip_id_string(void)
{
	return sernum;
}

int ShowMessage(char *title,char *message)
{
	int panel;
	panel = LoadPanel(0,"WallE.uir",MSG_PANEL);
	
	SetCtrlVal(panel,MSG_PANEL_MESSAGE,message);
	
	SetPanelAttribute (panel, ATTR_TITLE, title);
	DisplayPanel(panel);
	
	return panel;
}

int ConfirmMessage(char *title,char *message)
{
	int panel;
	int ret_val;
	
	panel = LoadPanel(0,"WallE.uir",CONF_PANEL);
	
	SetCtrlVal(panel,CONF_PANEL_MESSAGE,message);
	
	SetPanelAttribute (panel, ATTR_TITLE, title);

	DisplayPanel(panel);
	
	ret_val = RunUserInterface(); 
	
	DiscardPanel(panel);
	
	return ret_val;
}

int CVICALLBACK testThreadFunction (void *functionData)
{
	int test_item;
	// int panel;
	double elapsedTime;
	int status;
	unsigned long long chipID ;  
	char SQLbuff[500] ;    

	
	if (Load_TestItems())
	{
		SetStatusLine("Error...");
	
		g_abort_test_flag = 1;
		
		DisplayTestResult();
	
		SetCtrlAttribute (mainPanel, MAIN_PANEL_START_TEST, ATTR_DIMMED, FALSE);
			
		SetCtrlAttribute (mainPanel, MAIN_PANEL_STOP_TEST, ATTR_DIMMED, TRUE);	
	
		g_test_running = FALSE;
	
		CmtExitThreadPoolThread (0);
	
		return 0;
	}
	
	g_test_running =TRUE;
	
	if (g_use_power_supply)
	{
		test_item = InsertTestItem("Connecting to Power Supply",NULL,NULL,NULL,"Tryint...");
		
		status = init_power_supply(g_gpib_pwr_addr);
		
		if (status!=0)
		{
			test_item = UpdateTestItem(test_item,"Connecting to Power Supply",NULL,NULL,NULL,"FAIL");

			g_overall_test_result = FAIL;
			
			SetStatusLine("Power Supply Error...");
	
			DisplayTestResult();
	
			SetCtrlAttribute (mainPanel, MAIN_PANEL_START_TEST, ATTR_DIMMED, FALSE);
			
			SetCtrlAttribute (mainPanel, MAIN_PANEL_STOP_TEST, ATTR_DIMMED, TRUE);	
	
			g_test_running = FALSE;
	
			CmtExitThreadPoolThread (0);
			return 0;
		}
		
		test_item = UpdateTestItem(test_item,"Connecting to Power Supply",NULL,NULL,NULL,"PASS");
			
		set_voltage(g_pwr_voltage);
		set_current(g_pwr_current);
		set_output(1);
	}
	
	
	// SQL_hdl = DBActivateSQL (database_hdl, "SET search_path TO walle");
	
	if (g_module_level_test)
		MessagePopup ("SETUP", "Please connect the USB cable to the unit. Click OK when done");
		
		//panel = ShowMessage("Test Starting","Please connect the USB Cable to the device.");
		
	else 
		// panel = ShowMessage("Test Starting","Please connect the Power Supply, and USB Cable to the device.");
		MessagePopup ("SETUP", "Please connect the Power Supply and USB cables to the board. Click OK when done"); 
	
	g_overall_test_result = PASS;
	g_abort_test_flag = 0;
	
	GetCtrlVal(mainPanel,MAIN_PANEL_STOPONFAIL,&g_stop_on_fail);
	//g_stop_on_fail = 0 ;
	
	SetCtrlVal(mainPanel,MAIN_PANEL_TEST_RESULT,"....");
	
	DeleteTableRows (mainPanel, MAIN_PANEL_RESULT_TABLE, 1, -1);
	
	SetStatusLine("Testing...");
	

		
	if (g_module_level_test) 
		display_sernum();
	else 
		display_factory_serial_number();
	
	 
	StrToULongLong (sernum, &chipID);
	
	//InitTestLog(sernum,factory_serial_number, g_module_level_test);
	

	
	do_board_level_test();
	
	close_dev();

	
	elapsedTime = Timer() - g_testStartTime;
	report_meas_test_item("Overall Test Time",elapsedTime,0,600);
	
	CloseTestLog();
		
		//if (g_overall_test_result == FAIL)
		
		/*********************************************************************************************************
		if (g_module_level_test)
		{
			  strcpy(SQLbuff, " ") ;
			  sprintf(SQLbuff, "UPDATE walle.module_uut_results "
			  "SET module_test_time = '%.3f', module_pass = '%d' WHERE id = '%lld'", elapsedTime, g_overall_test_result, module_id) ; 
			 
		  
			  SQL_hdl = DBActivateSQL (database_hdl, SQLbuff); 
		  
			  DBDeactivateSQL(SQL_hdl) ;
  			
				
		}
		else
		{
			 strcpy(SQLbuff, " ") ;
			 sprintf(SQLbuff, "UPDATE walle.pcba_uut_results "
			  "SET pcba_test_time = '%.3f', pcba_pass = '%d' WHERE id = '%lld'", elapsedTime, g_overall_test_result, pcba_id) ;
			 
			 SQL_hdl = DBActivateSQL (database_hdl, SQLbuff);
			
			 
			 DBDeactivateSQL(SQL_hdl) ;
		}   // not g_module_level
		**************************************************************************************************************/
	// }
	
	/*****************************************************************
	else
	{
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		DiscardPanel(panel);
	}
	****************************************************************/
		
	if (g_use_power_supply)
	{
		set_output(0);
	}
	
	SetStatusLine("Done...");
	
	DisplayTestResult();
	
	SetCtrlAttribute (mainPanel, MAIN_PANEL_START_TEST, ATTR_DIMMED, FALSE);
			
	SetCtrlAttribute (mainPanel, MAIN_PANEL_STOP_TEST, ATTR_DIMMED, TRUE);	
	
	g_test_running = FALSE;
	
	CmtExitThreadPoolThread (0);
	
	return 0;
}

int CVICALLBACK OnStartTest (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int status;
	int sernum_panel;
	
	
	switch (event)
	{
		case EVENT_COMMIT:
			if (g_test_running) return 0;
			
			
			SetCtrlVal(mainPanel,MAIN_PANEL_SERIAL_NUM,"");
		
			if (!g_module_level_test)
			{
				sernum_panel = LoadPanel(panel,"WallE.uir",SN_PANEL);
			
				InstallPopup(sernum_panel);
			
				status = RunUserInterface();
			
				DiscardPanel(sernum_panel);
			
				if (status == FAILURE) 
				{
					return -1;
				}
			}

			/************************************************************************************
			database_hdl = DBConnect (database_connector);
			
			if (database_hdl <= 0)
			{
				MessagePopup ("database connection", "Error in connection to database");
				
				SetCtrlAttribute (mainPanel, MAIN_PANEL_START_TEST, ATTR_DIMMED, FALSE);
			
				SetCtrlAttribute (mainPanel, MAIN_PANEL_STOP_TEST, ATTR_DIMMED, TRUE);
			}
			
			else
				//MessagePopup ("database connection", "database connection successful");   
			*********************************************************************************/
			
			g_testStartTime = Timer();
			
			SetCtrlVal(panel,MAIN_PANEL_TEST_TIME,0.0);

			//SetCtrlVal(mainPanel,MAIN_PANEL_SERIAL_NUM,"");
			
			SetCtrlAttribute (panel, control, ATTR_DIMMED, TRUE);
			SetCtrlAttribute (panel, MAIN_PANEL_STOP_TEST, ATTR_DIMMED, FALSE);
			
			status = CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, testThreadFunction, NULL, NULL);
			
			if (status!=0)
			{
				SetCtrlAttribute (mainPanel, MAIN_PANEL_START_TEST, ATTR_DIMMED, FALSE);
			
				SetCtrlAttribute (mainPanel, MAIN_PANEL_STOP_TEST, ATTR_DIMMED, TRUE);
			}
			
			break;
	}
	return 0;
}

int CVICALLBACK OnStopTest (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			g_abort_test_flag = 1;
			
			
			break;
	}
	return 0;
}

/***************************************************************************
int CVICALLBACK OnStopOnFail (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel,control,&g_stop_on_fail);
		break;
	}
//	Delay(5.0) ;
	return 0;
}
*************************************************************************************/

int report_meas_test_item(char *test_name,double dbl_meas_value,double dbl_low_limit,double dbl_high_limit)
{
	char meas_value[64];
	char low_limit[64];
	char high_limit[64];
	int result = FAIL;
	int test_item;
	
	sprintf(meas_value,"%.3f",dbl_meas_value);
	sprintf(low_limit,"%.3f",dbl_low_limit);
	sprintf(high_limit,"%.3f",dbl_high_limit);
	
	if ((dbl_meas_value < dbl_low_limit) || (dbl_meas_value > dbl_high_limit)) result = FAIL;
	else result = PASS;
	
	test_item = InsertTestItem(test_name,NULL,NULL,NULL,NULL);	
	
	UpdateTestItem(test_item,test_name,meas_value,low_limit,high_limit,result?"PASS":"FAIL"); 
	return result;
}

int options_panel;

void populate_board_rev_list(int module_level)
{
	Board_rev_info_type *p_board_info;
	int num_of_items,x;
	
	DeleteListItem(options_panel,OPTIONS_BOARD_REV,0,-1);
	
	if (module_level)
	{
		num_of_items = ListNumItems (g_module_board_rev_list);
	
		for (x=0;x<num_of_items;x++)
		{
			p_board_info = ListGetPtrToItem (g_module_board_rev_list, x+1);
		
			InsertListItem (options_panel, OPTIONS_BOARD_REV,-1, p_board_info->rev_name,p_board_info->rev_file);
			
			if (strcmp(p_board_info->rev_name,g_current_module_board_rev)==0) 
			{
				SetCtrlIndex(options_panel,OPTIONS_BOARD_REV,x);
			}
		}
	}
	else
	{
		num_of_items = ListNumItems (g_pcba_board_rev_list);
	
		for (x=0;x<num_of_items;x++)
		{
			p_board_info = ListGetPtrToItem (g_pcba_board_rev_list, x+1);
		
			InsertListItem (options_panel, OPTIONS_BOARD_REV, -1, p_board_info->rev_name,p_board_info->rev_file);
			
			if (strcmp(p_board_info->rev_name,g_current_pcba_board_rev)==0) 
			{
				SetCtrlIndex(options_panel,OPTIONS_BOARD_REV,x);
			}
		}
	}
}

int CVICALLBACK OnSetupOptions (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
//	Board_rev_info_type *p_board_info;
//  int num_of_items,x;
	
	switch (event)
	{
		case EVENT_COMMIT:
			LoadOptions();
			
			options_panel = LoadPanel(panel,"WallE.uir",OPTIONS);
			
			SetCtrlVal(options_panel,OPTIONS_USE_RS232_PWR,g_use_rs232_power);
			SetCtrlVal(options_panel,OPTIONS_USE_PWR_SPLY,g_use_power_supply);
			SetCtrlVal(options_panel,OPTIONS_LOGFILE_APPEND,g_logfile_append_time);
			SetCtrlVal(options_panel,OPTIONS_CURRENT,g_pwr_current);
			SetCtrlVal(options_panel,OPTIONS_VOLTAGE,g_pwr_voltage);
			SetCtrlVal(options_panel,OPTIONS_PWR_ADDR,g_gpib_pwr_addr);
			//SetCtrlVal(options_panel,OPTIONS_PWR_PORT,g_pwr_serial_port);
			SetCtrlVal(options_panel,OPTIONS_GPS101_PORT,g_gps_101_port);
			SetCtrlVal(options_panel,OPTIONS_STATION_CONFIG,g_module_level_test);
			
			populate_board_rev_list(g_module_level_test);
			
			InstallPopup(options_panel);

			break;
	}
	return 0;
}

int CVICALLBACK OnOptionsPanelCallback (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			
			GetCtrlVal(options_panel,OPTIONS_USE_RS232_PWR,&g_use_rs232_power);
			GetCtrlVal(options_panel,OPTIONS_USE_PWR_SPLY,&g_use_power_supply);
			GetCtrlVal(options_panel,OPTIONS_LOGFILE_APPEND,&g_logfile_append_time);
			GetCtrlVal(options_panel,OPTIONS_CURRENT,&g_pwr_current);
			GetCtrlVal(options_panel,OPTIONS_VOLTAGE,&g_pwr_voltage);
			GetCtrlVal(options_panel,OPTIONS_PWR_ADDR,&g_gpib_pwr_addr);
			//GetCtrlVal(options_panel,OPTIONS_PWR_PORT,&g_pwr_serial_port);
			GetCtrlVal(options_panel,OPTIONS_GPS101_PORT,&g_gps_101_port);
			GetCtrlVal(options_panel,OPTIONS_STATION_CONFIG,&g_module_level_test);
			
			OnBoardRevConfig(options_panel,OPTIONS_BOARD_REV,EVENT_COMMIT,NULL,0,0);
			
			SaveOptions();
			
			if (g_module_level_test)
			{
				SetCtrlVal(mainPanel,MAIN_PANEL_MODULE_LEVEL,"Module Level Test");
				SetCtrlVal(mainPanel,MAIN_PANEL_BOARD_REV,g_current_module_board_rev);
			}
			else 
			{
				SetCtrlVal(mainPanel,MAIN_PANEL_MODULE_LEVEL,"PCBA Level Test");
				SetCtrlVal(mainPanel,MAIN_PANEL_BOARD_REV,g_current_pcba_board_rev);
			}
			
			RemovePopup(0);
			DiscardPanel(panel);

			break;
	}
	return 0;
}


int CVICALLBACK OnNfcPanel (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			  RemovePopup(0);
			  g_abort_test_flag = TRUE;
			break;
	}
	return 0;
}

int CVICALLBACK OnTimer (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double elapsedTime;


	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			if (g_test_running)
			{
				elapsedTime = Timer() - g_testStartTime;
				
				SetCtrlVal(panel,MAIN_PANEL_TEST_TIME,elapsedTime);
			}
			else
			{


			}

			break;
	}
	return 0;
}


int CVICALLBACK OnStationConfig (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int val;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel,control,&val);
			if (val)
			{
				SetCtrlVal(panel,OPTIONS_USE_PWR_SPLY,0);
			}
			else
			{
				SetCtrlVal(panel,OPTIONS_USE_PWR_SPLY,1);
			}
			
			populate_board_rev_list(val);

			break;
	}
	return 0;
}

int CVICALLBACK OnSerialNumOK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int x;
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal(panel,SN_PANEL_SERIAL_NUM,factory_serial_number);
			
			if (strlen(factory_serial_number)==0)
			{
				//MessagePopup("Missing Factory Serial Number","Please SCAN or TYPE IN factory serial number!\n");
				SetActiveCtrl(panel,SN_PANEL_SERIAL_NUM);
				
				for (x=0;x<3;x++)
				{
					SetCtrlAttribute(panel,SN_PANEL_WARNING,ATTR_VISIBLE,0);
					ProcessDrawEvents();
					Delay(0.3);
					SetCtrlAttribute(panel,SN_PANEL_WARNING,ATTR_VISIBLE,1);
					ProcessDrawEvents();
					Delay(0.5);
				}
				return 0;
			}
			else
			{
				RemovePopup(0);
				QuitUserInterface(SUCCESS);
			}

			break;
	}
	return 0;
}

int CVICALLBACK OnSnPanel (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			RemovePopup(0);
			QuitUserInterface(FAILURE);

			break;
	}
	return 0;
}

int CVICALLBACK OnSerialNumber (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_KEYPRESS:
			if (eventData1==VAL_ENTER_VKEY)
			{
				OnSerialNumOK (panel,control,EVENT_COMMIT,0,0,0);
			}
			else if (eventData1==VAL_ESC_VKEY)
			{
				OnSnPanel(panel,EVENT_CLOSE,0,0,0);
			}
			
			break;
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK OnMessageOK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK OnMessagePanel (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK OnConfirmYes (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface(YES); 

			break;
	}
	return 0;
}

int CVICALLBACK OnConfirmNo (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface(NO); 

			break;
	}
	return 0;
}

int CVICALLBACK OnConfirmPanel (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(NO); 
			break;
	}
	return 0;
}

int CVICALLBACK OnBoardRevConfig (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int index;
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlIndex(panel,control,&index);
			
			if (g_module_level_test) 
			{
				GetValueFromIndex (panel, control, index, g_current_module_board_file);
				GetLabelFromIndex (panel, control, index, g_current_module_board_rev);
			}
			else
			{
				GetValueFromIndex (panel, control, index, g_current_pcba_board_file);
				GetLabelFromIndex (panel, control, index, g_current_pcba_board_rev);
			}

			break;
	}
	return 0;
}
