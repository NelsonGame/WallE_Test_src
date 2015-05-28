#include <ansi_c.h>
#include <cvirte.h>
#include "flashmagiccommon.h"
#include "flashmagicarm.h"
#include <windows.h>
#include "TestCommon.h"



// erase progress function
// return 1 to continue erase
int erase_progress(int status, unsigned long value, unsigned long value2, void *callbackparam)
{
	char buffer[256];	
	sprintf(buffer,"erase status %d value %d value2 %d",status,value,value2);
	SetStatusLine(buffer);
	return 1;
}

// program progress function
// return 1 to continue programming
int program_progress(int status, unsigned long value, unsigned long value2, void *callbackparam)
{
	char buffer[256];	
	sprintf(buffer,"program status %d value %d value2 %d\n",status,value,value2);
	SetStatusLine(buffer);
	return 1;
}

// verify progress function
// return 1 to continue verifying
int verify_progress(int status, unsigned long value, unsigned long value2, void *callbackparam)
{
	char buffer[256];	
	sprintf(buffer,"verify status %d value %d value2 %d\n",status,value,value2);
	SetStatusLine(buffer);
	return 1;
}



int flash_magic_program(int comport,char *hexfile,char *errMsg) 
{
	//char hexfile[1000];
	//char comport[50];
	char baudrate[20];

	fm_results *presults;
	fm_connectoptions_com options;

	//sprintf(comport,"28");
	sprintf(baudrate,"38400");
	//sprintf(hexfile,"C:\\Users\\roylimon\\Documents\\GitHub\\Firmware-Bootloader\\bootloader_main.hex");

	// use standard timeouts
	fm_set_default_timeouts();

	// generate a debug file containing all ISP commands
	fm_select_debug_mode(FM_DEBUG_MODE_ON, "C:\\fmtest.fmd");

	options.osc            = 12.000;
	sprintf(options.comportname, "COM%d", comport);
	options.baudrate       = atoi(baudrate);
	options.selecteddevice = FM_LPC2148;
	options.highspeed      = 0;
	options.clocks         = 0;
	options.halfduplex     = 0;
	options.hwconfig       = FM_HWNONE; //FM_HWBOOTEXEC;
	options.hwt1           = 200;
	options.hwt2           = 200;
	options.i2caddr        = 0;
	options.maxbaudrate    = 115200;
	options.usinginterface = 0;
	options.interfacetype  = FM_INTERFACETYPE_NONE;
	options.flashbank      = 0;

  	// connect to the device on the specified com port using the specified baud rate
  	presults = fm_connect(&options, sizeof(options));
	
  	if (presults->result != FM_OK)
  	{
		strcpy(errMsg,presults->details);
		/*
		switch (presults->result)
		{
		  case FM_ERROR_PORT:
				MessageBox(NULL,"COM port error (connecting)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
			  break;
			case FM_ERROR_BAUDRATE:
				MessageBox(NULL,"Could not connect at baudrate (connecting)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
			case FM_ERROR_INVALID_PARAMS:
				MessageBox(NULL,"Invalid parameters (connecting)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
			case FM_ERROR_CMD:
				MessageBox(NULL,"Invalid command sent (connecting)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
			case FM_ERROR_CANCELLED:
				MessageBox(NULL,"Cancelled (connecting)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
			default:
				MessageBox(NULL,"Unknown error occurred (connecting)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
		}
		*/
		return 1;
  	}
	
  	// erase whole device except bootloader
  	presults = fm_erase(FM_DEVICE, 0, 1, erase_progress, 0, NULL);
	if (presults->result != FM_OK)
	{   
		strcpy(errMsg,presults->details);
		/*
		switch (presults->result)
		{
			case FM_ERROR_CMD:
				MessageBox(NULL,"Invalid command sent (erasing)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
			case FM_ERROR_INVALID_PARAMS:
				MessageBox(NULL,"Invalid parameters (erasing)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
			case FM_ERROR_CONNECT:
				MessageBox(NULL,"Not connected (erasing)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
			case FM_ERROR_UNSUPPORTED:
				MessageBox(NULL,"Erase not supported (erasing)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
			  break;
			case FM_ERROR_CANCELLED:
				MessageBox(NULL,"Cancelled (erasing)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
			case FM_ERROR_OPEN:
				MessageBox(NULL,"Unable to open hex file (erasing)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
			default:
				MessageBox(NULL,"Unknown error occurred (erasing)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
		}
		*/
  		
		fm_disconnect();
		return 1;
	}

	// program device
	presults = fm_program(hexfile, 0, 0, 0, 1, NULL, NULL, 0, FM_NORMAL, FM_PROGOPT_NONE, program_progress, 0);
	if (presults->result != FM_OK)
	{
		strcpy(errMsg,presults->details);
		/*
		switch (presults->result)
		{
			case FM_ERROR_PROGRAM:
				MessageBox(NULL,"Error programming (programming)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
			case FM_ERROR_HEX_CHECKSUM:
				MessageBox(NULL,"Incorrect checksum in hex file (programming)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
			case FM_ERROR_CHECKSUMS:
				MessageBox(NULL,"Error programming checksums (programming)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
			case FM_ERROR_GENERATE_CHECKSUMS:
				MessageBox(NULL,"Error generating checksums (programming)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
			case FM_ERROR_FILL:
				MessageBox(NULL,"Error filling memory (programming)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
			case FM_ERROR_OPEN:
				MessageBox(NULL,"Unable to open hex file (programming)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
			case FM_ERROR_CONNECT:
				MessageBox(NULL,"Not connected (programming)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
			case FM_ERROR_CMD:
				MessageBox(NULL,"Invalid command sent (programming)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
			case FM_ERROR_ALLOCATION:
				MessageBox(NULL,"Failed to allocate memory (programming)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
			case FM_ERROR_CANCELLED:
				MessageBox(NULL,"Cancelled (programming)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
			case FM_ERROR_JIT:
				MessageBox(NULL,"Error executing JIT module (programming)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
			case FM_ERROR_UNSUPPORTED:
				MessageBox(NULL,"Programming not supported (programming)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
			  break;
			default:
				MessageBox(NULL,"Unknown error occurred (programming)", "FM Test", MB_OK | MB_ICONEXCLAMATION);
				break;
		}
		*/
		fm_disconnect();
	    return 1;
	}

	// verify
	presults = fm_verify(hexfile, 0, verify_progress, 0);
	if (presults->result != FM_OK)
	{
		strcpy(errMsg,presults->details);
      	/*
		MessageBox(NULL,"Error verifying", "FM Test", MB_OK | MB_ICONEXCLAMATION);
	  	MessageBox(NULL,presults->details, "FM Test", MB_OK | MB_ICONEXCLAMATION);
	  	*/
		fm_disconnect();
      	return 1;
	}
	
	fm_reset(FM_RESET,0,0);

	// disconnect from device
	fm_disconnect();

	// tell user we are done
	//MessageBox(NULL,"Programming and verification complete", "FM Test", MB_OK | MB_ICONINFORMATION);
	return 0;
}

