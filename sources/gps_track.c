#include <tcpsupp.h>
#include <cvintwrk.h>
#include <utility.h>
#include <rs232.h>
#include <formatio.h>
#include <ansi_c.h>
#include <userint.h> 
#include <string.h>
#include "sentence.h"
#include "TestCommon.h"
#include "DevComm.h"
#include "TestItems.h" ; 

#define   GPS_ENABLE   "$AMT,GPS,1*00\r\n"

int labsat_config(void) ;
int labsat_stop(void) ;

//char hostname[20] = {"LABSATV3_037055"} ;
//char labsatIP[20] = {"192.168.1.225"} ; 
char* cmdbuffer = NULL ;
int handler = 0 ;
static char rcvBuffer[15] = {0} ;
//static unsigned int convers_handler ; 
int bytes_written = 0 ;

 int nmea_parser(void)
 {
	 int done = 0, GSVfound = 0 ;
	char firstline[100] ;
	int stat ;
	char** extralines = NULL;
	int *SatVID = NULL;
	int *SatCN = NULL;
	
	int sentence_count = 0;
	nmea_GSV first_line ;
	int test_item ;
	int num_of_items = 0 ;
	int sentence_index = 0 ;
	int sat_count = 0 ;
	
	int remain = 0 ;
	//int min_cn = 0 ;
	//int min_num_sat = 0 ;
	// float track_time = 0.0 ;
	int count = 0 ;
	int bytes_read = 0 ;
	double time1 ;
	double time_elapsed = 0.0;
	//double time_limit = 0.0 ;
	int out_of_time = 0;
	
	 first_line.sat_count = 0 ;
	 first_line.sentence_count = 0 ;
	 first_line.sentence_index = 0 ;
	 
	 
	 /** Asking the test firmware to start a GPS session *****/
	 //bytes_written = ComWrt (3, cmd, 15);
	 FlushInQ(g_dev_serial_port);
	 FlushOutQ(g_dev_serial_port) ;
	 
	 //time_limit = test_limits.gps_track.max_track_time.high ;
	 
	 test_item = InsertTestItem("GPS satellite tracking",NULL,NULL,NULL,NULL);
	 
	 stat = labsat_config() ;
	 if (stat == FAILURE) 
	 {
		 UpdateTestItem(test_item, "GPS satellite tracking", "ERROR", NULL, NULL, "FAIL") ;
		 return FAILURE ;
	 }
	 time_elapsed = 0.0 ;
	 time1 = 0.0 ;
	 send_dev_cmd(GPS_ENABLE);
			 
	 while ( !done && (time_elapsed <= test_limits.gps_track.max_track_time.high) )	
	 {
		
		FillBytes(firstline, 0, 99,32) ; 
		firstline[99] = '\0' ;
		//bytes_read = ComRdTerm (3, firstline, 99, 10);
		
	
		time1 = Timer() ;
		GSVfound = 0 ;
		while (!GSVfound )
	 	{
			// fgets(firstline,100, nmea_stream) ;
			bytes_read = ComRdTerm (g_dev_serial_port, firstline, 99, 10); 
			if ( strstr(firstline, "$GPGSV") )
			{
			
				//Scan(firstline, "%s>%s[dt#]%d", &sentence_count) ;
				Scan(firstline, "%s>%s[dt#]%d,%d,%d", &sentence_count, &sentence_index, &sat_count);  
				remain = sat_count % 4 ;
			
				if ( (sentence_count < 2) || (sat_count < 8) )
					continue ;
				//GSV_sentence = (nmea_GSV*)malloc(sentence_count * sizeof(nmea_GSV) ) ;
			
				extralines = malloc(sizeof(char*) * (sentence_count-1) ) ;
				for (int i = 0 ; i < sentence_count-1 ; i++)
				{
					extralines[i] = malloc(sizeof(char) * 100) ; 
					extralines[i][99] = '\0' ;
					//fgets(extralines[i], 99, nmea_stream ) ;
					bytes_read = ComRdTerm (g_dev_serial_port, extralines[i], 99, 10); 
					
				}
			
				SatVID = (int *)malloc(sizeof(int) * sat_count) ;
				SatCN   = (int *)malloc(sizeof(int) * sat_count) ;
			
			
		 		for (int i = 0 ; i < sat_count ; i++)
		 		{
			 		SatVID[i] = 0 ;
			 		SatCN[i] = 0 ;
		
		 		}
	 
			 
				GSVfound = 1 ;
			}   // if GPGSV is found
	
			// time_elapsed = Timer() - time1 ;
		}	 // while GPGS is not found
		
		
		num_of_items = Scan (firstline, "%s>%s[dt#]%d[d],%d[d],%d[d],%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d",
						 &SatVID[0], &SatCN[0], &SatVID[1], &SatCN[1],&SatVID[2], &SatCN[2],&SatVID[3], &SatCN[3]);
	
		Scan (extralines[0], "%s>%s[dt#]%d[d],%d[d],%d[d],%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d",
						 &SatVID[4], &SatCN[4], &SatVID[5], &SatCN[5],&SatVID[6], &SatCN[6],&SatVID[7], &SatCN[7]);
	
		switch (sentence_count)
		{
		
			case 3:
				switch (remain)
				{
					case 1:
						Scan (extralines[1], "%s>%s[dt#]%d[d],%d[d],%d[d],%d,%d[d],%d[d],%d",
						 &SatVID[8], &SatCN[8]);
					break;
				
					case 2:
						 Scan (extralines[1], "%s>%s[dt#]%d[d],%d[d],%d[d],%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d",
						 &SatVID[8], &SatCN[8], &SatVID[9], &SatCN[9]);
					break;
				
					case 3:
						  Scan (extralines[1], "%s>%s[dt#]%d[d],%d[d],%d[d],%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d",
						 &SatVID[8], &SatCN[8], &SatVID[9], &SatCN[9],&SatVID[10], &SatCN[10] );
					break;
				
					case 0:
						Scan (extralines[1], "%s>%s[dt#]%d[d],%d[d],%d[d],%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d",
						 &SatVID[8], &SatCN[8], &SatVID[9], &SatCN[9],&SatVID[10], &SatCN[10],&SatVID[11], &SatCN[11]);
					break;
				} // remain for sentence_count = 3
			break;   // case 3
					
			case 4:
				 Scan (extralines[1], "%s>%s[dt#]%d[d],%d[d],%d[d],%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d",
						 &SatVID[8], &SatCN[8], &SatVID[9], &SatCN[9],&SatVID[10], &SatCN[10],&SatVID[11], &SatCN[11]);
			 
				 switch (remain)
				{
					case 1:
						Scan (extralines[2], "%s>%s[dt#]%d[d],%d[d],%d[d],%d,%d[d],%d[d],%d",
						 &SatVID[12], &SatCN[12]);
					break;
				
					case 2:
						 Scan (extralines[2], "%s>%s[dt#]%d[d],%d[d],%d[d],%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d",
						 &SatVID[12], &SatCN[12], &SatVID[13], &SatCN[13]);
					break;
				
					case 3:
						  Scan (extralines[2], "%s>%s[dt#]%d[d],%d[d],%d[d],%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d",
						 &SatVID[12], &SatCN[12], &SatVID[13], &SatCN[13],&SatVID[14], &SatCN[14] );
					break;
				
					case 0:
						Scan (extralines[2], "%s>%s[dt#]%d[d],%d[d],%d[d],%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d",
						 &SatVID[12], &SatCN[12], &SatVID[13], &SatCN[13],&SatVID[14], &SatCN[14],&SatVID[15], &SatCN[15]);
					break;
				} // remain for sentence_count = 4
			break;   // case 4
					
			case 5:
				 Scan (extralines[1], "%s>%s[dt#]%d[d],%d[d],%d[d],%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d",
						 &SatVID[8], &SatCN[8], &SatVID[9], &SatCN[9],&SatVID[10], &SatCN[10],&SatVID[11], &SatCN[11]);
			 
				 Scan (extralines[2], "%s>%s[dt#]%d[d],%d[d],%d[d],%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d",
						 &SatVID[12], &SatCN[12], &SatVID[13], &SatCN[13],&SatVID[14], &SatCN[14],&SatVID[15], &SatCN[15]);
			 
				 switch (remain)
				{
					case 1:
						Scan (extralines[3], "%s>%s[dt#]%d[d],%d[d],%d[d],%d,%d[d],%d[d],%d",
						 &SatVID[16], &SatCN[16]);
					break;
				
					case 2:
						 Scan (extralines[3], "%s>%s[dt#]%d[d],%d[d],%d[d],%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d",
						 &SatVID[16], &SatCN[16], &SatVID[17], &SatCN[17]);
					break;
				
					case 3:
						  Scan (extralines[3], "%s>%s[dt#]%d[d],%d[d],%d[d],%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d",
						 &SatVID[16], &SatCN[16], &SatVID[17], &SatCN[17],&SatVID[18], &SatCN[18] );
					break;
				
					case 0:
						Scan (extralines[3], "%s>%s[dt#]%d[d],%d[d],%d[d],%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d,%d,%d[d],%d[d],%d",
						 &SatVID[16], &SatCN[16], &SatVID[17], &SatCN[17],&SatVID[18], &SatCN[18],&SatVID[19], &SatCN[19]);
					break;
				} // remain for sentence_count = 4
			break;   // case 5
					
		} // switch (sentence_count)	
		
		
	
		count = 0 ;
		for (int i = 0 ; i <= sat_count-1 ; i++)
		{
				
			if (SatCN[i] >= test_limits.gps_track.minCN )
				 count++ ;
	
		}  // 
	
		if (count >= test_limits.gps_track.num_of_sv.low )
		{
			done = 1 ;
			//time_elapsed = (float)(Timer()-time1) ;
			//SetCtrlVal(panelHandle, PANEL_TRACK_TIME,time_elapsed) ; 
		}
		 
		time_elapsed = Timer()-time1 ; 
		
		if (time_elapsed > test_limits.gps_track.max_track_time.high)
	
			out_of_time = 1 ;
			
	} // while not done	 and time not elapsed
			
	if (out_of_time) 
	
		UpdateTestItem(test_item, "GPS Satellite tracking", NULL, NULL, NULL, "FAIL") ;
	
	else
		UpdateTestItem(test_item, "GPS Satellite tracking", NULL, NULL, NULL, "PASS") ;  
	
	report_meas_test_item("Satellite track time", time_elapsed ,test_limits.gps_track.max_track_time.low, test_limits.gps_track.max_track_time.high) ; 
	report_meas_test_item("Number of satellites", count, test_limits.gps_track.num_of_sv.low, test_limits.gps_track.num_of_sv.high ) ; 
	
	labsat_stop();     
	
	 return SUCCESS;
 }	  // nmea_parser(void)


