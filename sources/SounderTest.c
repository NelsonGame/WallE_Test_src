#include <formatio.h>
#include <rs232.h>   
#include <utility.h>
#include <userint.h>
#include "ansi_c.h"
#include "TestCommon.h"
#include "VibratorTest.h"
#include "DevComm.h"

 #define TEST_SOUNDER  "$AMT,OBJECT,LED,TEST*00\r\n"
#define SOUNDER_ON "$AMT,SIGNAL,SOUNDER,ON*00\r\n"
#define SOUNDER_OFF "$AMT,SIGNAL,SOUNDER,OFF*00\r\n"

int test_sounder(void)
{
	int ret_val;
	int test_item; //index;
	//char buffer[50] = {" "};
	double begin, end ;
	float testtime ;
	//int byte_in_queue ;
	//int num_of_char ; 
	
	FlushInQ(g_dev_serial_port) ;
	FlushOutQ(g_dev_serial_port) ;
	
	//byte_in_queue = 0 ;
	//num_of_char = 39 ;
	
	
	begin = Timer() ;
	
	test_item = InsertTestItem("Sounder Test",NULL,NULL,NULL,"Testing...");
	
	ret_val = send_dev_cmd(SOUNDER_ON);
	
	
	if (ret_val == FAILURE)
	{
		end = Timer(); 
		testtime = end - begin ;
		Insert_step_char("Sounder Test", NULL, NULL, NULL, testtime, "FAIL") ; 
		
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	 /**************************************************************************
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
 
 	index = FindPattern(buffer, 0, -1, "Led test done",0,0) ;
	if (index > 0)
	{
		ret_val = ConfirmMessage ("Sounder Test", "Did you hear the Sounder?");
	
		if (ret_val!=YES)
		{
			end = Timer(); 
			testtime = end - begin ;
			Insert_step_char("Sounder Test", NULL, NULL, NULL, testtime, "FAIL") ; 
		
			UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
			return FAILURE;
		}
	}	
	******************************************************************************/
	 
	end = Timer(); 
	testtime = end - begin ;
	ret_val = ConfirmMessage ("Sounder Test", "Did you hear the Sounder?");
	
	if (ret_val == YES)
	
		  UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"PASS");
		 
	else 
	{
		 UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		 return FAILURE; 
	}
	
	//Insert_step_char("Sounder Test", NULL, NULL, NULL, testtime, "PASS") ; 
	
	ret_val = send_dev_cmd(SOUNDER_OFF);   
	
	
	
	
	return SUCCESS;
}
