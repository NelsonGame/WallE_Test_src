#include <formatio.h>
#include <rs232.h>   
#include <utility.h>
#include <userint.h>
#include "ansi_c.h"
#include "TestCommon.h"
#include "LedTest.h"
#include "DevComm.h"

// #define TEST_LED_ON "LED_ON\r\n"
// #define TEST_LED_OFF "LED_OFF\r\n"
#define LED_TEST "$AMT,OBJECT,LED,TEST*00\r\n" 

int test_led(void)
{
	int ret_val;
	int test_item;
	char buffer[40] = {" "};
	int byte_in_queue ;
	int num_of_char = 39 ;
	double begin, end ;
	float testtime ;
	int index ; 
	
	FlushInQ(g_dev_serial_port) ;
	FlushOutQ(g_dev_serial_port) ;
	
	begin = Timer() ;
	
	
	test_item = InsertTestItem("Led Test",NULL,NULL,NULL,"Testing...");
	
	ret_val = send_dev_cmd(LED_TEST);
	
	if (ret_val == FAILURE)
	{
		end = Timer(); 
		testtime = end - begin ;
		// Insert_step_char("LED Test", NULL, NULL, NULL, testtime, "FAIL") ; 
		
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	 byte_in_queue = 0 ;
	 for (index = 0 ; index < num_of_char ; index++)
	 {
	  	  byte_in_queue = ComRdByte(g_dev_serial_port) ;
		  //if (byte_in_queue < 0)
		//	  break ;    // no more chars in serial port buffer
		  if ( (byte_in_queue == CR) || (byte_in_queue == LF) )
			buffer[index] = 32 ;
		  else
			buffer[index] = (char) byte_in_queue;  
	 } // for chars in COM port
	 
	 index = FindPattern(buffer, 0, -1, "pass",0,0) ;
	 if (index > 0)
	 	 UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"PASS");  
	 else
		 UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL"); 

	return SUCCESS;
}