int labsat_config(void)
{
	
	char hostname[20] = {"LABSATV3_037055"} ;
	//char labsatIP[20] = {"192.168.1.225"} ; 
	//char cmdbuffer[30] ;   
	// char* cmdbuffer = NULL ;
	int stat  ;
	int byte_read;//index ; 
	int byte_written=0 ; 
	//cmdbuffer = (char*) malloc(10) ;
	//FillBytes(cmdbuffer, 0, 29, 32) ;
	//cmdbuffer[29] = '\0' ;
	//strcpy(cmdbuffer, "ATTN:10") ;
	
	
	handler = InetTelnetOpen (hostname, 23, 0);
	InetTelnetRead (handler, rcvBuffer, 12, &byte_read, 5000);
	
	
	if (handler >=0)
	{ 
		cmdbuffer = (char*) malloc(10) ;
	
		strcpy(cmdbuffer, "ATTN:10") ;
		
		
		if ( (stat = InetTelnetWrite (handler, cmdbuffer, "\r", strlen(cmdbuffer), &byte_written, -1)) < 0 )
		
		{
			MessagePopup("ERROR", "Failure in sending 'ATTN:10' to LabSat") ;
			return FAILURE ;
		}
		Delay(0.500) ;
		cmdbuffer = (char*) realloc(cmdbuffer, 30) ;
		strcpy(cmdbuffer, "PLAY:FILE:GPS2BIT_USA_Static") ;
	
		if ( (stat = InetTelnetWrite (handler, cmdbuffer, "\r", strlen(cmdbuffer), &byte_written, -1)) < 0 )    
		{
			MessagePopup("ERROR", "Failure in sending 'PLAY:FILE' to LabSat") ;
			return FAILURE ;
		}
		
		Delay(0.500) ;
	}
	else
	{
		MessagePopup("ERROR", "Failure in connecting to LabSat3") ;
		return FAILURE ;
	}
	return SUCCESS ;
}

int labsat_stop(void)
{
	int stat = 0 ;
	int byte_written=0 ; 
	
	cmdbuffer = (char*) realloc(cmdbuffer, 12) ;   
	strcpy(cmdbuffer, "PLAY:STOP") ;
	
	// if ( (stat = ClientTCPWrite (convers_handler, cmdbuffer, strlen(cmdbuffer), 0) ) < 0 )
		if ( (stat = InetTelnetWrite (handler, cmdbuffer, "\r", strlen(cmdbuffer), &byte_written, -1)) < 0 ) 
	{
		MessagePopup("ERROR", "Failure in sending 'PLAY:STOP' to LabSat") ;
		return FAILURE ;
	}
		
	Delay(0.500) ;
	stat = InetTelnetClose (handler);
	
	//stat = DisconnectFromTCPServer (convers_handler);
	
	return  SUCCESS ;
}
