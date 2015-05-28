#include <ansi_c.h>
#include "Gps101.h"
#include "TestCommon.h"
#include "SerialDevice.h"


static int gps_sim_serial_port_num;

int init_gps_sim(int serial_portnum)
{
	gps_sim_serial_port_num = serial_portnum;
	return serial_open(gps_sim_serial_port_num,9600);
}

int close_gps_sim(void)
{
	return serial_close(gps_sim_serial_port_num);
}	 
int gps_sim_set_pwr(double power)
{
	char cmd[128];
	char response[1024];
	
	int status;
	
	sprintf(cmd,"MAIN:RFLV %0.0f\r\n",power);
	
	status = serial_send(gps_sim_serial_port_num,cmd,strlen(cmd));
	status = serial_waitfor(gps_sim_serial_port_num,"\r\n!",response,3000);
	return status;
}

int gps_sim_set_sat(int sat_num)
{
	char cmd[128];
	char response[1024];
	
	int status;
	
	sprintf(cmd,"MAIN:SV %d\r\n",sat_num);
	
	status = serial_send(gps_sim_serial_port_num,cmd,strlen(cmd));
	status = serial_waitfor(gps_sim_serial_port_num,"\r\n!",response,3000);
	
	return status;
}

//int gps_sim_set_output(int output)
int gps_sim_set_output(char * output) 
{
	char cmd[128];
	char response[1024];
	
	int status;
	
	//sprintf(cmd,"MAIN:RFCO %d\r\n",output);
	
	sprintf(cmd,"MAIN:RFCO %s\r\n",output); 
	status = serial_send(gps_sim_serial_port_num,cmd,strlen(cmd));
	status = serial_waitfor(gps_sim_serial_port_num,"\r\n!",response,3000);
	
	return status;
}

int gps_sim_set_dplr(int doppler)
{
	char cmd[128];
	char response[1024];
	
	int status;
	
	sprintf(cmd,"MAIN:DPLR %d\r\n",doppler);
	
	status = serial_send(gps_sim_serial_port_num,cmd,strlen(cmd));
	status = serial_waitfor(gps_sim_serial_port_num,"\r\n!",response,3000);
	
	return status;
}

int gps_sim_set_test(int testNum)
{
	char cmd[128];
	char response[1024];
	
	int status;
	
	sprintf(cmd,"MAIN:TEST %d\r\n",testNum);
	
	status = serial_send(gps_sim_serial_port_num,cmd,strlen(cmd));
	status = serial_waitfor(gps_sim_serial_port_num,"\r\n!",response,3000);
	
	return status;
}


