#include "toolbox.h"
#include <formatio.h>
#include <utility.h>
#include <rs232.h> 
#include <userint.h>
#include "ansi_c.h"
#include "TestCommon.h"
#include "DevComm.h"
#include "TestItems.h" ;

#define MFI_TEST_CMD	"$AMT,OBJECT,MFI,TEST*00\r\n"

int MFI_test(void)
{
	char buffer[350] ;
	
	int byte_in_queue ;
	int index, test_item ; 
	//double begin, end;
	
	FlushInQ(g_dev_serial_port) ;
	FlushOutQ(g_dev_serial_port) ;
	
	Delay(.500) ;
	
	FillBytes(buffer, 0, 349, 32) ;
	buffer[349] = '\0';
	//pass_count = 0 ;
	test_item = InsertTestItem("MFI Test",NULL,NULL,NULL,NULL);    
	
	// begin = Timer(); 
	
	send_dev_cmd(MFI_TEST_CMD); 
	
	for (index = 0; index < 349; index++)
	{
		byte_in_queue = ComRdByte(g_dev_serial_port) ;
		if (byte_in_queue < 0) 
			break ;  // no more byte to read
		if ( (byte_in_queue == CR) || (byte_in_queue == LF) )
			buffer[index] = 32 ;
		else
			buffer[index] = (char) byte_in_queue;
	}	  // for all chars in COM buffer
	
	index = FindPattern(buffer,0,-1, "passed",0,0) ;
	 /***********************************************************
	if (index >0 )
	{
		pass_count++ ;
		if ( FindPattern(buffer, index + 10, -1, "passed", 0,0) > 0 ) 
			pass_count++ ;
	}
	
	if (pass_count >=2) 
	*******************************************************************/
	if (index > 0 )
		UpdateTestItem(test_item, "MFI Test", NULL, NULL, NULL, "PASS") ;
	else
		UpdateTestItem(test_item, "MFI Test", NULL, NULL, NULL, "FAIL") ; 
	
	return SUCCESS ;
}
