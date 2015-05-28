#include <userint.h>
#include <utility.h>
#include <ansi_c.h>
#include "TestCommon.h"
#include "SerialDevice.h"
#include "rs232.h"

//static int g_port_number;
//static double g_timeout_value=5000; //ms

extern FILE * dev_log_file;
void write_log_data(char *data);
void write_tx_log_data(char *data);

/****** OK to use for Wall-E *******/
int serial_send(int g_port_number,char *data,int len)
{
	int x;
	
	if (dev_log_file) write_tx_log_data(data);
	
	for (x=0;x<len;x++)
	{
		ComWrtByte(g_port_number,data[x]);
		
		while (GetOutQLen(g_port_number))
		{
			ProcessSystemEvents();
		}
		
		Delay(0.001);
	}
	//if (ComWrt(g_port_number,data,len)<=0) return FAILURE;
	return SUCCESS;
}

int serial_waitfor(int g_port_number,char *prompt,char *data,double g_timeout_value)
{
	int index =0;
	double start_time,elapsed,timeout;
	int inque;
	
	timeout = ((double)g_timeout_value / 1000.0);

	start_time = Timer();
	
	do
	{
		inque = GetInQLen(g_port_number);
		while (inque)
		{
			data[index] = (char)ComRdByte(g_port_number);
			index++;
			data[index]=0;
	
			if (strstr(data,prompt)!=NULL) 
			{
				if (dev_log_file) write_log_data(data);
				return SUCCESS;
			}
			inque--;
		}			
		
		elapsed = Timer() - start_time;
		
	} while (elapsed < timeout);
	
	if (dev_log_file) write_log_data(data);
	
	return FAILURE;
}

int serial_open(int com_port_num,int baud)
{
	int status;
	char com_port_str[128];
	
	sprintf(com_port_str,"COM%d",com_port_num);
	
	status = OpenComConfig (com_port_num, com_port_str, baud, 0, 8, 1, 2048, 2048);
	
	if (status<0) return FAILURE;
	
	return SUCCESS;
}

int serial_close(int g_port_number)
{
	int status;
	
	status = CloseCom(g_port_number);
	
	if (status!=0) return FAILURE;
	return SUCCESS;
}
