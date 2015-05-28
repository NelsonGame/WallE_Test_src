#ifndef TEST_ITEMS_H
#define TEST_ITEMS_H

#include "userint.h"

typedef struct TestItems_type{
	
	
	int  POWER_ON_SELFTEST ; 
	int NFC_I2C_TEST;
	int QUICK_NAND_TEST;
	int NFC_SCAN_TEST;
	int GPS_FIX_TEST;
	int GPS_CN_TEST;
	int SOUNDER_TEST;
	int VIBRATOR_TEST;
	int LED_TEST;
	int SENSORS_TEST ;
	int PMIC_I2C_TEST;
	int CHARGING_TEST;
	int	GASGAUGE_TEST ;
	int MFI_TEST ;
	int GPS_SV_TRACK ;
	int WRITE_CHIP_ID;
	// int PROGRAM_BOOTLOADER;
	int PROGRAM_TESTAPP;
	
	int POWER_BUTTON_TEST;
	int MAIN_ON_SIG_TEST ; 
	//int RESET_BUTTON_TEST ;
	
	int LOAD_PRODUCTION_FW;
	int FW_VERSION_TEST;
	int HW_VERSION_TEST;
	int DEVICE_SERNUM ;
	
	char Bootloader_hexfile[MAX_PATHNAME_LEN];
	char TestFwFile[MAX_PATHNAME_LEN];
	char ProductionFwFile[MAX_PATHNAME_LEN];
	
} TestItems_type;

int Load_TestItems(void);
int Save_TestItems(void);
TestItems_type * get_test_items(void);

typedef struct TestLimit_type{
	double low;
	double high;
} TestLimit_type;

typedef struct gyro_test_limits_type{
	TestLimit_type x,y,z;
} gyro_test_limits_type;

typedef struct accel_test_limits_type{
	TestLimit_type x,y,z;
} accel_test_limits_type;

typedef struct compass_test_limits_type{
	TestLimit_type x,y,z;
} compass_test_limits_type;

typedef struct gasgauge_test_limits_type{
	TestLimit_type voltage, agv_curr ;
} gasgauge_test_limits_type;



typedef struct gps_test_limits_type{
	double max_gps_fix_time;
	
	double MAX_GPS_CN_L1_TIME;
	double MAX_GPS_CN_L2_TIME;
	double MAX_GPS_CN_L3_TIME;
	
	TestLimit_type CN_L1;
	TestLimit_type CN_L2;
	TestLimit_type CN_L3;
	
	int gps_sat_num;
	
	double CN_L1_POWER;
	double CN_L2_POWER;
	double CN_L3_POWER;
	
	int CN_L1_ENABLE;
	int CN_L2_ENABLE;
	int CN_L3_ENABLE;
} gps_test_limits_type;

typedef struct charging_test_limits_type{
	
	TestLimit_type charge_current;
	
} charging_test_limits_type;

typedef struct nfc_test_limits_type{
	double max_nfc_scan_time;
} nfc_test_limits_type;

typedef struct gps_track_limits_type{
	TestLimit_type max_track_time, num_of_sv ;
	int minCN ;
} gps_track_limits_type ;

typedef struct TestItemLimits_type {
	gyro_test_limits_type 		gyro;
	accel_test_limits_type 		accel;
	compass_test_limits_type 	compass;
	gps_test_limits_type		gps;
	nfc_test_limits_type		nfc;
	charging_test_limits_type 	charging_test;
	gasgauge_test_limits_type        gasgauge ;
	gps_track_limits_type		gps_track ;		
	int hw_version;
	char fw_version[64];
	
} TestItemLimits_type;

typedef struct Board_rev_info_type{
	char rev_name[256];
	char rev_file[MAX_PATHNAME_LEN];
} Board_rev_info_type;

extern TestItemLimits_type test_limits;
extern TestItems_type TestItems;

void SaveOptions(void);
void LoadOptions(void);
void LoadWindowOptions(void);
void SaveWindowOptions(void);




#endif
