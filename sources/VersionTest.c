#include <formatio.h>
#include "toolbox.h"
#include <rs232.h>
#include <utility.h>
#include <userint.h>
#include "ansi_c.h"
//#include "ActiveMindTest.h" 
#include "WallE.h"
#include "TestCommon.h"
#include "VersionTest.h"
#include "DevComm.h"
#include "TestItems.h"
#include "TestLog.h"

#define TEST_FWVERSION "$AMT,VERSION*2E\r\n"
#define TEST_HW_VERSION "$AMT,HWVERSION*31\r\n"
#define TEST_DEVICE_SERNUM "$AMT,CHIPID*6B\r\n"

#define NUM_OF_BLANKLINES 3

FILE *dev_log_file;
char filename[MAX_PATHNAME_LEN]; 

char *device_sernum= NULL ; 
extern int g_stop_on_fail;
extern int g_dev_serial_port; 
extern int g_module_level_test ;
extern char factory_serial_number[] ;
extern double dev_log_start_timer ;
//extern static char sernum[32] ; 
extern int mainPanel; 
int test_version(void)
{
	int ret_val=0;
	int test_item;
	char buffer[50] = {" "} ;
	//char *version = NULL; 
	//char *verstr = NULL;
	//char *ptr = NULL;
	char fw_version[15] = {" "} ;
	//char dev_serial[15] = {" "}; 
	//char hwID_str[15] = {" "};
	// int hw_ID = -9;
	int tries, index, i;
	double begin, end ;
	float testtime ;
	
	begin = Timer() ;
	
	memset(buffer,0,sizeof(buffer));
	
	test_item = InsertTestItem("Firmware Version Check",NULL,NULL,NULL,NULL);
	
	/******************************************************************************************************
	for (tries=0;tries<3;tries++)
	{
		ret_val = send_dev_cmd(TEST_FWVERSION);
	
		if (ret_val == FAILURE)
		{
			end = Timer() ;
			testtime = (float) (end - begin) ; 
			Insert_step_char("Firmware Version Test", NULL, NULL, NULL, testtime, "FAIL") ;
			UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
			
			if (!g_stop_on_fail) MessagePopup("Communication Error!", "Lost Communication with Device!\nCan't continue!");
			return FAILURE;
		}
	
		ret_val = get_dev_response(buffer);
		
		if ((ret_val == SUCCESS)&&(strstr(buffer,">")!=NULL)&&(strstr(buffer,"invalid")==NULL)) break;
		
		if (g_abort_test_flag) 
		{
			end = Timer() ;
			testtime = (float) (end - begin) ; 
			Insert_step_char("Firmware Version Test", NULL, NULL, NULL, testtime, "FAIL") ;
			
			
			return FAILURE;
		}
	}  // for (tries)

	if ((ret_val == FAILURE)||(strstr(buffer,">")==NULL))
	{
		end = Timer() ;
		testtime = (float) (end - begin) ; 
		
		Insert_step_char("Firmware Version Test", NULL, NULL, NULL, testtime, "FAIL") ;
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		
		if (!g_stop_on_fail) MessagePopup("Communication Error!", "Lost Communication with Device!\nCan't continue!");
		return FAILURE;
	}
	
	// skip the first line
	ptr = strstr(buffer,"\r\n");
	if (ptr==NULL)
	{
		end = Timer() ;
		testtime = (float) (end - begin) ;
		Insert_step_char("Firmware Version Test", NULL, NULL, NULL, testtime, "FAIL") ;   
		
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		if (!g_stop_on_fail) MessagePopup("Communication Error!", "Lost Communication with Device!\nCan't continue!");
		return FAILURE;
	}
	
	ptr+=2;
	if (atoi(ptr)==0)
	{
		 end = Timer() ;
		testtime = (float) (end - begin) ;
		Insert_step_char("Firmware Version Test", NULL, NULL, NULL, testtime, "FAIL") ;  
		  
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		if (!g_stop_on_fail) MessagePopup("Communication Error!", "Lost Communication with Device!\nCan't continue!");
		return FAILURE;
	}
	
	version = strtok(ptr,"\r\n");
	 verstr = malloc(30 * sizeof (char)) ;
	 sprintf(verstr, " ") ;
	if (strcmp(version,test_limits.fw_version)==0)
	{
		end = Timer() ;
		testtime = (float) (end - begin) ;
		sprintf (verstr, " ") ;
		sprintf(verstr, "Firmware Version: %s", version) ;
	 
		Insert_step_char(verstr, NULL, NULL, NULL, testtime, "PASS") ; 
		 	
		UpdateTestItem(test_item,NULL,version,test_limits.fw_version,test_limits.fw_version,"PASS");
	}
	else
	{
		end = Timer() ;
		testtime = (float) (end - begin) ;
		Insert_step_char("Firmware Version Test", NULL, NULL, NULL, testtime, "FAIL") ; 
		UpdateTestItem(test_item,NULL,version,test_limits.fw_version,test_limits.fw_version,"FAIL");
	}
	
	*****************************************************************************************************************/
	
	send_dev_cmd(TEST_FWVERSION); 
	for ( i = 0 ; i < NUM_OF_BLANKLINES ; i++)
		ComRdTerm(g_dev_serial_port, buffer, 1, 10);
	
	FillBytes(buffer, 0, 49,32) ;
	buffer[49] =  '\0'; 
	ComRdTerm(g_dev_serial_port, buffer, 32, 10);
	
	index = FindPattern (buffer, 0, -1, "Test firmware version", 0, 0);
	CopyString(fw_version, 0, buffer, index + 22 , 10) ;
	RemoveSurroundingWhiteSpace(fw_version) ;
	
	if ( StrICmpWithoutSurrWhiteSpace (fw_version, test_limits.fw_version) == 0 )
	
		UpdateTestItem(test_item,"Firmware Version Check", fw_version, test_limits.fw_version, test_limits.fw_version, "PASS");
	else
		UpdateTestItem(test_item,"Firmware Version Check", fw_version, test_limits.fw_version, test_limits.fw_version, "FAIL"); 
	
	return SUCCESS;
}


