#include <formatio.h>
#include <rs232.h>
#include <utility.h>
#include <userint.h>
#include "ansi_c.h"
#include "TestCommon.h"
#include "VibratorTest.h"
#include "DevComm.h"

#define TEST_VIBRATOR_OBJECT "$AMT,OBJECT,NOTIFICATION,TEST*00\r\n"
#define TEST_VIBRATOR_ON "$AMT,SIGNAL,VIBRATE,ON*00\r\n"
#define TEST_VIBRATOR_OFF "$AMT,SIGNAL,VIBRATE,OFF*00\r\n"

extern int g_dev_serial_port ;   

int test_vibrator(void)
{
	int ret_val;
	int test_item=0;
	// int index=0, try, endtest ;
	//char buffer[40] = {" "};
	// int num_of_char = 30 ;
	// int byte_in_queue = 0 ;
	double begin, end ;
	float testtime; 
	// float vibrate_time ;
	// unsigned short pass_count ;
	
	SetComTime(g_dev_serial_port, 12.0) ; 
	
	

	// vibrate_time = 0.0 ;
	//endtest = 0 ;
	FlushInQ(g_dev_serial_port) ;
	FlushOutQ(g_dev_serial_port) ;
	
	test_item = InsertTestItem("Vibrator Test",NULL,NULL,NULL,NULL); 
	begin = Timer() ;
	
	ret_val = send_dev_cmd(TEST_VIBRATOR_ON);
	
	if (ret_val == FAILURE)
	{
		end = Timer(); 
		testtime = end - begin ;
		// Insert_step_char("Vibrator Test", NULL, NULL, NULL, testtime, "FAIL") ; 
	
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	
	
	// pass_count = 0 ;
	// byte_in_queue = 0 ;
	// try = 0 ;
	/*******************************************************************************
	//while ( (pass_count < 1 ) && () ) 
	while (!endtest)
	{
		pass_count = 0 ;
		// Delay(2.0) ;
		//  FlushInQ(g_dev_serial_port) ;
		// FlushOutQ(g_dev_serial_port) ;
		
	
		
		// ret_val = send_dev_cmd(TEST_VIBRATOR);
	
		//if (ret_val == FAILURE)
		// {
		//	end = Timer(); 
		//	testtime = end - begin ;
			// Insert_step_char("Vibrator Test", NULL, NULL, NULL, testtime, "FAIL") ; 
		
		//	UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		//	return FAILURE;
		// }
	
		
		 byte_in_queue = 0 ;
		 
	     
		// pass_count = 0 ;
					
		byte_in_queue = ComRd (g_dev_serial_port, buffer, num_of_char);
		
		index = FindPattern(buffer, 0, -1, "end test", 0,0);
		if (index > 0)
			endtest = 1; 
		
	
		
	} // while vibrator is still ON
	***********************************************************************************/
	
	end = Timer(); 
	testtime = end - begin; 
	// if (pass_count == 1)
	
	ret_val = ConfirmMessage ("Vibrator Test", "Did you hear the Vibrator?");
	
	if (ret_val == YES)
	
		  UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"PASS");
		 
	else 
		 UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL"); 
	
	send_dev_cmd(TEST_VIBRATOR_OFF);  
	
	SetComTime(g_dev_serial_port, 5.0) ; 
	//Delay(8.00) ;
	
	return SUCCESS;
}
