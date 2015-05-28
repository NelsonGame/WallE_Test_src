#include <utility.h>
#include <userint.h>
#include "ansi_c.h"
#include "TestCommon.h"
#include "PmicTest.h"
#include "DevComm.h"

 #define TEST_PMIC_I2C "I2CRD,7E,1C\r\n"

//void LeftTrim(char *string);


void LeftTrim(char *string)
{
    char *ptr = string;
    
    while(isspace(*string))
    {
        string++;
    }
    
    if (ptr!=string)
    {
        // copy the characters if there is left space
        while(*string)
        {
            *ptr=*string;
            string++;
            ptr++;
        }
        // null terminate string
        *ptr = 0;
    }
}




int test_pmic_i2c(void)
{
	int ret_val;
	int test_item;
	char buffer[256];
	char *ptr;
	double begin, end ;
	float testtime ;
	unsigned long i2c_res ;
	begin = Timer() ;
	
	test_item = InsertTestItem("PMIC I2C Test",NULL,NULL,NULL,"Testing...");
	
	ret_val = send_dev_cmd(TEST_PMIC_I2C);
	
	if (ret_val == FAILURE)
	{
		end = Timer(); 
		testtime = end - begin ;
		Insert_step_char("PMIC I2C Test", NULL, NULL, NULL, testtime, "FAIL") ;  
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	ret_val = get_dev_response(buffer);
	
	if ( (ret_val == FAILURE) || (strstr(buffer,"\r\n>")==NULL) )
	{
		end = Timer();
		testtime = end - begin ;
		Insert_step_char("PMIC I2C Test", NULL, NULL, NULL, testtime, "FAIL") ; 
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}

	// skip the first line
	ptr = strstr(buffer,"=");
	if (ptr==NULL)
	{
		end = Timer();
		testtime = end - begin ;
		Insert_step_char("PMIC I2C Test", NULL, NULL, NULL, testtime, "FAIL") ; 
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	ptr++;
	LeftTrim(ptr);
	i2c_res = strtoul(ptr,NULL,0) ;
	// if (strtoul(ptr,NULL,0)==0)
	if ( i2c_res == 0)
	{
		end = Timer();
		testtime = end - begin ;
		Insert_step_char("PMIC I2C Test", NULL, NULL, NULL, testtime, "FAIL") ; 
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	end = Timer();
	testtime = end - begin ;
	Insert_step_char("PMIC I2C Test", NULL, NULL, NULL, testtime, "PASS") ; 
	UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"PASS");
	
	return SUCCESS;
}
