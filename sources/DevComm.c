#include <windows.h>
#include <rs232.h>
#include <userint.h>
#include <utility.h>
#include <ansi_c.h>
#include "TestCommon.h"
#include "DevComm.h"
#include "SerialDevice.h"

// #include "FTChipID.h"
// #include "ftd2xx.h"

#define DEV_PROMPT "AMT>"

#define USE_CVI_COMM

//static FT_HANDLE ftHandle; // valid handle returned from FT_OpenEx 
//static g_timeout_value=5000;
	
extern int g_abort_test_flag;
extern int g_dev_serial_port ;    
extern char *device_sernum ;

FILE *dev_log_file;
//static char cSerialNumber[50];

double dev_log_start_timer;

int GetRunMsTimer(void)
{
	return (int)((Timer()-dev_log_start_timer)*1000.0);
}

void write_dev_log_data(int tx_dir,char *data_ptr)
{
	int len;
	int x;
	int pos;
	unsigned char *data;
	
	data=(unsigned char *) data_ptr;
	
	len = strlen(data_ptr);
	pos = 0;
	
	if (tx_dir) fprintf(dev_log_file,"%08.8d==>",GetRunMsTimer());
	else fprintf(dev_log_file,"%08.8d<==",GetRunMsTimer());
	
	for (x=0;x<len;x++)
	{
		if (data[x]<32)
		{
			if(x-pos) fwrite(data+pos,x-pos,1,dev_log_file);
			
			pos = x+1;
			
			switch(data[x])
			{
				case '\n':
					fputs("\\n",dev_log_file);
				break;
				case '\r':
					fputs("\\r",dev_log_file);
				break;
				case '\t':
					fputs("\\t",dev_log_file);
				break;
				default:
					fprintf(dev_log_file,"\\x%02.2X",(unsigned int)data[x]);
				break;
			}
		}
	}
	
	if (data[pos]) fwrite(data+pos,x-pos,1,dev_log_file);
	fputs("\n",dev_log_file);
}

void write_tx_log_data(char *data)
{
	write_dev_log_data(TRUE,data);
}
// assume RX data
void write_log_data(char *data)
{
	write_dev_log_data(FALSE,data);
}

/*******************************************************************************
int DetectUsbDevice(char *SerialNumber)
{
	FTID_STATUS dStatus=0;
	unsigned long ulNumDevices=0, ulLocID, ulChipID;
	char cDescription[100], cLocationIDS[32], cChipIDS[32], ErrorMessage[256];
	int i;
	int tries;
	
	if (SerialNumber==NULL) return 1;
	
	for (tries=0;tries < 45;tries++)
	{
		dStatus = FTID_GetNumDevices(&ulNumDevices);

		if((dStatus == FTID_SUCCESS) && ulNumDevices) {

			for( i = 0; i < (int)ulNumDevices; i++) {
	
				dStatus = FTID_GetDeviceSerialNumber(i, cSerialNumber, 50);
				if(dStatus == FTID_SUCCESS) {
					//strcpy(SerialNumber,cSerialNumber);
				}

				dStatus = FTID_GetDeviceDescription(i, cDescription, 100);
				if(dStatus == FTID_SUCCESS) {
				}

				dStatus = FTID_GetDeviceLocationID(i, &ulLocID);
				if(dStatus == FTID_SUCCESS) {
					sprintf(cLocationIDS, "0x%08X", ulLocID);
				}

				dStatus = FTID_GetDeviceChipID(i, &ulChipID);
				if(dStatus == FTID_SUCCESS) {
					//sprintf(cChipIDS, "0x%08X", ulChipID);
					sprintf(cChipIDS, "%u", ulChipID);
					strcpy(SerialNumber,cChipIDS);
					break;
				}
	#ifdef PRINT_INFO			
				printf("SerialNumber: %s\r\n"
					"Device Desc:%s\r\n"
					"Device Location:%s\r\n"
					"Device ChipID:%s\r\n",  SerialNumber, cDescription,cLocationIDS,cChipIDS);
			
	#endif
			}
			break;			
		}
		
		if (g_abort_test_flag) return FAILURE;
		
		Delay(1.0);
	}
	
	if(dStatus != FTID_SUCCESS) {
		FTID_GetErrorCodeString("EN", dStatus, ErrorMessage, 256);
		MessageBox(NULL, ErrorMessage, NULL, MB_OK);
		return 1;
	}	
	
	if ((ulNumDevices==0)||(tries>44))
	{
		return 1;
	}
	
	return 0;
}

 ***************************************************************************************/
 /***** 
 WallE test firmware does not return any prompt after any command. So this function can be 
 removed 
 ****/
 /********************************************************
int get_dev_response(char *data)
{
//#ifdef USE
	//serial_waitfor(int g_port_number,char *prompt,char *data,double g_timeout_value)  
	if (serial_waitfor(g_dev_serial_port,DEV_PROMPT,data,)==FAILURE) return FAILURE;


	return SUCCESS;
}

*************************************************/
 
 /************************************************************
void flush_dev(void)
{
	//char buffer[1024]={0};
	DWORD bytes_in_queue=0;
	int bytes_to_read=0;
	DWORD bytes_read = 0;
	double start,stop,elapsed;
	
	//FT_GetQueueStatus(ftHandle,&bytes_in_queue);
	
	if (bytes_in_queue)
	{
		start = Timer();
		while (bytes_in_queue)
		{
			if (bytes_in_queue>1024) bytes_to_read = 1024;
			else bytes_to_read = bytes_in_queue;
			
			// read and ignore
			//FT_Read(ftHandle,buffer,bytes_to_read,&bytes_read);
			
			bytes_in_queue -= bytes_read;
			stop=Timer();
			elapsed = stop-start;
			
			if (elapsed>2.0) break;
		}
	}
}

************************************************************/
// reads upto one line of data
 
 /****************************************************************
int get_dev_line(char *data)
{
//#ifdef USE_CVI_COMM
	// serial_waitfor(int g_port_number,char *prompt,char *data,double g_timeout_value)  
	if (serial_waitfor(g_dev_serial_port,"\n",data, 5.0)==FAILURE) return FAILURE;


	return SUCCESS;
}

*******************************************************************/

