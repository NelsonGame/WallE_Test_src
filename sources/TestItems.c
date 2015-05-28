#include <windows.h>
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include "TestItems.h"

TestItems_type TestItems;
TestItemLimits_type test_limits;

static char filename[MAX_PATHNAME_LEN]={0};
extern int g_module_level_test;

extern int g_need_config_file;

extern int g_overall_test_result;
extern int g_abort_test_flag;
extern int g_stop_on_fail;
extern int g_use_power_supply;
extern int g_use_rs232_power;
extern int g_gpib_pwr_addr;
extern int g_logfile_append_time;
extern double g_pwr_voltage,g_pwr_current;
//extern int g_pwr_serial_port;
extern int g_gps_101_port;
extern int g_dev_serial_port ;

extern ListType g_module_board_rev_list,g_pcba_board_rev_list;			

extern int g_initial_x_pos,g_initial_y_pos;
extern char g_current_module_board_rev[];
extern char g_current_pcba_board_rev[];
extern char g_current_module_board_file[];
extern char g_current_pcba_board_file[];

extern char database_connector[50] ;
extern char initfilepath[50] ;
extern char station_name[50] ;
extern char location[50] ;

int WriteIniInt(LPCSTR section,LPCSTR item,int value,LPCSTR filename)
{
	char buffer[32];
	
	sprintf(buffer,"%d",value);
	
	return WritePrivateProfileString(section,item,buffer,filename);
}

int GetIniInt(LPCSTR lpAppName,LPCSTR lpKeyName,INT nDefault,LPCSTR lpFileName)
{
	char buffer[128];
	int value;
	
	value = nDefault;
	
	GetPrivateProfileString(lpAppName,lpKeyName,"",buffer,128,lpFileName);
	
	if (strlen(buffer))
	{
		value = atoi(buffer);	
	}
	else
	{
		WriteIniInt(lpAppName,lpKeyName,nDefault,lpFileName);
	}
	
	return value;
}

void InitTestItems(void)
{
	
	
	TestItems.POWER_ON_SELFTEST = 0 ;
	TestItems.NFC_I2C_TEST = 0;
	TestItems.QUICK_NAND_TEST =1;
	TestItems.NFC_SCAN_TEST = 1;

	//TestItems.GPS_FIX_TEST = 	0;
	TestItems.GPS_CN_TEST = 	1;
	
	TestItems.SOUNDER_TEST 		= 	1;
	TestItems.VIBRATOR_TEST 	= 	1;
	TestItems.LED_TEST 			= 	1;
	TestItems.SENSORS_TEST  	=   1; 
	TestItems.FW_VERSION_TEST 	= 	1;
	TestItems.HW_VERSION_TEST 	= 	1; 
	TestItems.DEVICE_SERNUM  	= 	1; 
	TestItems.GASGAUGE_TEST  	= 	1 ;
	TestItems.GPS_SV_TRACK      =   1 ;
	TestItems.MFI_TEST       	= 	1 ;
	TestItems.CHARGING_TEST 	= 	0;
	TestItems.PMIC_I2C_TEST 	= 	0;
	
	//TestItems.WRITE_CHIP_ID = 1;
	
	// TestItems.PROGRAM_BOOTLOADER = 1;
	TestItems.PROGRAM_TESTAPP = 0;
	TestItems.LOAD_PRODUCTION_FW=0;
	
	//TestItems.RESET_BUTTON_TEST=0;
	TestItems.POWER_BUTTON_TEST=1;
	TestItems.MAIN_ON_SIG_TEST = 1 ;
	
	
	test_limits.gyro.x.high=9;
	test_limits.gyro.x.low=-9;
	
	test_limits.gyro.y.high=9;
	test_limits.gyro.y.low=-9;

	test_limits.gyro.z.high=9;
	test_limits.gyro.z.low=-9;

	test_limits.accel.x.high=5;
	test_limits.accel.x.low=-5;
	
	test_limits.accel.y.high=5;
	test_limits.accel.y.low=-5;

	test_limits.accel.z.high=5;
	test_limits.accel.z.low=-5;

	test_limits.compass.x.high=5;
	test_limits.compass.x.low=-5;
	
	test_limits.compass.y.high=5;
	test_limits.compass.y.low=-5;

	test_limits.compass.z.high=5;
	test_limits.compass.z.low=-5;
	
	test_limits.gasgauge.voltage.high = 4200 ; 
	test_limits.gasgauge.voltage.low  = 3900;
	test_limits.gasgauge.agv_curr.high = 200 ;
	test_limits.gasgauge.agv_curr.low  = 100 ;
	
	test_limits.gps_track.max_track_time.high = 30 ;
	test_limits.gps_track.max_track_time.low  = 1 ;
	test_limits.gps_track.num_of_sv.high = 32 ;
	test_limits.gps_track.num_of_sv.low  = 8 ;
	//test_limits.gps.max_gps_fix_time=120;
	//test_limits.nfc.max_nfc_scan_time=120;	

	//strcpy(TestItems.TestFwFile,"test_fw_v2.4.28_main.bin");
	//strcpy(TestItems.Bootloader_hexfile,"bootloader_main_v6.25.13.hex");
	//strcpy(TestItems.ProductionFwFile,"main_b10rev8_5.5.0.0_2013.10.09.bin");
	//strcpy(test_limits.fw_version,"2.4.28");	
}

