#include <utility.h>
#include "ansi_c.h"
#include "TestCommon.h"
#include "GpsFixTest.h"
#include "DevComm.h"
#include "TestItems.h"

#define TEST_GPS_FIX "GPS_FIX_TEST\r\n"
//#define GPS_FIX_TIMEOUT (2*60*1000) // 2min in ms units


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

int test_gps_fix(void)
{
	int ret_val;
	int test_item;
	char buffer[256];
	int old_timeout;
	char *ptr;
	double start,stop,elapsed;
	
	test_item = InsertTestItem("GPS Fix Test",NULL,NULL,NULL,"Testing...");
	
	ret_val = send_dev_cmd(TEST_GPS_FIX);
	
	start = Timer();
	
	if (ret_val == FAILURE)
	{
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	old_timeout = set_dev_timeout(test_limits.gps.max_gps_fix_time * 1000); //convert to ms
	
	//ret_val = get_dev_response(buffer);

	stop = Timer();
	elapsed = stop-start;
	
	
	if ( (ret_val == FAILURE) || (strstr(buffer,">")==NULL))
	{
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		
		if (!g_abort_test_flag) report_meas_test_item("Gps fix time",elapsed,0,120);
		
		set_dev_timeout(old_timeout);
		return FAILURE;
	}
	
	
		// skip the first line
	ptr = strstr(buffer,"\r\n");
	if (ptr==NULL)
	{
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		if (!g_abort_test_flag) report_meas_test_item("Gps fix time",elapsed,0,120);
		return FAILURE;
	}
	
	LeftTrim(ptr);
	//ptr+=2;
	
	if (atoi(ptr)==0)
	{
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		if (!g_abort_test_flag) report_meas_test_item("Gps fix time",elapsed,0,120);
		return FAILURE;
	}
	
	
	UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"PASS");
	report_meas_test_item("Gps fix time",elapsed,0,120);
	
	set_dev_timeout(old_timeout);
	
	return SUCCESS;
}
