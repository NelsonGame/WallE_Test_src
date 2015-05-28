#include <utility.h>
#include "TestCommon.h"
#include "TestSequence.h"
#include "TestItems.h"
#include "VibratorTest.h"
#include "SounderTest.h"
#include "LedTest.h"

#include "NfcTest.h"
// #include "GpsFixTest.h"
#include "VersionTest.h"

#include "GpsCnTest.h"
// #include "ButtonTest.h"
#include "post_test.h"
#include "sensorstest.h"

extern int g_overall_test_result,g_abort_test_flag,g_stop_on_fail,g_use_power_supply;


void do_board_level_test(void)
{
	TestItems_type *pTestItems;
	int comm_status=0;
	int status;
	
	pTestItems = get_test_items();
	
	reset_test() ;
	
	if (pTestItems->POWER_BUTTON_TEST){
		
		//power_button_test();
		pwr_btn_test() ;  
		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
		
		// if (!g_stop_on_fail && pTestItems->FW_VERSION_TEST) comm_status = test_version();
	
		if (g_abort_test_flag || comm_status || (!g_overall_test_result && g_stop_on_fail)) return;
	}
	
	if (pTestItems->POWER_ON_SELFTEST) {
		
		post(1); 
		
		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
	}
	else
		post(0); 
	/***********************************************************************************
	if (pTestItems->PROGRAM_TESTAPP) {
		
		program_test_app();
		
		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
	}
	*****************************************************************************************/
	
	if (pTestItems->DEVICE_SERNUM){
		
		read_dev_serial() ; 

		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
		
		
	
		if (g_abort_test_flag || comm_status || (!g_overall_test_result && g_stop_on_fail)) return;
	}
	
	////////////////// Check firmware version /////////////////////////////////////////////
	if (pTestItems->FW_VERSION_TEST) 
	{
		
		comm_status = test_version();
		
		if (g_abort_test_flag || comm_status || (!g_overall_test_result && g_stop_on_fail)) return;
	}
	
	///////////////////  Hardware Version Test ///////////////////////////
	if (pTestItems->HW_VERSION_TEST){
		
		test_hw_version();

		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
		
		
	
		if (g_abort_test_flag || comm_status || (!g_overall_test_result && g_stop_on_fail)) return;
	}
	
	
	
	if (pTestItems->NFC_SCAN_TEST) {
		
		test_nfc_scan();
	
		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;

		Delay(1.0);
		
		
	
		if (g_abort_test_flag || comm_status || (!g_overall_test_result && g_stop_on_fail)) return;
	
	}
	/*************************************************************************************************
	if (pTestItems->POWER_BUTTON_TEST){
		
		power_button_test();

		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
		
		// if (!g_stop_on_fail && pTestItems->FW_VERSION_TEST) comm_status = test_version();
	
		if (g_abort_test_flag || comm_status || (!g_overall_test_result && g_stop_on_fail)) return;
	}

	if (pTestItems->RESET_BUTTON_TEST){
		
		reset_button_test();

		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
		
		//if (!g_stop_on_fail && pTestItems->FW_VERSION_TEST) comm_status = test_version();
	
		if (g_abort_test_flag || comm_status || (!g_overall_test_result && g_stop_on_fail)) return;
	}

	*****************************************************************************************************/
	
	/***********************************************************************************************
	if (pTestItems->PMIC_I2C_TEST) {
		
		test_pmic_i2c();
		
		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
	
		// if (!g_stop_on_fail && pTestItems->FW_VERSION_TEST) comm_status = test_version();
	
		if (g_abort_test_flag || comm_status || (!g_overall_test_result && g_stop_on_fail)) return;
	}
	
	*************************************************************************************************/
	if (pTestItems->GASGAUGE_TEST) {
		
		gas_gauge_read(); 
		
		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
								  
		if (g_abort_test_flag || comm_status || (!g_overall_test_result && g_stop_on_fail)) return;
			
	}
	
	/***********************************************************************************************
	if (g_use_power_supply) {
			if (pTestItems->CHARGING_TEST) {
		
			test_charging();
		
			if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
	
			
			if (g_abort_test_flag || comm_status || (!g_overall_test_result && g_stop_on_fail)) return;
		}
	}
	***********************************************************************************************/

	/***********************************************************************************************
	/////////// NAND TEST
	if (pTestItems->QUICK_NAND_TEST) {
		
		test_nand_quick();
	
		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
	
		// if (!g_stop_on_fail && pTestItems->FW_VERSION_TEST) comm_status = test_version();
	
		if (g_abort_test_flag || comm_status || (!g_overall_test_result && g_stop_on_fail)) return;
	
	}
	 **********************************************************************************************/

	///////////////// VIBRATOR
	
	if (pTestItems->VIBRATOR_TEST) {
		
		test_vibrator();
	
		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
	
		// if (!g_stop_on_fail && pTestItems->FW_VERSION_TEST) comm_status = test_version();
	
		if (g_abort_test_flag || comm_status || (!g_overall_test_result && g_stop_on_fail)) return;
	
	}
	
	
	///////////// SOUNDER /////////////////////////////////
	
	if (pTestItems->SOUNDER_TEST) {
		test_sounder();
	
		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
	
		if (g_abort_test_flag || comm_status || (!g_overall_test_result && g_stop_on_fail)) return;
	}
	
	///////////////// LED TEST  //////////////////////////////
	
	if (pTestItems->LED_TEST) {
		
		test_led();
	
		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
	
	
		if (g_abort_test_flag || comm_status || (!g_overall_test_result && g_stop_on_fail)) return;
	
	}
	
	if (pTestItems->SENSORS_TEST) {
		
		sensors_test();
	
		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
	
		
	
		if (g_abort_test_flag || comm_status || (!g_overall_test_result && g_stop_on_fail)) return;
	
	}
	
	if (pTestItems->MFI_TEST) {
		
		MFI_test();
	
		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
	
																				  
		if (g_abort_test_flag || comm_status || (!g_overall_test_result && g_stop_on_fail)) return;
	
	}
	
	if (pTestItems->MAIN_ON_SIG_TEST) {
		
		///pwr_btn_test();
		main_on_sig_test() ;
	
		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
	
																				  
		if (g_abort_test_flag || comm_status || (!g_overall_test_result && g_stop_on_fail)) return;
	
	}
	
// #ifndef NFC_AT_END
	///////////////// NFC I2C
	/***********************************************************************************
	if (pTestItems->NFC_I2C_TEST) {
		
		test_nfc_I2C();
	
		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
	
		// if (!g_stop_on_fail && pTestItems->FW_VERSION_TEST) comm_status = test_version();
	
		if (g_abort_test_flag || comm_status || (!g_overall_test_result && g_stop_on_fail)) return;
	}
	
	************************************************************************************************/
	////////////////// NFC SCAN
	
	/****************************************************************************************************
	if (pTestItems->NFC_SCAN_TEST) {
		
		test_nfc_scan();
	
		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;

		Delay(1.0);
		
		
	
		if (g_abort_test_flag || comm_status || (!g_overall_test_result && g_stop_on_fail)) return;
	
	}
	****************************************************************************************************/
// #endif

	
	
	
#ifdef NFC_AT_END	
	///////////////// NFC I2C
	if (pTestItems->NFC_I2C_TEST) {
		
		test_nfc_I2C();
	
		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
	
	
	
		if (g_abort_test_flag || comm_status || (!g_overall_test_result && g_stop_on_fail)) return;
	}
	
	////////////////// NFC SCAN
	if (pTestItems->NFC_SCAN_TEST) {
		
		test_nfc_scan();
	
		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;

		Delay(1.0);
		

	
		if (g_abort_test_flag || comm_status || (!g_overall_test_result && g_stop_on_fail)) return;
	
	}
#endif	

	if (pTestItems->GPS_CN_TEST) 
	{
		test_gps_cn(); 
	
		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
	}
	
	/****************************************************************************
	if (pTestItems->GPS_FIX_TEST) 
	{
		if (pTestItems->GPS_CN_TEST) reset_board();
			
		test_gps_fix(); 
	
		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
	}
	********************************************************************************/
	if (pTestItems->GPS_SV_TRACK) 
	{
		nmea_parser() ; 
		
		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
	}
	
	/*************************************************************************************
	if (pTestItems->LOAD_PRODUCTION_FW)
	{
		status = YES;
		
		if (!g_stop_on_fail) status = ConfirmMessage("PRODUCTION FIRMWARE","Do you want to load Production FW?");
		
		if (status==YES)
		{
			load_production_fw();
		}
		
		if (g_abort_test_flag || (!g_overall_test_result && g_stop_on_fail)) return;
	}
	*************************************************************************************/
}