static int get_filename(int options_file)
{
	ssize_t fileSize;
	char buffer[MAX_PATHNAME_LEN+256];
	
	if (options_file)
	{
		strcpy(filename,"c:\\walle_test_results\\TestStationOptions.ini");
	}
	else if (g_module_level_test)
	{
		//sprintf(filename,"c:\\walle_test_results\\%s",g_current_module_board_file);
		  sprintf(filename, initfilepath) ;
		  strcat(filename, g_current_module_board_file) ;
	}
	else
	{
		//sprintf(filename,"c:\\walle_test_results\\%s",g_current_pcba_board_file);
		sprintf(filename, initfilepath) ;
		strcat(filename, g_current_pcba_board_file) ;
	}
	
	if (FileExists (filename, &fileSize)!=1)
	{
		g_need_config_file = 0;
		sprintf(buffer,"Config File: %s\nIs MISSING!",filename);
		MessagePopup("Error Can't open Config file!",buffer);
		return 1;
	}
	
	return 0;
}

int GetIniString(LPCSTR lpAppName,LPCSTR lpKeyName,LPCSTR lpDefault,LPSTR lpReturnedString,DWORD nSize,LPCSTR lpFileName)
{
	char buffer[2048];
	DWORD value;
	
	value = GetPrivateProfileString(lpAppName,lpKeyName,"",buffer,2048,lpFileName);
	
	if (strlen(buffer))
	{
		strcpy(lpReturnedString,buffer);
	}
	else
	{
		strcpy(lpReturnedString,lpDefault);
		WritePrivateProfileString(lpAppName,lpKeyName,lpDefault,lpFileName);
	}
	
	return value;
}

int WriteIniDouble(char *section, char *item_name, double double_ptr)
{
	char buffer[256];
	int result;
	
	sprintf(buffer,"%f",double_ptr);
	
	result = WritePrivateProfileString(section,item_name,buffer,filename);
	
	return 0;
}

int GetIniDouble(char *section, char *item_name, double *double_ptr,double defaultValue)
{
	char buffer[256];
	int result;
	
	if (double_ptr==NULL) return 1;
	
	result = GetPrivateProfileString(section,item_name,"NONE",buffer,256,filename);
	
	if (strcmp(buffer,"NONE")==0)
	{
		*double_ptr = defaultValue;
		WriteIniDouble(section,item_name,defaultValue);
		return 1;
	}
	
	*double_ptr = atof(buffer);
	return 0;
}

