#include "toolbox.h"
#include <utility.h>
#include <userint.h>
#include "ansi_c.h"
#include "TestCommon.h"
#include "VersionTest.h"
#include "DevComm.h"
#include "Bootloader.h"
#include "PowerSupply.h"
#include "TestFlashMagic.h"
#include "DevComm.h"
#include "TestItems.h"

#define TEST_VERSION "VERSION\r\n"

extern int com_port_num;

/***********************************************************************************************
int program_bootloader(void)
{
	int ret_val;
	int test_item;
	char buffer[1024];
	//char *version;
	//char *ptr;
	//int tries;
	char errMsg[1024];
	int comport=28;
	//char sernum[64];
	int FileSize;
	
	comport = com_port_num;
	
	memset(buffer,0,sizeof(buffer));
	
	test_item = InsertTestItem("Program BootLoader",NULL,NULL,NULL,NULL);

	ps_enable_bootloader(FALSE);
	
	close_dev();
	
	Delay(0.5);
	
	ret_val = FileExists(TestItems.Bootloader_hexfile,&FileSize);
	if (ret_val!=1)	
	{
	
		UpdateTestItem(test_item,"Bootloader file is missing!",NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	ret_val = flash_magic_program(comport,TestItems.Bootloader_hexfile,errMsg);
	
	
	//set_output(OUTPUT_OFF);
	//Delay(0.5);
	//set_output(OUTPUT_ON);

//	Delay(0.5);
	
//	DetectUsbDevice(sernum);
	
//	init_dev();
	
	Delay(5.0);
	
	if (ret_val)	
	{
	
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}

	
	
	UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"PASS");
	
	
	
	return SUCCESS;
}   // boot loader loading
*********************************************************************************************/
