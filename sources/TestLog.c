#include "cvi_db.h"
#include <utility.h>
#include "toolbox.h"
#include <ansi_c.h>
#include "TestLog.h"
#include "TestCommon.h"

static FILE *log_file;

 extern int database_hdl ;
 extern int SQL_hdl ;
 extern char station_name[50] ; 
 extern char location[50] ;

 //extern char TestDate[15] ;
 //extern char TestTime[10] ;
 
 
char *getTime(void)
{
	static char time[32];
	int hour,min,sec;
	
	GetSystemTime(&hour,&min,&sec);
	
	sprintf(time,"%02.2d%02.2d%02.2d",hour,min,sec);
	
	return time;
}

char *getDate(void)
{
	static char date[32];
	int month,day,year;
	
	GetSystemDate(&month,&day,&year);
	
	sprintf(date,"%02.2d%02.2d%02.2d",month,day,year);
	
	return date;
}

extern int g_logfile_append_time;

char *get_test_sw_ver(void);

int InitTestLog(char *serial_num,char *factory_serial_number, g_module_level_test)
{
	char dir_name[MAX_PATHNAME_LEN];
	char file_name[MAX_PATHNAME_LEN];
	char *time,*date;
	//char SQLbuff[500] ;
	unsigned long long chipID ;
	time = getTime();
	date = getDate();
	char TestDate[15] ;
	char TestTime[10] ;
	long fetch_stat; 
	// extern long long pcba_id ;
	int month, day, year, hour, minute, second; 
	
	
	//GetProjectDir(dir_name);
	strcpy(dir_name,"c:\\walle_test_results");
	
	if (FileExists(dir_name,0)!=1) 
	{
		MakeDir(dir_name);	
	}
	
	if (g_logfile_append_time)
	{
		if (!g_module_level_test)  // board level 
			sprintf(file_name,"%s\\WALLE_LOG_%s_%s_%s_%s.csv",dir_name,serial_num,factory_serial_number,time,date);
		else
			sprintf(file_name,"%s\\WALLE_LOG_%s_%s_%s.csv",dir_name,serial_num,time,date); 
	}
	else 
	{   if (!g_module_level_test)
			sprintf(file_name,"%s\\WALLE_LOG_%s_%s_%s.csv",dir_name,serial_num,factory_serial_number,date);
		else
			sprintf(file_name,"%s\\WALLE_LOG_%s_%s.csv",dir_name,serial_num, date); 
	}	
	
	log_file = fopen(file_name,"a");	
	
	if (log_file==NULL) return FAILURE;
	
	fprintf(log_file,"Chip ID,%s\n",serial_num);
	fprintf(log_file,"Factory Serial Number,%s\n",factory_serial_number);
	fprintf(log_file,"Test Software Version,%s\n",get_test_sw_ver());
	fprintf(log_file,"Test Start Time,%s\n",time);
	fprintf(log_file,"Test Start Date,%s\n",date);
	
	fprintf(log_file,"Test Name,Meas Value,Low Limit,High Limit,Test Result\n");
	
	
	//chipID = atol(serial_num) ;
	StrToULongLong(serial_num, &chipID);  // for PostgreSQL
	
	GetSystemDate (&month, &day, &year);
	GetSystemTime (&hour, &minute, &second);
	sprintf(TestDate, "%02d/%02d/%04d", month, day, year);
	sprintf(TestTime, "%02d:%02d:%02d", hour, minute, second);
	
	 /*****************************************************************************************************************
	if (g_module_level_test)
	{
		  strcpy(SQLbuff, " ") ;
		  sprintf(SQLbuff, "INSERT INTO walle.module_uut_results"
		  "(module_chipid,module_station_name, module_location,module_start_date, module_start_time,module_test_time,module_pass)"
		  "VALUES('%llu','%s','%s','%s','%s','%.3f', '%d')RETURNING id", chipID, station_name, location, TestDate, TestTime, 0.0, FAIL);
		  
		  SQL_hdl = DBActivateSQL (database_hdl, SQLbuff); 
		  
		 
		  DBBindColLongLong (SQL_hdl, 1, &module_id, &fetch_stat);
		 
		 DBFetchNext(SQL_hdl) ;
		 
		
		 if (fetch_stat == DB_TRUNCATION)
			 MessagePopup("ERROR", "returned data was truncated");
		 if (fetch_stat == DB_NULL_DATA)
			 MessagePopup ("ERROR", "NULL data received") ;
		 
		 DBDeactivateSQL(SQL_hdl) ;
  			
				
	}
	else
	{
		 strcpy(SQLbuff, " ") ;
		 sprintf(SQLbuff, "INSERT INTO walle.pcba_uut_results"
		  "(pcba_chipid,mfg_serial,pcba_station_name, pcba_location, pcba_start_date, pcba_start_time, pcba_test_time,pcba_pass)"
		  "VALUES('%llu','%s', '%s','%s','%s','%s','%.3f', '%d') RETURNING id", chipID, factory_serial_number, station_name, location, TestDate, TestTime, 0.0, FAIL);
		  
		 SQL_hdl = DBActivateSQL (database_hdl, SQLbuff);
		
		 
		 DBBindColLongLong (SQL_hdl, 1, &pcba_id, &fetch_stat);
		 
		 DBFetchNext(SQL_hdl) ;
		 
		
		 if (fetch_stat == DB_TRUNCATION)
			 MessagePopup("ERROR", "returned data was truncated");
		 if (fetch_stat == DB_NULL_DATA)
			 MessagePopup ("ERROR", "NULL data received") ;
		 
		 DBDeactivateSQL(SQL_hdl) ;
	}
	***********************************************************************************************************************************/
	
	return SUCCESS;
}

int AddTestLog(char *test_item,char *meas_value,char *low_limit,char *high_limit,char *result)
{
	char *p_test_item,*p_meas_value,*p_low_limit,*p_high_limit,*p_result;
	char empty[]="\0";
	
	if (log_file==NULL) return FAILURE;
	
	if (test_item==NULL) p_test_item = empty;
	else p_test_item = test_item;
	
	if (meas_value==NULL) p_meas_value = empty;
	else p_meas_value = meas_value;
	
	if (low_limit==NULL) p_low_limit = empty;
	else p_low_limit = low_limit;
	
	if (high_limit==NULL) p_high_limit = empty;
	else p_high_limit = high_limit;
	
	if (result==NULL) p_result = empty;
	else p_result = result;
	
	fprintf(log_file,"%s,%s,%s,%s,%s\n", p_test_item, p_meas_value, p_low_limit, p_high_limit, p_result);
	
	return SUCCESS;
	
}

int CloseTestLog(void)
{
	if (log_file==NULL) return FAILURE;
	
	fclose(log_file);	
	
	return SUCCESS;
}
