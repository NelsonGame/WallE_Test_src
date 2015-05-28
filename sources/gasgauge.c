#include "toolbox.h"
#include <formatio.h>
#include <utility.h>
#include <rs232.h> 
#include <userint.h>
#include "ansi_c.h"
#include "TestCommon.h"
#include "DevComm.h"
#include "TestItems.h" ;

#define GAS_GAUGE_CMD "$AMT,OBJECT,GAS_GAGE*00\r\n"

extern int g_dev_serial_port ;

int gas_gauge_read(void)
{
	char buffer[300] ;
	int byte_in_queue ;
	int index ; 
	//double begin, end;
	char voltage[12] ;
	char avg_curr[12] ;
	// int ivoltage, iavg_curr  ; 
	double fvoltage, favg_curr ; 
	FlushInQ(g_dev_serial_port) ;
	FlushOutQ(g_dev_serial_port) ;
	
	Delay(.500) ;
	
	FillBytes(buffer, 0, 299, 32) ;
	buffer[299] = '\0';
	
	FillBytes(voltage, 0, 11, 32) ;
	voltage[11] = '\0' ;
	
	FillBytes(avg_curr, 0, 11, 32) ;
	avg_curr[11] = '\0';
	
	// begin = Timer() ;
	send_dev_cmd(GAS_GAUGE_CMD); 
	
	for (index = 0; index < 299; index++)
	{
		byte_in_queue = ComRdByte(g_dev_serial_port) ;
		if (byte_in_queue < 0) 
			break ;  // no more byte to read
		if ( (byte_in_queue == CR) || (byte_in_queue == LF) )
			buffer[index] = 32 ;
		else
			buffer[index] = (char) byte_in_queue;
	}	  // for all chars in COM buffer
	
	index = 0 ;
	index = FindPattern(buffer, 0, -1, "Voltage", 0, 0 );
	if (index > 0)
		 CopyString(voltage, 0, buffer, index+14, 6) ;
	index = FindPattern(buffer, 0, -1, "avg current", 0,0);
	if (index > 0)
		CopyString(avg_curr, 0, buffer, index+12, 6) ;
	 

	 fvoltage = atof(voltage) ;
	 favg_curr = atof(avg_curr) ;
	 
	report_meas_test_item("Gas gauge: voltage", fvoltage, test_limits.gasgauge.voltage.low,test_limits.gasgauge.voltage.high); 
	report_meas_test_item("Gas gauge: avg current", favg_curr, test_limits.gasgauge.agv_curr.low,test_limits.gasgauge.agv_curr.high);
	

	
	return SUCCESS ;
}