int Save_TestLimits(void)
{
	int result=0;
	
	result += WriteIniDouble("GYRO_TEST","x.high",test_limits.gyro.x.high);
	result += WriteIniDouble("GYRO_TEST","x.low", test_limits.gyro.x.low);
	
	result += WriteIniDouble("GYRO_TEST","y.high",test_limits.gyro.y.high);
	result += WriteIniDouble("GYRO_TEST","y.low", test_limits.gyro.y.low);

	result += WriteIniDouble("GYRO_TEST","z.high",test_limits.gyro.z.high);
	result += WriteIniDouble("GYRO_TEST","z.low", test_limits.gyro.z.low);

	result += WriteIniDouble("ACCEL_TEST","x.high",test_limits.accel.x.high);
	result += WriteIniDouble("ACCEL_TEST","x.low", test_limits.accel.x.low);
	
	result += WriteIniDouble("ACCEL_TEST","y.high",test_limits.accel.y.high);
	result += WriteIniDouble("ACCEL_TEST","y.low",test_limits.accel.y.low);

	result += WriteIniDouble("ACCEL_TEST","z.high",test_limits.accel.z.high);
	result += WriteIniDouble("ACCEL_TEST","z.low",test_limits.accel.z.low);

	result += WriteIniDouble("COMPASS_TEST","x.high",test_limits.compass.x.high);
	result += WriteIniDouble("COMPASS_TEST","x.low",test_limits.compass.x.low);
	
	result += WriteIniDouble("COMPASS_TEST","y.high",test_limits.compass.y.high);
	result += WriteIniDouble("COMPASS_TEST","y.low",test_limits.compass.y.low);

	result += WriteIniDouble("COMPASS_TEST","z.high",test_limits.compass.z.high);
	result += WriteIniDouble("COMPASS_TEST","z.low",test_limits.compass.z.low);

	//result += WriteIniDouble("GPS_FIX_TEST","MAX_GPS_FIX_TIME",test_limits.gps.max_gps_fix_time);
	//result += WriteIniDouble("NFC_SCAN_TEST","MAX_NFC_SCAN_TIME",test_limits.nfc.max_nfc_scan_time);
	
	return result;
}

