#include <utility.h>
#include "ansi_c.h"
#include "TestCommon.h"
#include "CompassTest.h"
#include "DevComm.h"
#include "TestItems.h"

#define TEST_COMPASS_I2C "COMPASS_I2C_TEST\r\n"
#define TEST_COMPASS "COMPASS\r\n"

int test_compass_I2C(void)
{
	int ret_val;
	int test_item;
	char buffer[256];
	char *ptr;
	
	test_item = InsertTestItem("Compass I2C Test",NULL,NULL,NULL,"Testing...");
	
	ret_val = send_dev_cmd(TEST_COMPASS_I2C);
	
	if (ret_val == FAILURE)
	{
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	ret_val = get_dev_response(buffer);
	
	if ( (ret_val == FAILURE) || (strstr(buffer,">")==NULL) )
	{
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	// skip the first line
	ptr = strstr(buffer,"\r\n");
	if (ptr==NULL)
	{
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	ptr+=2;
	if (atoi(ptr)==0)
	{
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"PASS");
	
	return SUCCESS;
}

int test_compass(void)
{
	int ret_val;
	int test_item;
	char buffer[256]={0};
	char *ptr;
	double x,y,z;
	//int result = PASS;
	int result_x,result_y,result_z;
	int tries;
	double begin, end ;
	float testtime =0.0 ;
	
	begin = Timer() ;
	
	test_item = InsertTestItem("Compass Test",NULL,NULL,NULL,"Testing...");
	
	for (tries=0;tries<7;tries++)
	{
		ret_val = send_dev_cmd(TEST_COMPASS);
	
		if (ret_val == FAILURE)
		{
			end = Timer(); 
			testtime = end - begin ;
			Insert_step_char("Compass Test", NULL, NULL, NULL, testtime, "FAIL") ;
			
			UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
			return FAILURE;
		}
	
		ret_val = get_dev_response(buffer);
	
		if ( (ret_val == FAILURE) || (strstr(buffer,">")==NULL) ) 
		{
			end = Timer(); 
			testtime = end - begin ;
			Insert_step_char("Compass Test", NULL, NULL, NULL, testtime, "FAIL") ;
			
			UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
			return FAILURE;
		}

		if ((tries>2) && (strstr(buffer,"0.000")==NULL))
		{
			break;
		}

	}
	
	// skip the first line
	ptr = strstr(buffer,"X=");
	if (ptr==NULL)
	{
		end = Timer(); 
		testtime = end - begin ;
		//Insert_step_char("Compass Test", NULL, NULL, NULL, testtime, "FAIL") ;
		
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	ptr+=2;
	x = atof(ptr);
	
	ptr = strstr(buffer,"Y=");
	if (ptr==NULL)
	{
		end = Timer(); 
		testtime = end - begin ;
		//Insert_step_char("Compass Test", NULL, NULL, NULL, testtime, "FAIL") ;
			
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	ptr+=2;
	y = atof(ptr);
	
	ptr = strstr(buffer,"Z=");
	if (ptr==NULL)
	{
		end = Timer(); 
		testtime = end - begin ;
		Insert_step_char("Compass Test", NULL, NULL, NULL, testtime, "FAIL") ;
		
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	ptr+=2;
	z = atof(ptr);
	
	if (tries==7)
	{
		result_x = report_meas_test_item("Compass X value",x, 0.0001,-0.0001);
		result_y = report_meas_test_item("Compass Y value",y, 0.0001,-0.0001); 
		result_z = report_meas_test_item("Compass Z value",z, 0.0001,-0.0001);
	}
	else
	{
		result_x = report_meas_test_item("Compass X value",x, test_limits.compass.x.low,test_limits.compass.x.high);
		if (!result_x)
			Insert_step("Compass Test X-axis",x ,test_limits.compass.x.low , test_limits.compass.x.high, testtime, "FAIL") ; 
		else
			Insert_step("Compass Test X-axis",x ,test_limits.compass.x.low , test_limits.compass.x.high, testtime, "PASS") ;  
		
		result_y = report_meas_test_item("Compass Y value",y, test_limits.compass.y.low,test_limits.compass.y.high); 
		if (!result_y)
			Insert_step("Compass Test Y-axis",y ,test_limits.compass.y.low , test_limits.compass.y.high, testtime, "FAIL") ; 
		else
			Insert_step("Compass Test Y-axis",y ,test_limits.compass.y.low , test_limits.compass.y.high, testtime, "PASS") ; 
		
		result_z = report_meas_test_item("Compass Z value",z, test_limits.compass.z.low,test_limits.compass.z.high);
		if (!result_z)
			Insert_step("Compass Test Z-axis",z ,test_limits.compass.z.low , test_limits.compass.z.high, testtime, "FAIL") ; 
		else
			Insert_step("Compass Test Z-axis",z ,test_limits.compass.z.low , test_limits.compass.z.high, testtime, "PASS") ; 
	}
	
	if ( !result_x | !result_y | !result_z)
	{
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"PASS");
	
	return SUCCESS;
}
