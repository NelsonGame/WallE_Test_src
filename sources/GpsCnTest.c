#include <formatio.h>
#include <rs232.h>
#include <utility.h>
#include "ansi_c.h"
#include "TestCommon.h"
#include "GpsCnTest.h"
#include "DevComm.h"
#include "TestItems.h"
#include "Gps101.h"
// #include "ActiveMindTest.h"
#include "WallE.h"

#define GPS_SAT_NUM 10

#define TEST_ENABLE_GPS "$AMT,GPS,1*00\r\n"
//#define GPS_FIX_TIMEOUT (2*60*1000) // 2min in ms units

//#define DEBUG_GPS

int GpsCnDebug;

void LeftTrim(char *string);
extern FILE* dev_log_file ;

char calc_gps_checksum(char *mstr)  
{
	char mx=0; 
	unsigned int mi;
	
	for( mi = 0; mi < strlen(mstr);mi++ ) mx ^= mstr[mi];
	
	return mx;
}

#define GPS_CHECKSUM(mstr,mx) mx=calc_gps_checksum(mstr);

char compareChecksum(char *sentence) {

    int cur_index=0, i=0;
    char string[100];
    char checksum_string[2]={0};
    char checksum=0, checksum_compare_value=0;
    int str_len = strlen(sentence);
	int j;
    if (sentence[0] != '$' || sentence[str_len - 5] != '*') {
        return 0;
    }
    //Get the message from the GPS string
    for(i=0; i<str_len; i++){
        //If we reach the end of the string without seeing a checksum indicator, return an error
        if (sentence[i]=='*') {
            break;
        }
        if(sentence[i]=='\n' || sentence[i]=='\r')return 0;
        if(sentence[i]!='$'){
            string[cur_index++]=sentence[i];
        }
    }
    string[cur_index++]='\0';
    
    //Get the checksum value from the gps string
    for( j=0; j<2; j++){
        //If the value of the checksum character isn't alphanumeric, return an error.
        char ac = sentence[++i];
        
        //ascii to hex
        if(ac >= 65)
            ac -=55;
        checksum_string[j]=ac; //Skip the '*'
    }
    
    //Combine the two checksum characters
    checksum=((checksum_string[0]<<4)&0xF0)|(checksum_string[1]&0x0F);
    //Find the checksum of the received string
    GPS_CHECKSUM(string,checksum_compare_value);
    
    //
    //Compare the calculated checksum to the received checksum; if they don't match we're out!
    if(checksum != checksum_compare_value)
    {
        return 0;
    }
    return 1;
}

char *my_strtok(char *string,char *character)
{
	char *orig_string;
	static char *last_pos;
	
	if (string==NULL)
	{
		orig_string = last_pos;
		string = orig_string;
	}
	else orig_string = string;
	
	if (string==NULL) return NULL;
	if (*string==NULL) return NULL;
	
	while (*string)
	{
		if (*string==*character)
		{
			*string = 0;
			last_pos = string+1;
			return orig_string;
		}
		
		string++;
	}
	
	last_pos = NULL;
	
	return orig_string; 
}

int parse_gps_command(char *gps_data,int sat_num,int *cn_val)
{
	char *ptr;
	int parameter;
	int param_mod;
	int satellite_num=0,elevation,azimuth,gps_cn;
	
	
	//gps_data[gps_data_index]=0;
	//gps_data_index = 0;
	
	
	if (strstr(gps_data,"$GPGSV"))
	{
		//if (!compareChecksum(gps_data)) return FAILURE;
	
		//UpdateStatus(gps_data);
		
		ptr = my_strtok(gps_data,"*");
		ptr = my_strtok(gps_data,",");
		
		parameter = 0;
		
		while(ptr)
		{
			if (parameter>3)
			{
				param_mod = (parameter-4) % 4;
				switch(param_mod)
				{
					case 0:
						satellite_num = atoi(ptr);
						if (satellite_num>50)
						{
							//Breakpoint();
						}
					break;
					case 1:
						elevation = atoi(ptr);
					break;
					case 2:
						azimuth = atoi(ptr);
					break;
					case 3:
						gps_cn = atoi(ptr);
						
						if (satellite_num == sat_num) 
						{
							*cn_val = gps_cn;
							return SUCCESS;
						}
						//update_satellite(satellite_num,elevation,azimuth,gps_cn);
					break;
				}
			}
			
			ptr = my_strtok(NULL,",");
			parameter++;
			
		}
		
	}
	
	return FAILURE;
}

