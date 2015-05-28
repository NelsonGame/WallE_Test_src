#include <utility.h>
#include "ansi_c.h"
#include "TestCommon.h"
#include "NandTest.h"
#include "DevComm.h"

#define TEST_NAND_QUICK "QUICK_NAND_TEST\r\n"

int test_nand_quick(void)
{
	int ret_val;
	int test_item;
	char buffer[256];
	char *ptr;
	double begin, end ;
	float testtime ;
	
	begin = Timer() ;
	
	test_item = InsertTestItem("Quick NAND Test",NULL,NULL,NULL,"Testing...");
	
	ret_val = send_dev_cmd(TEST_NAND_QUICK);
	
	if (ret_val == FAILURE)
	{
		end = Timer(); 
		testtime = end - begin ;
		Insert_step_char("Quick NAND Test", NULL, NULL, NULL, testtime, "FAIL") ; 
		
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	ret_val = get_dev_response(buffer);
	
	if ( (ret_val == FAILURE) || (strstr(buffer,">")==NULL) ) 
	{
		end = Timer(); 
		testtime = end - begin ;
		Insert_step_char("Quick NAND Test", NULL, NULL, NULL, testtime, "FAIL") ; 
		
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
		
	// skip the first line
	ptr = strstr(buffer,"\r\n");
	if (ptr==NULL)
	{
		end = Timer(); 
		testtime = end - begin ;
		Insert_step_char("Quick NAND Test", NULL, NULL, NULL, testtime, "FAIL") ; 
		
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	ptr+=2;
	if (atoi(ptr)==0)
	{
		end = Timer(); 
		testtime = end - begin ;
		Insert_step_char("Quick NAND Test", NULL, NULL, NULL, testtime, "FAIL") ; 
		
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	end = Timer(); 
	testtime = end - begin ;
	Insert_step_char("Quick NAND Test", NULL, NULL, NULL, testtime, "PASS") ;
	
	UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"PASS");
	
	return SUCCESS;
}