int Load_TestLimits(void)
{
	int result=0;
	double dbl_val;
	
	result += GetIniDouble("GYRO_TEST","x.high",&test_limits.gyro.x.high,5.0);
	result += GetIniDouble("GYRO_TEST","x.low",	&test_limits.gyro.x.low,-5.0);
	
	result += GetIniDouble("GYRO_TEST","y.high",&test_limits.gyro.y.high,5.0);
	result += GetIniDouble("GYRO_TEST","y.low",	&test_limits.gyro.y.low,-5.0);

	result += GetIniDouble("GYRO_TEST","z.high",&test_limits.gyro.z.high,5.0);
	result += GetIniDouble("GYRO_TEST","z.low",	&test_limits.gyro.z.low,-5.0);

	result += GetIniDouble("ACCEL_TEST","x.high",	&test_limits.accel.x.high,5.0);
	result += GetIniDouble("ACCEL_TEST","x.low",	&test_limits.accel.x.low,-5.0);
	
	result += GetIniDouble("ACCEL_TEST","y.high",	&test_limits.accel.y.high,5.0);
	result += GetIniDouble("ACCEL_TEST","y.low",	&test_limits.accel.y.low,-5.0);

	result += GetIniDouble("ACCEL_TEST","z.high",	&test_limits.accel.z.high,5.0);
	result += GetIniDouble("ACCEL_TEST","z.low",	&test_limits.accel.z.low,-5.0);

	result += GetIniDouble("COMPASS_TEST","x.high",	&test_limits.compass.x.high,5.0);
	result += GetIniDouble("COMPASS_TEST","x.low",	&test_limits.compass.x.low,-5.0);
	
	result += GetIniDouble("COMPASS_TEST","y.high",	&test_limits.compass.y.high,5.0);
	result += GetIniDouble("COMPASS_TEST","y.low",	&test_limits.compass.y.low,-5.0);

	result += GetIniDouble("COMPASS_TEST","z.high",	&test_limits.compass.z.high,5.0);
	result += GetIniDouble("COMPASS_TEST","z.low",	&test_limits.compass.z.low,-5.0);
	
	result += GetIniDouble("GPS_FIX_TEST","MAX_GPS_FIX_TIME",&test_limits.gps.max_gps_fix_time,180.0);
	
	result += GetIniDouble("CHARGING_TEST","charge_current.high",&test_limits.charging_test.charge_current.high,0.0001);
	result += GetIniDouble("CHARGING_TEST","charge_current.low",&test_limits.charging_test.charge_current.low,-1.000);
	
	result += GetIniDouble("GASGAUGE_TEST","voltage.high",	&test_limits.gasgauge.voltage.high,4200.00); 
	result += GetIniDouble("GASGAUGE_TEST","voltage.low",	&test_limits.gasgauge.voltage.low,3900.00); 
	
	result += GetIniDouble("GASGAUGE_TEST","avg_curr.high",	&test_limits.gasgauge.agv_curr.high,200.00); 
	result += GetIniDouble("GASGAUGE_TEST","avg_curr.low",	&test_limits.gasgauge.agv_curr.low, 100.00); 
	
	result += GetIniDouble("GPS_CN_TEST","MAX_GPS_CN_L1_TIME",&test_limits.gps.MAX_GPS_CN_L1_TIME,45.0);
	result += GetIniDouble("GPS_CN_TEST","MAX_GPS_CN_L2_TIME",&test_limits.gps.MAX_GPS_CN_L2_TIME,20.0);
	result += GetIniDouble("GPS_CN_TEST","MAX_GPS_CN_L3_TIME",&test_limits.gps.MAX_GPS_CN_L3_TIME,20.0);

	result += GetIniDouble("GPS_CN_TEST","CN_L1_ENABLE",&dbl_val,1.0);
	test_limits.gps.CN_L1_ENABLE = dbl_val;

	result += GetIniDouble("GPS_CN_TEST","CN_L2_ENABLE",&dbl_val,1.0);
	test_limits.gps.CN_L2_ENABLE = dbl_val;

	result += GetIniDouble("GPS_CN_TEST","CN_L3_ENABLE",&dbl_val,0.0);
	test_limits.gps.CN_L3_ENABLE = dbl_val;
	
	result += GetIniDouble("GPS_CN_TEST","CN_L1_POWER",	&test_limits.gps.CN_L1_POWER,-95.0);
	result += GetIniDouble("GPS_CN_TEST","CN_L2_POWER",	&test_limits.gps.CN_L2_POWER,-110.0);
	result += GetIniDouble("GPS_CN_TEST","CN_L3_POWER",	&test_limits.gps.CN_L3_POWER,-120.0);
	
	result += GetIniDouble("GPS_CN_TEST","CN_L1.high",	&test_limits.gps.CN_L1.high,60);
	result += GetIniDouble("GPS_CN_TEST","CN_L1.low",	&test_limits.gps.CN_L1.low,40);
	
	result += GetIniDouble("GPS_CN_TEST","CN_L2.high",	&test_limits.gps.CN_L2.high,50);
	result += GetIniDouble("GPS_CN_TEST","CN_L2.low",	&test_limits.gps.CN_L2.low,30);
	
	result += GetIniDouble("GPS_CN_TEST","CN_L3.high",	&test_limits.gps.CN_L3.high,30);
	result += GetIniDouble("GPS_CN_TEST","CN_L3.low",	&test_limits.gps.CN_L3.low,10);
	
	result += GetIniDouble("GPS_CN_TEST","GPS_SAT_NUM",&dbl_val,10.0);
	test_limits.gps.gps_sat_num = dbl_val;
	
	result  += GetIniDouble("GPS_SV_TRACK", "num_of_sv.high", &test_limits.gps_track.num_of_sv.high, 32.00) ;  
	result  += GetIniDouble("GPS_SV_TRACK", "num_of_sv.low", &test_limits.gps_track.num_of_sv.low, 8.00) ; 
	result  += GetIniDouble("GPS_SV_TRACK", "max_track_time.high", &test_limits.gps_track.max_track_time.high, 20.00) ; 
	result  += GetIniDouble("GPS_SV_TRACK", "max_track_time.low", &test_limits.gps_track.max_track_time.low, 1.00) ; 
	result  += GetIniDouble("GPS_SV_TRACK", "minCN", &dbl_val, 30.0 ) ;
	test_limits.gps_track.minCN = (int) dbl_val ; 
	
	/**********************************************************************************
	result += GetIniDouble("GPS_SV_TRACK", "num_of_sv", &dbl_val, 8.0);
	test_limits.gps_track.num_of_sv = (int) dbl_val ;
	result  += GetIniDouble("GPS_SV_TRACK", "minCN", &dbl_val, 30.0 ) ;
	test_limits.gps_track.minCN = (int) dbl_val ; 
	result  += GetIniDouble("GPS_SV_TRACK", "max_track_time", &test_limits.gps_track.max_track_time, 30.0 ) ;
	********************************************************************************************************/
	
	
	result += GetIniDouble("HW_VERSION_TEST","HW_VERSION",&dbl_val,2.0);
	test_limits.hw_version = dbl_val;
	
	result += GetIniString("FW_VERSION_TEST","FW_VERSION","",test_limits.fw_version,sizeof(test_limits.fw_version),filename);
		
	result += GetIniDouble("NFC_SCAN_TEST","MAX_NFC_SCAN_TIME",&test_limits.nfc.max_nfc_scan_time,120.0);
	
	return result;
}