/************************************
void reset_board(void)
{
	char buffer[1024];
	send_dev_cmd("RESET\r\n");
	get_dev_response(buffer);
}

**************************************/
extern int g_gps_101_port;

int test_gps_cn(void)
{
	int ret_val;
	int test_item;
	char buffer[100];
	//int old_timeout;
	//int GSV_count, SV10_count ;
	//char *ptr;
	double start,stop,elapsed;
	int gps_cn=0;
	int reading;
	int result;
	int byte_read ;
	char meas_name[128];
	char meas_time_name[128];

	
	test_item = InsertTestItem("GPS CN Test",NULL,NULL,NULL,"Testing...");
	
	if (init_gps_sim(g_gps_101_port)==FAILURE)
	{
		UpdateTestItem(test_item,"GPS101 SERIAL OPEN",NULL,NULL,NULL,"FAIL");
		

		return FAILURE;
	}

	if (gps_sim_set_pwr(test_limits.gps.CN_L1_POWER)==FAILURE)
	{
		gps_sim_set_output("OFF");  
		close_gps_sim();
		UpdateTestItem(test_item,"GPS101 RFPWR Command",NULL,NULL,NULL,"FAIL");



		return FAILURE;
	}  //if (gps_sim_set)
	
 	gps_sim_set_sat(test_limits.gps.gps_sat_num);
 	gps_sim_set_dplr(0);
 	gps_sim_set_test(1);
	gps_sim_set_output("ON");

	Delay(1.0);

	ret_val = send_dev_cmd(TEST_ENABLE_GPS);

	if (ret_val == FAILURE)
	{
		gps_sim_set_output("OFF"); 
		close_gps_sim();

		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");

		return FAILURE;
	}	 

	//old_timeout = set_dev_timeout(test_limits.gps.MAX_GPS_CN_L1_TIME * 1000); //convert to ms

	if (test_limits.gps.CN_L1_ENABLE)
	{
		sprintf(meas_name,"Gps CN @ %0.1f dBm",test_limits.gps.CN_L1_POWER);
		sprintf(meas_time_name,"Gps CN @ %0.1f dBm Test Time",test_limits.gps.CN_L1_POWER);


		//flush_dev();
		FlushInQ(g_dev_serial_port) ;
		
		start = Timer();
		reading = 0;
		// GSV_count = 0 ;
		// SV10_count = 0 ;
		do
		{
			stop = Timer();
			elapsed = stop-start;

			FillBytes(buffer, 0, 99, 32) ;
			buffer[99] = '\0' ;
			byte_read = ComRdTerm (g_dev_serial_port, buffer, 99, 10);
			if (dev_log_file) write_log_data(buffer) ;
			
			
			if (strstr(buffer,"$GPGSV"))
			{
				// GSV_count++ ;
				
				if (parse_gps_command(buffer,test_limits.gps.gps_sat_num,&gps_cn)==SUCCESS)
				{
	
					// SV10_count++ ;
					
					reading++;
					if (reading>1)
					{
						result = report_meas_test_item(meas_name,(double)gps_cn,test_limits.gps.CN_L1.low,test_limits.gps.CN_L1.high);	
						if (result == FAIL)
						{
							stop = Timer();
							elapsed = stop-start;
	
							
							report_meas_test_item(meas_time_name,elapsed,0,test_limits.gps.MAX_GPS_CN_L1_TIME);
						
							UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
							// set_dev_timeout(old_timeout);
							gps_sim_set_output("OFF"); 
							close_gps_sim();
							return FAILURE;
						}	  // (result == FAIL)
						break;
					}	 // (reading > 1)
	
				}		// ( parse_gps_command() == SUCCESS )
			}		     // (buffer == GPGSV)
		
			if (g_abort_test_flag) {
				
	
				gps_sim_set_output("OFF");  
				close_gps_sim();
				return FAILURE;
			}
	

		} while (elapsed < test_limits.gps.MAX_GPS_CN_L1_TIME);


	
		stop = Timer();
		elapsed = stop-start;
		
		report_meas_test_item(meas_time_name,elapsed,0,test_limits.gps.MAX_GPS_CN_L1_TIME);
	
		if (elapsed>test_limits.gps.MAX_GPS_CN_L1_TIME)
		{
			UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
			if (!g_abort_test_flag) report_meas_test_item(meas_name,0.0,test_limits.gps.CN_L1.low,test_limits.gps.CN_L1.high);
	

			gps_sim_set_output("OFF");  
			close_gps_sim();
			return FAILURE;
		}
	}
	
	if (test_limits.gps.CN_L2_ENABLE)
	{
		sprintf(meas_name,"Gps CN @ %0.1f dBm",test_limits.gps.CN_L2_POWER);
		sprintf(meas_time_name,"Gps CN @ %0.1f dBm Test Time",test_limits.gps.CN_L2_POWER);
	
	
		gps_sim_set_pwr(test_limits.gps.CN_L2_POWER);
	
	 	Delay(3.0);
	
		start = Timer();
	
		//flush_dev();
	
		FlushInQ(g_dev_serial_port) ;
		reading = 0;
		do
		{
			stop = Timer();
			elapsed = stop-start;

			//get_dev_line(buffer);
			
			//byte_read = ComRdTerm (1, buffer, 80, LF);
			byte_read = ComRdTerm (g_dev_serial_port, buffer, 80, LF); 
			if (strstr(buffer,"$GPGSV"))
			{
	
				if (parse_gps_command(buffer,test_limits.gps.gps_sat_num,&gps_cn)==SUCCESS)
				{
	

					reading++;
					if (reading>2)
					{
						report_meas_test_item(meas_name,(double)gps_cn,test_limits.gps.CN_L2.low,test_limits.gps.CN_L2.high);	
						break;
					}

				}
			}
		
			if (g_abort_test_flag) 
			{
	

	
				gps_sim_set_output("OFF");  
				close_gps_sim();
				return FAILURE;
			}
		

		} while (elapsed < test_limits.gps.MAX_GPS_CN_L2_TIME);
	
														
		stop = Timer();
		elapsed = stop-start;
	
		report_meas_test_item(meas_time_name,elapsed,0,test_limits.gps.MAX_GPS_CN_L2_TIME);
	
		if (elapsed>test_limits.gps.MAX_GPS_CN_L2_TIME)
		{
			gps_sim_set_output("OFF");  
			close_gps_sim();
	
			UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
			if (!g_abort_test_flag) report_meas_test_item(meas_name,0.0,test_limits.gps.CN_L2.low,test_limits.gps.CN_L2.high);
	

			return FAILURE;
		}
	}
	
	if (test_limits.gps.CN_L3_ENABLE)
	{
		sprintf(meas_name,"Gps CN @ %0.1f dBm",test_limits.gps.CN_L3_POWER);
		sprintf(meas_time_name,"Gps CN @ %0.1f dBm Test Time",test_limits.gps.CN_L3_POWER);


		gps_sim_set_pwr(test_limits.gps.CN_L3_POWER);
	
	 	Delay(3.0);
	
		//flush_dev();
		
		FlushInQ(g_dev_serial_port) ; 
		start = Timer();
	
		reading = 0;
		do
		{
			stop = Timer();
			elapsed = stop-start;

			//get_dev_line(buffer);
			
			//byte_read = ComRdTerm (1, buffer, 80, LF);
			byte_read = ComRdTerm (g_dev_serial_port, buffer, 80, LF); 
			if (strstr(buffer,"$GPGSV"))
			{
				

				if (parse_gps_command(buffer,test_limits.gps.gps_sat_num,&gps_cn)==SUCCESS)
				{
					

					reading++;
					if (reading>2)
					{
						report_meas_test_item(meas_name,(double)gps_cn,test_limits.gps.CN_L3.low,test_limits.gps.CN_L3.high);	
						break;
					}

				}
			}
		
			if (g_abort_test_flag)
			{
				

				gps_sim_set_output("OFF");  
				close_gps_sim();
				return FAILURE;
			}

		
		} while (elapsed < test_limits.gps.MAX_GPS_CN_L3_TIME);
	
														
		stop = Timer();
		elapsed = stop-start;

		if (elapsed>test_limits.gps.MAX_GPS_CN_L3_TIME)
		{
			gps_sim_set_output("OFF");  
			close_gps_sim();
	
			UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
			if (!g_abort_test_flag) report_meas_test_item(meas_name,0.0,test_limits.gps.CN_L3.low,test_limits.gps.CN_L3.high);
		
	
			
			return FAILURE;
		}
	
		report_meas_test_item(meas_time_name,elapsed,0,test_limits.gps.MAX_GPS_CN_L3_TIME);
	}
	
	UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"PASS");
	
	//set_dev_timeout(old_timeout);
	gps_sim_set_output("OFF");  
	close_gps_sim();
	
	
	
	return SUCCESS;
}