int test_hw_version(void)
{
	int ret_val= -99;
	int test_item;
	char empty[2]={" "} ;
	int version ; 
	char hwID_str[5] = {" "};
	char chwID_limit[5] = {" "} ; 
	int ihwID_limit ;  
	int hw_ID = -9 ;
	//char *ptr = NULL;
	//char *verstr = NULL;
	//int tries;
	double begin, end ;
	float testtime ;
	
	FillBytes(hwID_str,0,4, 32);
	hwID_str[4] = '\0' ;
	FillBytes(chwID_limit,0, 4,32) ;
	chwID_limit[4] = '\0' ;
	
	FlushInQ(g_dev_serial_port) ;
	FlushOutQ(g_dev_serial_port) ;
	
	begin = Timer() ;
	
	//memset(buffer,0,sizeof(buffer));
	
	test_item = InsertTestItem("HW Version Check",NULL,NULL,NULL,NULL);
	 
	
	/****************************************************************************************************
	//for (tries=0;tries<3;tries++)
	while (ret_val < 0)
	{
		ret_val = send_dev_cmd(TEST_HW_VERSION);
	
		if (ret_val == FAILURE)
		{
			end = Timer() ;
			testtime = (float) (end - begin) ; 
			Insert_step_char("HW Version Test", NULL, NULL, NULL, testtime, "FAIL") ;
			
			UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
			if (!g_stop_on_fail) MessagePopup("Communication Error!", "Lost Communication with Device!\nCan't continue!");
			return FAILURE;
		}
	
	
		 ret_val = ComRdByte(g_dev_serial_port) ;
		 
		
		if (g_abort_test_flag) return FAILURE;
	}  // while not success


	version = ret_val ;
	 verstr = malloc(30 * sizeof (char)) ; 
	 sprintf(verstr, " ") ;
	sprintf(buffer,"%d",test_limits.hw_version);
	
	if (version!=test_limits.hw_version)
	{
		end = Timer() ;
		testtime = (float) (end - begin) ; 
		// Insert_step_char("HW Version Test", NULL, NULL, NULL, testtime, "FAIL") ;
			
		UpdateTestItem(test_item,NULL,version,buffer,buffer,"FAIL");
	}
	
	else 
	{
		end = Timer() ;
		testtime = (float) (end - begin) ; 
		sprintf(verstr, " HW Version: %s ", version) ;
		// Insert_step_char(verstr, NULL, NULL, NULL, testtime, "PASS") ;
			
		UpdateTestItem(test_item,NULL,version,buffer,buffer,"PASS");
	}
	 ****************************************************************************************/
	send_dev_cmd(TEST_HW_VERSION); 
	//ComRdTerm(g_dev_serial_port, empty, 1, 10); 
	ComRdTerm (g_dev_serial_port, hwID_str, 2, 10);
	RemoveSurroundingWhiteSpace(hwID_str) ; 
	Scan(hwID_str, "%s>%d", &hw_ID) ; 
	
	ihwID_limit = (int)test_limits.hw_version ;
	sprintf(chwID_limit,"%d",test_limits.hw_version);  
	
	if (hw_ID == ihwID_limit) 
		UpdateTestItem(test_item,"HW Version Check", hwID_str, chwID_limit, chwID_limit, "PASS"); 
	else
		UpdateTestItem(test_item,"HW Version Check", hwID_str, chwID_limit, chwID_limit, "FAIL"); 
	
	return SUCCESS;
}