int Load_TestItems(void)
{
	
	
	if (get_filename(FALSE)) return 1;


	TestItems.POWER_ON_SELFTEST  =  GetIniInt("TEST_ITEMS","POWER_ON_SELFTEST",0,filename);  
	TestItems.FW_VERSION_TEST = 	GetIniInt("TEST_ITEMS","FW_VERSION_TEST",1,filename); 
	TestItems.HW_VERSION_TEST = 		GetIniInt("TEST_ITEMS","HW_VERSION_TEST",1,filename); 
	TestItems.DEVICE_SERNUM = 		GetIniInt("TEST_ITEMS","DEVICE_SERNUM",1,filename);
	TestItems.NFC_SCAN_TEST = 		GetIniInt("TEST_ITEMS","NFC_SCAN_TEST",1,filename);     
	
	//TestItems.GPS_FIX_TEST = 		GetIniInt("TEST_ITEMS","GPS_FIX_TEST",1,filename);
	
	TestItems.SOUNDER_TEST = 		GetIniInt("TEST_ITEMS","SOUNDER_TEST",1,filename);
	TestItems.VIBRATOR_TEST = 		GetIniInt("TEST_ITEMS","VIBRATOR_TEST",1,filename);
	TestItems.LED_TEST = 			GetIniInt("TEST_ITEMS","LED_TEST",1,filename);
	TestItems.SENSORS_TEST = 		GetIniInt("TEST_ITEMS","SENSORS_TEST",1,filename); 
	
	    
	
	
	TestItems.GASGAUGE_TEST = 		GetIniInt("TEST_ITEMS","GASGAUGE_TEST",1,filename); 
	TestItems.MFI_TEST		 = 		GetIniInt("TEST_ITEMS","MFI_TEST",1,filename); 
	TestItems.GPS_CN_TEST = 		GetIniInt("TEST_ITEMS","GPS_CN_TEST",1,filename);  
	TestItems.GPS_SV_TRACK   =		GetIniInt("TEST_ITEMS","GPS_SV_TRACK",1,filename);
	
	TestItems.CHARGING_TEST = 		GetIniInt("TEST_ITEMS","CHARGING_TEST",1,filename); 
	TestItems.NFC_I2C_TEST = 		GetIniInt("TEST_ITEMS","NFC_I2C_TEST",1,filename);
	TestItems.QUICK_NAND_TEST = 	GetIniInt("TEST_ITEMS","QUICK_NAND_TEST",1,filename);
	TestItems.PMIC_I2C_TEST = 		GetIniInt("TEST_ITEMS","PMIC_I2C_TEST",1,filename); 
	
	//TestItems.WRITE_CHIP_ID = 		GetIniInt("TEST_ITEMS","WRITE_CHIP_ID",1,filename); 
	
	// TestItems.PROGRAM_BOOTLOADER = 		GetIniInt("TEST_ITEMS","PROGRAM_BOOTLOADER",1,filename); 
	TestItems.PROGRAM_TESTAPP = 		GetIniInt("TEST_ITEMS","PROGRAM_TESTAPP",1,filename); 
	
	TestItems.LOAD_PRODUCTION_FW =	GetIniInt("TEST_ITEMS","LOAD_PRODUCTION_FW",1,filename); 

	TestItems.POWER_BUTTON_TEST = 		GetIniInt("TEST_ITEMS","POWER_BUTTON_TEST",1,filename); 
	TestItems.MAIN_ON_SIG_TEST  = 		GetIniInt("TEST_ITEMS","MAIN_ON_SIG_TEST",1,filename); 
	
	//TestItems.RESET_BUTTON_TEST = 		GetIniInt("TEST_ITEMS","RESET_BUTTON_TEST",1,filename); 

	
	/**************************************************************************
	GetPrivateProfileString("FIRMWARE","BOOTLOADER_HEX_FILE","",TestItems.Bootloader_hexfile,MAX_PATHNAME_LEN,filename);
	GetPrivateProfileString("FIRMWARE", "TEST_FW_FILE","",TestItems.TestFwFile,MAX_PATHNAME_LEN,filename);
	GetPrivateProfileString("FIRMWARE", "PRODUCTION_FW_FILE","",TestItems.ProductionFwFile,MAX_PATHNAME_LEN,filename);
	*****************************************************/
	Load_TestLimits();
	
	
	return 0;
}

