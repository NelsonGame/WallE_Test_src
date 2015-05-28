#include <rs232.h>
#include <formatio.h>
#include <utility.h>
#include "ansi_c.h"
#include "TestCommon.h"
#include "VibratorTest.h"
#include "DevComm.h"
// #include "ActiveMindTest.h"
#include "WallE.h"
#include "TestItems.h"

// #define TEST_NFC_SCAN "NFC_SCAN_TEST\r\n"
 #define TEST_NFC_I2C "NFC_I2C_TEST\r\n"
#define NUM_OF_CHAR 30
//#define NFC_SCAN_TIMEOUT (2*60*1000) // 2min in ms units

int test_nfc_scan(void)
{
	//int ret_val;
	int test_item = 0, index = 0;
	char buffer[35] = {" "};
	char tag_keyword[5] = {" "} ;
	// char *ptr;
	// int old_timeout;
	char nfc_id_str[128] = {" "};
	int tag_found ; 
	int nfc_scan_panel;
	unsigned long long nfc_id;
	double begin, end ;
	float testtime = 0.0, scantime ;
	
	FlushInQ(g_dev_serial_port) ;
	FlushOutQ(g_dev_serial_port) ;
	
	begin = Timer() ;
	
	
	nfc_scan_panel = LoadPanel(0,"WallE.uir",NFC_PANEL);
	
	InstallPopup (nfc_scan_panel);
	
	// old_timeout = set_dev_timeout(test_limits.nfc.max_nfc_scan_time * 1000);
	
	test_item = InsertTestItem("NFC Scan Test",NULL,NULL,NULL,NULL);
	
	scantime = 0.0;
	tag_found = 0 ; 
	while ( (!tag_found) && (scantime  < 15.0) )
	{
		
		ComRdTerm(g_dev_serial_port, buffer,NUM_OF_CHAR,10) ;  
	//ComRd (g_dev_serial_port, buffer, NUM_OF_CHAR,10);
		index = FindPattern(buffer, 0,-1,"TAG",0,0) ;
		if (index > 0)
		{
			 
			Scan(buffer, "%s>%s %s", tag_keyword, nfc_id_str) ;
			if (strlen(nfc_id_str) >= 14)
				tag_found = 1;
		}
		
		scantime = Timer() - begin ;
	}   // while loop
		end = Timer(); 
		testtime = end - begin ;
	//Insert_step_char("NFC Scan Test", NULL, NULL, NULL, testtime, "PASS") ;
	   if (tag_found)
		UpdateTestItem(test_item,NULL,nfc_id_str,NULL,NULL,"PASS");
	   else if ( (!tag_found) && (scantime >= 10.0) )
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL"); 	   
	//set_dev_timeout(old_timeout);
	RemovePopup(0);	
	return SUCCESS;
}

/****************************************************************************
int test_nfc_I2C(void)
{
	int ret_val;
	int test_item;
	char buffer[256];
	char *ptr;
	
	test_item = InsertTestItem("NFC I2C Test",NULL,NULL,NULL,"Testing...");
	
	ret_val = send_dev_cmd(TEST_NFC_I2C);
	
	if (ret_val == FAILURE)
	{
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	//ret_val = get_dev_response(buffer);
	
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

**********************************************************************/
