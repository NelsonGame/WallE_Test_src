#include <userint.h>
//#include "ActiveMindTest.h"
#include "WallE.h"
#include <ansi_c.h>
#include "PowerSupply.h"
#include "SerialDevice.h"
#include "TestCommon.h" 
#include "Visa.h"
#include "TestItems.h"

static double ps_timeout_value = 5000;
// static int ps_com_port;

extern int g_use_rs232_power;

ViSession visaRM;
static ViSession ps_handle;

#define RESP_BUF_SIZE 256

//int init_power_supply(int comport,int gpib_address)
int init_power_supply(int gpib_address)
{
	//char comport_str[64]={0};
	int status=SUCCESS;
	char response[RESP_BUF_SIZE];
	char ps_desc[64];
	
	/*******************************************
	if (g_use_rs232_power)
	{
		ps_com_port = comport;
	
		status = serial_open(comport,9600);
	
	}
	*********************************************/
//	else
//	{
	sprintf(ps_desc,"GPIB0::%d::INSTR",gpib_address);
	
	if (!visaRM)
	{
		status = viOpenDefaultRM (&visaRM);
	}
	
	
	if (!ps_handle)
	{
		status = viOpen (visaRM, ps_desc, VI_NULL, VI_NULL, &ps_handle);
	}
//	}

	if (status==SUCCESS)
	{
		send_ps_command("*IDN?",response);
		send_ps_command("DIG:FUNC DIG",response);
		
		/**********************************************
		if (TestItems.PROGRAM_BOOTLOADER)
		{
			ps_enable_bootloader(TRUE);
		}
		else
		{
			ps_enable_bootloader(FALSE);
		}
		******************************************/
	}
	
	
	return status;
}

void close_power_supply(void)
{
	/***********************************
	if (g_use_rs232_power)
	{
		serial_close(ps_com_port);
	}
	********************************/
//	else
//	{
	viClose(ps_handle);
	ps_handle = 0;
//	}
}

int send_ps_command(char *command,char *response)
{
	int status;
	char buffer[RESP_BUF_SIZE];
	int count;
	
	sprintf(buffer,"%s\r\n",command);
	
	/***************************************************************************
	if (g_use_rs232_power)
	{
		status = serial_send(ps_com_port,buffer,strlen(buffer));
		if (status!=SUCCESS) return status;
	
		if (strchr(command,'?'))
		{
			status = serial_waitfor(ps_com_port,"\r\n",response,ps_timeout_value);
		}
	}
	***********************************************************************************/
//	else
//	{
	if (ps_handle==0) return 0;
	
	status = viPrintf(ps_handle,"%s\n",command);
	if (strchr(command,'?'))
	{
		status = viRead(ps_handle,(ViPBuf)response,256,(ViPUInt32)&count);
	}				 
//	}
	
	return status;
}

int set_voltage(double voltage)
{
	char response[RESP_BUF_SIZE];
	char command[64];
	int status;
	
	sprintf(command,"VOLT %0.3f",voltage);
	
	status = send_ps_command(command,response);
	
	return status;
}

int set_current(double current)
{
	char response[RESP_BUF_SIZE];
	char command[64];
	int status;
	
	sprintf(command,"CURR %0.3f",current);
	
	status = send_ps_command(command,response);
	
	return status;
}

int meas_voltage(double *voltage)
{
	char response[RESP_BUF_SIZE];
	
	int status;
	
	status = send_ps_command("MEAS:VOLT?",response);
	
	*voltage = atof(response);
	
	return status;
}

int meas_current(double *current)
{
	char response[RESP_BUF_SIZE];
	
	int status;
	
	status = send_ps_command("MEAS:CURR?",response);
	
	*current = atof(response);
	
	return status;
}

int set_output(int output)
{
	char *outp_cmd;
	int status;
	char response[RESP_BUF_SIZE];
	
	if (output) outp_cmd = "OUTPUT ON";
	else outp_cmd = "OUTPUT OFF";
	
	status = send_ps_command(outp_cmd,response);
	
	return status;
}

int get_syst_err(int *error)
{
	char response[RESP_BUF_SIZE];
	
	int status;
	
	status = send_ps_command("SYST:ERR?",response);
	
	*error = atoi(response);
	
	return status;
	
}

/***************************************************	 
int ps_enable_bootloader(int enable)
{
	char outp_cmd[64];
	int status;
	char response[RESP_BUF_SIZE];
	unsigned char value;

	status = send_ps_command("DIG:DATA?",response);
	
	value = (unsigned char)atoi(response);
	
	if (enable==FALSE)
	{
		value |= BL_DIS_BIT;
	}
	else 
	{
		value &= ~(BL_DIS_BIT);
	}
	
	sprintf(outp_cmd,"DIG:DATA %d",value);
	
	status = send_ps_command(outp_cmd,response);
	
	return status;
}

 *********************************************************/