int Save_TestItems(void)
{
	get_filename(FALSE);

	
	WriteIniInt("TEST_ITEMS","POWER_ON_SELFTEST",TestItems.NFC_I2C_TEST,filename); 
	WriteIniInt("TEST_ITEMS","NFC_I2C_TEST",TestItems.NFC_I2C_TEST,filename);
	WriteIniInt("TEST_ITEMS","QUICK_NAND_TEST",TestItems.QUICK_NAND_TEST,filename);
	WriteIniInt("TEST_ITEMS","NFC_SCAN_TEST",TestItems.NFC_SCAN_TEST,filename);
	//WriteIniInt("TEST_ITEMS","GPS_FIX_TEST",TestItems.GPS_FIX_TEST,filename);
	//WriteIniInt("TEST_ITEMS","GPS_CN_TEST",TestItems.GPS_CN_TEST,filename);
	WriteIniInt("TEST_ITEMS","SOUNDER_TEST",TestItems.SOUNDER_TEST,filename);
	WriteIniInt("TEST_ITEMS","VIBRATOR_TEST",TestItems.VIBRATOR_TEST,filename);
	WriteIniInt("TEST_ITEMS","LED_TEST",TestItems.LED_TEST,filename);
	WriteIniInt("TEST_ITEMS","SENSORS_TEST",TestItems.SENSORS_TEST,filename);  
	
	WriteIniInt("TEST_ITEMS","FW_VERSION_TEST",TestItems.FW_VERSION_TEST,filename);

	WriteIniInt("TEST_ITEMS","CHARGING_TEST",TestItems.CHARGING_TEST,filename);
	WriteIniInt("TEST_ITEMS","GASGAUGE_TEST",TestItems.GASGAUGE_TEST,filename); 
	WriteIniInt("TEST_ITEMS","PMIC_I2C_TEST",TestItems.PMIC_I2C_TEST,filename);
	
	//WriteIniInt("TEST_ITEMS","WRITE_CHIP_ID",TestItems.WRITE_CHIP_ID,filename);
	
	// WriteIniInt("TEST_ITEMS","PROGRAM_BOOTLOADER",TestItems.PROGRAM_BOOTLOADER,filename);
	WriteIniInt("TEST_ITEMS","PROGRAM_TESTAPP",TestItems.PROGRAM_TESTAPP,filename);
	
	WriteIniInt("TEST_ITEMS","LOAD_PRODUCTION_FW",TestItems.LOAD_PRODUCTION_FW,filename);
	
	//WriteIniInt("TEST_ITEMS","RESET_BUTTON_TEST",TestItems.RESET_BUTTON_TEST,filename);
	WriteIniInt("TEST_ITEMS","POWER_BUTTON_TEST",TestItems.POWER_BUTTON_TEST,filename);
	
	WriteIniInt("TEST_ITEMS","HW_VERSION_TEST",TestItems.HW_VERSION_TEST,filename);
	/*******************************************************************
	WritePrivateProfileString("FIRMWARE","BOOTLOADER_HEX_FILE",TestItems.Bootloader_hexfile,filename);	
	WritePrivateProfileString("FIRMWARE", "TEST_FW_FILE",TestItems.TestFwFile,filename);
	WritePrivateProfileString("FIRMWARE", "PRODUCTION_FW_FILE",TestItems.ProductionFwFile,filename);
	***************************************************************************************************/
	Save_TestLimits();
	return 0;
}