int send_dev_cmd(char *cmd)
{
	// int x;

   FlushOutQ(g_dev_serial_port) ;
	if (serial_send(g_dev_serial_port,cmd,strlen(cmd))==FAILURE) return FAILURE;
	
	
	return SUCCESS;	
}

 /*********************************************
int set_dev_timeout(int timeout)
{
	int old_timeout;
	
	old_timeout = g_timeout_value;
	
	g_timeout_value = timeout;
	
//#ifdef USE_CVI_COMM
	return serial_set_timeout(timeout);
//#else
//	return old_timeout;
// #endif
}

********************************************/
char *getTime(void);
char *getDate(void);

// char *get_chip_id_string(void);

/********************************************************************************************
int get_dev_com_port(int *p_com_port_num)
{
	//FT_STATUS ftStatus; 
	LONG lComPortNumber;
	int tries;
	char filename[MAX_PATHNAME_LEN];
	
	if (p_com_port_num)
	{
		*p_com_port_num=0;
		
		for (tries=0;tries<20;tries++)
		{
			ftStatus = FT_Open(0,&ftHandle); 
			
			if (ftStatus == FT_OK) 
			{
				
				ftStatus = FT_GetComPortNumber(ftHandle,&lComPortNumber);
			
				if (ftStatus == FT_OK) 
				{ 
					if (lComPortNumber == -1) { 
						// No COM port assigned 
					//	return FAILURE;
						FT_Close(ftHandle);

						if (g_abort_test_flag) return FAILURE;
							
						Delay(3.0);
						continue;
					} 
					else 
					{ 
						// COM port assigned with number held in lComPortNumber 
						*p_com_port_num = lComPortNumber;
					} 
				} 
				else 
				{ 
					// FT_GetComPortNumber FAILED! 
					FT_Close(ftHandle);
					
					if (g_abort_test_flag) return FAILURE;
					
					Delay(3.0);
					continue;
					//return FAILURE;
					
				} 
			
	#ifdef USE_CVI_COMM
				FT_Close(ftHandle);
				ftHandle = 0;
				ProcessSystemEvents();
				Delay(1);
				if (dev_log_file) 
				{
					fclose(dev_log_file);
					dev_log_file=NULL;
				}
					
#else
				
				sprintf(filename,"c:\\walle_test_results\\dev_log_%s_%s_%s",get_chip_id_string(),getTime(),getDate());
				dev_log_file = fopen(filename,"a");
				dev_log_start_timer = Timer();
	#endif
				
				break;
			}
			
			Delay(1.0);
			if (g_abort_test_flag) return FAILURE;
		}
		
		if (tries==10)
		{
			return FAILURE;
		}
	}
	
	return SUCCESS;
}

********************************************************************************/
int com_port_num;

/**********************************************************************************************
int init_dev(void)
{
// #ifdef USE_CVI_COMM
	char com_port_str[128]={0};
// #endif
	int status;
	
	status = get_dev_com_port(&com_port_num);

//#ifdef USE_CVI_COMM
	return serial_open(com_port_num);

#else
	g_timeout_value = 5000;
	
	status = FT_SetTimeouts(ftHandle,5000,1000);
	status = FT_SetFlowControl(ftHandle,FT_FLOW_NONE,0,0);
	
	status = FT_SetBaudRate(ftHandle,115200); //3000000);
	status = FT_SetDataCharacteristics (ftHandle, FT_BITS_8,FT_STOP_BITS_1, FT_PARITY_NONE);
		
	return status;	
#endif

}
***************************************************************************************************/


int close_dev(void)
{
// #ifdef USE_CVI_COMM
	return serial_close(g_dev_serial_port);

/*********************************************************
#else
	if (FT_Close(ftHandle)!=FT_OK) return FAILURE;
	
	if (dev_log_file) 
	{
		fclose(dev_log_file);
		dev_log_file=NULL;
	}
	
	return SUCCESS;
#endif
*********************************************************/

}

