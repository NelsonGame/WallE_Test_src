#include <utility.h>
#include <userint.h>
#include "ansi_c.h"
#include "TestCommon.h"
#include "VersionTest.h"
#include "DevComm.h"
//#include "ActiveMindTest.h"
#include "WallE.h"

#define TEST_POWER_BUTTON "POWEROFF\r\n"
#define TEST_RESET_BUTTON "RESET_BUTTON\r\n"


int ShowMessage(char *title,char *message);


int power_button_test(void)
{
	int ret_val=0;
	int test_item;
	char buffer[1024];
	//int old_timeout;
	int panel;
	double begin, end ;
	float testtime ;
	
	begin = Timer() ;
	
	//old_timeout = set_dev_timeout(30000);
	
	memset(buffer,0,sizeof(buffer));
	
	test_item = InsertTestItem("POWER Button Test",NULL,NULL,NULL,NULL);
	
	ret_val = send_dev_cmd(TEST_POWER_BUTTON);

	if (ret_val == FAILURE)
	{
		end = Timer();
		testtime = end - begin;
		
		//Insert_step_char("POWER Button Test", NULL, NULL, NULL, testtime, "FAIL") ;  
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	Delay(1.0);

	panel = ShowMessage("Power Button Test","PRESS the POWER Button ON the board!");
	
	ret_val = get_dev_response(buffer);
	
	if (g_abort_test_flag)
	{
		DiscardPanel(panel);
		return FAILURE;
	}
	
	if ((ret_val == FAILURE)||(strstr(buffer,"AMT>")==NULL))
	{
		end = Timer();
		testtime = end - begin; 
		// Insert_step_char("POWER Button Test", NULL, NULL, NULL, testtime, "FAIL") ; 
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		DiscardPanel(panel);
		return FAILURE;
	}
	
	end = Timer();
	testtime = end - begin ;
	// Insert_step_char("POWER Button Test", NULL, NULL, NULL, testtime, "PASS") ;  
	UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"PASS");
	
	//set_dev_timeout(old_timeout);
	
	DiscardPanel(panel);
	return SUCCESS;
}

int reset_button_test(void)
{
	int ret_val=0;
	int test_item;
	char buffer[1024];
	// int old_timeout;
	int panel;
	double begin, end ;
	float testtime ;
	
	begin = Timer() ;
	
	//old_timeout = set_dev_timeout(30000);
	
	memset(buffer,0,sizeof(buffer));
	
	test_item = InsertTestItem("RESET Button Test",NULL,NULL,NULL,NULL);
	
	panel = ShowMessage("RESET Button Test","Now PRESS the RESET Button on the board!");
	
	ret_val = get_dev_response(buffer);
	
	if (g_abort_test_flag)
	{
		
		DiscardPanel(panel);
		return FAILURE;
	}
	
	if ((ret_val == FAILURE)||(strstr(buffer,"AMT>")==NULL))
	{
		end = Timer();
		testtime = end - begin ; 
		// Insert_step_char("RESET Button Test", NULL, NULL, NULL, testtime, "FAIL") ; 
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		DiscardPanel(panel);
		return FAILURE;
	}
	
	end = Timer();
	testtime = end - begin;
	// Insert_step_char("RESET Button Test", NULL, NULL, NULL, testtime, "PASS") ; 
	UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"PASS");
	
	//set_dev_timeout(old_timeout);

	DiscardPanel(panel);
	
	Delay(1.0);

	return SUCCESS;
}