TestItems_type * get_test_items(void)
{
	return &TestItems;
}


void SaveOptions(void)
{
	get_filename(TRUE);   
	
	WriteIniInt("CONFIG","g_overall_test_result",g_overall_test_result,filename);
	WriteIniInt("CONFIG","g_abort_test_flag",g_abort_test_flag,filename);
	//WriteIniInt("CONFIG","g_stop_on_fail",g_stop_on_fail,filename);
	WriteIniInt("CONFIG","g_use_power_supply",g_use_power_supply,filename);
	//WriteIniInt("CONFIG","g_use_rs232_power",g_use_rs232_power,filename);
	WriteIniInt("CONFIG","g_gpib_pwr_addr",g_gpib_pwr_addr,filename);
	WriteIniInt("CONFIG","g_logfile_append_time",g_logfile_append_time,filename);
	//WriteIniInt("CONFIG","g_pwr_serial_port",g_pwr_serial_port,filename);
	WriteIniInt("CONFIG","g_gps_101_port",g_gps_101_port,filename);
	WriteIniInt("CONFIG","g_dev_serial_port",g_dev_serial_port,filename); 
	WriteIniInt("CONFIG","g_module_level_test",g_module_level_test,filename);
	
	WriteIniDouble("CONFIG","g_pwr_voltage",g_pwr_voltage);
	WriteIniDouble("CONFIG","g_pwr_current",g_pwr_current);
	
	WritePrivateProfileString("CONFIG","CurrentModuleBoardRev",g_current_module_board_rev,filename);
	WritePrivateProfileString("CONFIG","CurrentModuleBoardFile",g_current_module_board_file,filename);
	
	WritePrivateProfileString("CONFIG","CurrentPCBABoardRev",g_current_pcba_board_rev,filename);
	WritePrivateProfileString("CONFIG","CurrentPCBABoardFile",g_current_pcba_board_file,filename);
	
}

