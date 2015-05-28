//#include "ActiveMindTest.h"
#include "WallE.h"
#include "toolbox.h"
#include <utility.h>
#include <userint.h>
#include "ansi_c.h"
#include "TestCommon.h"
#include "VersionTest.h"
#include "DevComm.h"
#include "Testfirmware.h"
#include "TestItems.h"
#include "AmtFwLoader.h"

#define TEST_VERSION "VERSION\r\n"

int ShowMessage(char *title,char *message);

int program_test_app(void)
{
	int ret_val=FAILURE;
	int test_item;
	char buffer[1024];
	//char *version;
	//char *ptr;
	//int tries;
	char firmware_file[MAX_PATHNAME_LEN];
	char sernum[256];
	int FileSize;
	int panel;
	
	memset(buffer,0,sizeof(buffer));
	
	test_item = InsertTestItem("Test App Firmware Load Test",NULL,NULL,NULL,NULL);
	
	/******************************************************************************
	if (TestItems.PROGRAM_BOOTLOADER==FALSE)
	{
		panel = ShowMessage("App Load Mode!","Press the TOP button until RED and WHITE LED are Solid ON!");
		SetCtrlAttribute (panel, MSG_PANEL_OK, ATTR_VISIBLE, 1);
		
		close_dev();
		Delay(2.0);
		
		RunUserInterface();
		RemovePopup(0);
		DiscardPanel(panel);
	}
	**********************************************************************************/
	
// #if 1	
	
	GetProjectDir(firmware_file);
	strcat(firmware_file,"\\");
	strcat(firmware_file,TestItems.TestFwFile);
	ret_val = FileExists(firmware_file,&FileSize);
	
	if (ret_val!=1)	
	{
	
		UpdateTestItem(test_item,"Test firmware file is missing!",NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	ret_val = amt_load_firmware(get_chip_id_string(),firmware_file);
//#endif
	
	if (ret_val == FAILURE)
	{
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}

	Delay(2.0);
	 /***********************************************************************
	if (DetectUsbDevice(sernum)==SUCCESS)
	{
		if (init_dev()==SUCCESS)
		{
			UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"PASS");
			return SUCCESS;
		}
	}
	*********************************************************************/
	
	UpdateTestItem(test_item,"unable to open serial",NULL,NULL,NULL,"FAIL");
	return FAILURE;
	
}


int load_production_fw(void)
{
	int ret_val=FAILURE;
	int test_item;
	char buffer[1024];
	//char *version;
	//char *ptr;
	//int tries;
	char firmware_file[MAX_PATHNAME_LEN];
	char sernum[256];
	int FileSize;
	//int panel;
	char err_msg[1024];
	
	memset(buffer,0,sizeof(buffer));
	
	test_item = InsertTestItem("Production Firmware Load Test",NULL,NULL,NULL,NULL);
	
	send_dev_cmd("RESET\r\n");
	
	Delay(3.0);
	
	get_dev_response(buffer);
	
	if (strstr(buffer,"AMT>")==NULL)
	{
		UpdateTestItem(test_item,"Device RESET Failed!",NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	send_dev_cmd("LOADFW\r\n");
	Delay(1.0);
	close_dev();
	Delay(1.0);
	
#if 1	
	
	GetProjectDir(firmware_file);
	strcat(firmware_file,"\\");
	strcat(firmware_file,TestItems.ProductionFwFile);
	ret_val = FileExists(firmware_file,&FileSize);
	
	if (ret_val!=1)	
	{
		sprintf("Production firmware file is missing! %s",firmware_file);
		UpdateTestItem(test_item,err_msg,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	ret_val = amt_load_firmware(get_chip_id_string(),firmware_file);
#endif
	
	if (ret_val == FAILURE)
	{
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}

	Delay(2.0);
	
	/***************************************************************
	if (DetectUsbDevice(sernum)==SUCCESS)
	{
		if (init_dev()==SUCCESS)
		{
			UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"PASS");
			return SUCCESS;
		}
	}
	**********************************************************************/
	
	UpdateTestItem(test_item,"unable to open serial",NULL,NULL,NULL,"FAIL");
	return FAILURE;
}


