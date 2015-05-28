#include <utility.h>
#include "ansi_c.h"
#include "TestCommon.h"
#include "AccelTest.h"
#include "DevComm.h"
#include "TestItems.h"

#define TEST_ACCEL_I2C "ACCEL_I2C_TEST\r\n"
#define TEST_ACCEL "ACCEL\r\n"

int test_accel_I2C(void)
{
	int ret_val;
	int test_item;
	char buffer[256];
	char *ptr = NULL;
	double begin, end ;
	float testtime ;
	
	begin = Timer() ; 
	test_item = InsertTestItem("Accelerometer I2C Test",NULL,NULL,NULL,"Testing...");
	
	
	ret_val = send_dev_cmd(TEST_ACCEL_I2C);
	
	if (ret_val == FAILURE)
	{
		end = Timer(); 
		testtime = end - begin ;
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		Insert_step_char("Accel I2C Test", NULL, NULL, NULL, testtime, "FAIL") ; 
		return FAILURE;
	}
	
	ret_val = get_dev_response(buffer);
	
	if ( (ret_val == FAILURE) || (strstr(buffer,"\r\n>")==NULL) )
	{
		end = Timer(); 
		testtime = end - begin ;
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		Insert_step_char("Accel I2C Test", NULL, NULL, NULL, testtime, "FAIL") ; 
		return FAILURE;
	}

	// skip the first line
	ptr = strstr(buffer,"\r\n");
	if (ptr==NULL)
	{
		end = Timer(); 
		testtime = end - begin ;
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		Insert_step_char("Accel I2C Test", NULL, NULL, NULL, testtime, "FAIL") ; 
		return FAILURE;
	}
	
	ptr+=2;
	if (atoi(ptr)==0)
	{
		end = Timer(); 
		testtime = end - begin ;
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		Insert_step_char("Accel I2C Test", NULL, NULL, NULL, testtime, "FAIL") ; 
		return FAILURE;
	}
	
	end = Timer(); 
	testtime = end - begin ;
	UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"PASS");
	
	Insert_step_char("Accel I2C Test", NULL, NULL, NULL, testtime, "PASS") ;
	
	return SUCCESS;
}

int test_accel(void)
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
	float testtime ;
	
	begin = Timer() ;
	
	
	test_item = InsertTestItem("Accelorometer Test",NULL,NULL,NULL,"Testing...");
	
	for (tries=0;tries<7;tries++)
	{
		ret_val = send_dev_cmd(TEST_ACCEL);
	
		if (ret_val == FAILURE)
		{
			end = Timer(); 
			testtime = end - begin ;
			Insert_step_char("Accelorometer Test", NULL, NULL, NULL, testtime, "FAIL") ;
		
			UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
			return FAILURE;
		}
	
		ret_val = get_dev_response(buffer);
	
		if ( (ret_val == FAILURE) || (strstr(buffer,">")==NULL) ) 
		{
			end = Timer(); 
			testtime = end - begin ;
			Insert_step_char("Accelorometer Test", NULL, NULL, NULL, testtime, "FAIL") ;
			
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
		Insert_step_char("Accelorometer Test", NULL, NULL, NULL, testtime, "FAIL") ;
		
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
		Insert_step_char("Accelorometer Test", NULL, NULL, NULL, testtime, "FAIL") ;
		
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
		Insert_step_char("Accelorometer Test", NULL, NULL, NULL, testtime, "FAIL") ;
		
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	ptr+=2;
	z = atof(ptr);

	end = Timer();
	testtime = end - begin ;
	if (tries==7)
	{
		result_x = report_meas_test_item("Accel X value",x, 0.0001,-0.0001);
		result_y = report_meas_test_item("Accel Y value",y, 0.0001,-0.0001); 
		result_z = report_meas_test_item("Accel Z value",z, 0.0001,-0.0001);
	}
	else
	{
		result_x = report_meas_test_item("Accel X value",x, test_limits.accel.x.low,test_limits.accel.x.high);
		if (!result_x)
		
			Insert_step("Accelorometer Test X-axis",x ,test_limits.accel.x.low , test_limits.accel.x.high, testtime, "FAIL") ; 	
		
		else
			Insert_step("Accelorometer Test X-axis",x ,test_limits.accel.x.low , test_limits.accel.x.high, testtime, "PASS") ; 
			
		result_y = report_meas_test_item("Accel Y value",y, test_limits.accel.y.low,test_limits.accel.y.high); 
		
		if (!result_y)
			Insert_step("Accelorometer Test Y-axis",y ,test_limits.accel.y.low , test_limits.accel.y.high, testtime, "FAIL") ; 
		 else
			 Insert_step("Accelorometer Test Y-axis",y ,test_limits.accel.y.low , test_limits.accel.y.high, testtime, "PASS") ; 
		 
		result_z = report_meas_test_item("Accel Z value",z, test_limits.accel.z.low,test_limits.accel.z.high);
		if (!result_z)
			 Insert_step("Accelorometer Test Z-axis",z ,test_limits.accel.z.low , test_limits.accel.z.high, testtime, "FAIL") ; 
		else
			Insert_step("Accelorometer Test Z-axis",z ,test_limits.accel.z.low , test_limits.accel.z.high, testtime, "PASS") ; 
	}
	

	if ( !result_x | !result_y | !result_z)
	{
		
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	

	
	UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"PASS");
	
	return SUCCESS;
}