void LoadOptions(void)
{
	char section[32767];
	char *ptr = NULL;
	char *tokptr = NULL;
	Board_rev_info_type board_rev_info;
	char curr_section[512];
	char boardrevfile[50] ;
	
	
	get_filename(TRUE);   
	
	g_overall_test_result = GetIniInt("CONFIG","g_overall_test_result",0,filename);
	g_abort_test_flag = 	GetIniInt("CONFIG","g_abort_test_flag",0,filename);
	//g_stop_on_fail = 		GetIniInt("CONFIG","g_stop_on_fail",1,filename);
	g_use_power_supply =	GetIniInt("CONFIG","g_use_power_supply",1,filename);
	g_use_rs232_power 	=	GetIniInt("CONFIG","g_use_rs232_power",0,filename);
	g_gpib_pwr_addr	=		GetIniInt("CONFIG","g_gpib_pwr_addr",7,filename);
	g_logfile_append_time=	GetIniInt("CONFIG","g_logfile_append_time",1,filename);
	//g_pwr_serial_port =		GetIniInt("CONFIG","g_pwr_serial_port",4,filename);
	g_gps_101_port =		GetIniInt("CONFIG","g_gps_101_port",23,filename);
	g_dev_serial_port =		GetIniInt("CONFIG","g_dev_serial_port",23,filename); 
	g_module_level_test =   GetIniInt("CONFIG","g_module_level_test",0,filename);
	
	GetIniString("CONFIG","CurrentModuleBoardRev","v1.7",g_current_module_board_rev,256,filename);
	GetIniString("CONFIG","CurrentModuleBoardFile","ModuleLevelTestItems.ini",g_current_module_board_file,256,filename);
	
	GetIniString("CONFIG","CurrentPCBABoardRev","v1.7",g_current_pcba_board_rev,256,filename);
	GetIniString("CONFIG","CurrentPCBABoardFile","PCBALevelTestItems.ini",g_current_pcba_board_file,256,filename);
	
	GetIniDouble("CONFIG","g_pwr_voltage",&g_pwr_voltage,4.0);
	GetIniDouble("CONFIG","g_pwr_current",&g_pwr_current,1.0);
	
	// added 08/19/2014 Nelson for server support
	
	GetIniString("SERVER","DSN","dummy", database_connector,30,filename); 
	
	 GetIniString("SERVER","init_path","dummypath", initfilepath,30,filename);   
	 GetIniString("SERVER","station_name","dummystation", station_name,30,filename); 
	 GetIniString("SERVER","location","dummylocation", location,30,filename); 
	 
	 /*********************************************************************************************
	 strcpy(boardrevfile, initfilepath) ;
	 strcat(boardrevfile, "\\board_rev.ini") ;
	 
	 GetIniString("PCBOARDREV","pcboard_rev","v1.0",g_current_module_board_rev,256,boardrevfile); 
	 GetIniString("PCBOARDREV","pcboard_rev","v1.0",g_current_pcba_board_rev,256,boardrevfile); 
	 *********************************************************************************************/
	// delete previous list it could have changed
	if (g_module_board_rev_list)
	{
		ListDispose(g_module_board_rev_list);
		g_module_board_rev_list=0;
	}
	
	g_module_board_rev_list = ListCreate(sizeof(Board_rev_info_type));
	
	if (g_module_board_rev_list)
	{
		GetPrivateProfileSection("MODULE_TEST",section,32767,filename);
		
		ptr = section;
		
		while(*ptr)
		{
			strcpy(curr_section,ptr);
			
			tokptr = strtok(curr_section,"=");
			
			if (tokptr)
			{
				strcpy(board_rev_info.rev_name,tokptr);
				
				tokptr = strtok(NULL,"\n");
				
				if (tokptr)
				{
					strcpy(board_rev_info.rev_file,tokptr);
					ListInsertItem (g_module_board_rev_list, &board_rev_info, END_OF_LIST);
				}
			}
			
			while(*ptr)
			{
				ptr++;
			}
			
			ptr++;
		}
	}
	
	// delete previous list it could have changed
	if (g_pcba_board_rev_list)
	{
		ListDispose(g_pcba_board_rev_list);
		g_pcba_board_rev_list=0;
	}
	
	g_pcba_board_rev_list = ListCreate(sizeof(Board_rev_info_type));
	
	if (g_pcba_board_rev_list)
	{
		GetPrivateProfileSection("PCBA_TEST",section,32767,filename);
		ptr = section;
		
		while(*ptr)
		{
			strcpy(curr_section,ptr);
			
			tokptr = strtok(curr_section,"=");
			
			if (tokptr)
			{
				strcpy(board_rev_info.rev_name,tokptr);
				
				tokptr = strtok(NULL,"\n");
				
				if (tokptr)
				{
					strcpy(board_rev_info.rev_file,tokptr);
					ListInsertItem (g_pcba_board_rev_list, &board_rev_info, END_OF_LIST);
				}
			}
			
			while(*ptr)
			{
				ptr++;
			}
			
			ptr++;
		}
	}
}

void LoadWindowOptions(void)
{
	get_filename(TRUE);   
	
	g_initial_x_pos = GetIniInt("WINDOW","g_initial_x_pos",0,filename);
	g_initial_y_pos = GetIniInt("WINDOW","g_initial_y_pos",0,filename);
}

void SaveWindowOptions(void)
{
	get_filename(TRUE);   
	
	WriteIniInt("WINDOW","g_initial_x_pos",g_initial_x_pos,filename);
	WriteIniInt("WINDOW","g_initial_y_pos",g_initial_y_pos,filename);
}


