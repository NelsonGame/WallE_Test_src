
#include "toolbox.h"
#include <formatio.h>
#include <utility.h>
#include <rs232.h> 
#include <userint.h>
#include "ansi_c.h"
#include "TestCommon.h"
#include "DevComm.h"
#include "TestItems.h" ;

extern int g_dev_serial_port ;
// extern char sernum[64] ;

int post(int run)
{
	int ret_val;
	int test_item, index;
	int byte_in_queue ;	 // old: int byte_in_queue ;
	char portname[6];
	//char hwID_limit[5] = {" "} ;  
	char pos_capture[3000]; 
	//char char_read ;
	double begin, end ;
	float testtime ;
	

	FillBytes(portname, 0, 5, 32) ;
	portname[5] = '\0' ;
	sprintf(portname,"COM%d",g_dev_serial_port) ;
	
	//test_item = InsertTestItem("RESET test", NULL, NULL, NULL, NULL); 
	
	ret_val = ConfirmMessage ("Turn unit back on", "Press the Power button and the Reset button. Did the three LEDs flash?"); 
	/********************************************************************************
	if (ret_val == YES)
	  
		UpdateTestItem(test_item, "RESET test", NULL, NULL, NULL, "PASS") ;
	else

		UpdateTestItem(test_item, "RESET test", NULL, NULL, NULL, "FAIL");  
		
	****************************************************************************/
	Delay(1.0) ;
	
	//test_item = InsertTestItem("Power On Self Test",NULL,NULL,NULL,NULL); 
	begin = Timer() ; 
	 
	FillBytes(pos_capture, 0, 2999, 32);
	pos_capture[2999] = '\0' ;
	test_item = InsertTestItem("Opening device port",NULL,NULL,NULL,NULL); 
	ret_val = OpenComConfig (g_dev_serial_port, portname, 115200, 0, 8, 1, 512, 512);
	if (ret_val < 0) 
	{
		UpdateTestItem(test_item, "Opening UUT port", "ERROR", NULL, NULL, "FAIL") ;
		return FAILURE ;
	}
	else
		UpdateTestItem(test_item, "Opening UUT port", NULL, NULL, NULL, "PASS") ; 
	if (!run)
    {
		ComWrtByte (1, 'n');
		
	}	
	else
	{
		ComWrtByte (1, 'Y'); 
		
		test_item = InsertTestItem("Power On Self Test",NULL,NULL,NULL,NULL); 
	
		SetComTime(g_dev_serial_port, 5.0) ;
		FlushInQ(g_dev_serial_port) ;
		FlushOutQ(g_dev_serial_port) ;
		//ComRd(g_dev_serial_port, pos_capture, 3000) ;
		for (index = 0; index < 2999; index++)
		{
			byte_in_queue = ComRdByte(g_dev_serial_port) ;
			if (byte_in_queue < 0) 
				break ;  // no more byte to read
			if ( (byte_in_queue == CR) || (byte_in_queue == LF) )
				pos_capture[index] = 32 ;
			else
				pos_capture[index] = (char) byte_in_queue;
		}	  // for all chars in COM buffer
	
	
		index = FindPattern(pos_capture, 0, -1, "failed", 0, 0) ;
		if (index > 0) 
		{
			   //test_item = InsertTestItem("Power On Self Test",NULL,NULL,NULL,"FAILED");  
			   UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL"); 
			   return FAILURE ;
		} 
		else
				//test_item = InsertTestItem("Power On Self Test",NULL,NULL,NULL,"PASS"); 
				UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"PASS"); 
	}   // else, perform post test
	
	return SUCCESS ;
}

int reset_test(void)
{
	int test_item, ret_val ;  
	
	test_item = InsertTestItem("RESET test", NULL, NULL, NULL, NULL); 
	
	ret_val = ConfirmMessage ("RESET test", "Press the RESET button. Did the three LEDs flash?"); 
	if (ret_val == YES)
	  
		UpdateTestItem(test_item, "RESET test", NULL, NULL, NULL, "PASS") ;
	else

		UpdateTestItem(test_item, "RESET test", NULL, NULL, NULL, "FAIL");  
	return SUCCESS ;
}

int pwr_btn_test(void)
{
	int ret_val ;
	int test_item ; 
	
	test_item = InsertTestItem("Pwr button test", NULL, NULL, NULL, NULL); 
	
	ret_val = ConfirmMessage ("Pwr Button test", "Press the POWER button. Are the LED OFF ?"); 
	if (ret_val == YES)
	  
		UpdateTestItem(test_item, "Pwr Button test", NULL, NULL, NULL, "PASS") ;
	else

		UpdateTestItem(test_item, "Pwr Button test", NULL, NULL, NULL, "FAIL");  
	
	return SUCCESS ;
}	

int main_on_sig_test(void)
{
	int ret_val ;
	int test_item ; 
	
	test_item = InsertTestItem("MAIN_ON Signal test", NULL, NULL, NULL, NULL); 
	
	ret_val = ConfirmMessage ("MAIN_ON Signal test", "Press the POWER button and wait for at least 10 seconds. Are the LED OFF ?"); 
	if (ret_val == YES)
	  
		UpdateTestItem(test_item, "MAIN_ON Signal test", NULL, NULL, NULL, "PASS") ;
	else

		UpdateTestItem(test_item, "MAIN_ON Signal test", NULL, NULL, NULL, "FAIL");  
	
	return SUCCESS ;
}	