int read_dev_serial(void)
{
    char buffer[30] ;
	// char *device_sernum= NULL, *sernum_ptr = NULL;
	char empty[2] = {" "} ;
	double begin, end ;
	float testtime ;
	int stat = 0 ;
	int byte_in_queue = 0 ;
	// char buffer[15] ;
	
	FlushInQ(g_dev_serial_port) ;
	FlushOutQ(g_dev_serial_port) ;
	
	FillBytes(buffer, 0, 29, 32) ;
	buffer[29] = '\0' ;
	//device_sernum = malloc( 20*sizeof(char) ); 
	//sernum_ptr = &device_sernum ; 
	begin = Timer() ;
	
	send_dev_cmd(TEST_DEVICE_SERNUM) ;
	//ComRdTerm (g_dev_serial_port, empty,1, 10);
	//ComRdTerm (g_dev_serial_port, sernum,30, 10); 
	ComRdTerm (g_dev_serial_port, buffer,25, 10); 
    /********************************************************	
	byte_in_queue = ComRdByte(g_dev_serial_port) ;
	if ( (byte_in_queue == CR) || (byte_in_queue == LF) )
		*device_sernum = 32 ;
		
	else 
		*device_sernum = (char) byte_in_queue ;
	*********************************************************/	
	/***********************************************************
	while (byte_in_queue >= 0)
	{
		byte_in_queue = ComRdByte(g_dev_serial_port) ;  
		if ( (byte_in_queue == CR) || (byte_in_queue == LF) )
			*device_sernum = 32 ;
		else 
			*device_sernum = (char) byte_in_queue ;
		device_sernum++ ;
	}	  // for all chars in COM buffer
	//*sernum_ptr = '\0' ;
	********************************************************/
	device_sernum = StrDupWithoutSurrWhiteSpace(buffer) ;
	SetCtrlVal(mainPanel,MAIN_PANEL_SERIAL_NUM,device_sernum); 
	InsertTestItem("Chip ID",device_sernum,NULL,NULL,"PASS");   
	stat = InitTestLog(device_sernum,factory_serial_number, g_module_level_test) ;  
	sprintf(filename,"c:\\walle_test_results\\dev_log_%s_%s_%s",device_sernum,getTime(),getDate());
	dev_log_file = fopen(filename,"a");
	dev_log_start_timer = Timer();
	
	//if (stat == FAILURE)
	//	MessagePopup("ERROR", "Failure in InitTestLog") ;
					 
	return SUCCESS ;
}
