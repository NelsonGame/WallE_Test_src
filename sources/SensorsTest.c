
#include <formatio.h>
#include <rs232.h>
#include <utility.h>
#include <userint.h>
#include <analysis.h>

#include "ansi_c.h"
#include "TestCommon.h"
#include "DevComm.h"
#include "TestItems.h" 

#define SENSORS_CMD		"$AMT,SENSOR,10*00\r\n"
#define DATA_SET	10 
#define NUM_BLANK_LINES  7
extern int g_dev_serial_port ; 

int sensors_test(void)
{
	int ret_val;
	int test_item;
	char sensordata[DATA_SET][100];
	char  emptyline[2] ;
	char  header[100] ;
	//char databuffer[8000]; 
	double begin, end ;
	float testtime ;
	int ax[DATA_SET], ay[DATA_SET], az[DATA_SET] ; 
	int mx[DATA_SET], my[DATA_SET], mz[DATA_SET] ;
	int gx[DATA_SET], gy[DATA_SET], gz[DATA_SET] ;
	
	int i, sum_ax = 0 , sum_ay = 0, sum_az = 0;
	int sum_mx = 0, sum_my = 0, sum_mz = 0 ;
	int sum_gx = 0, sum_gy = 0, sum_gz = 0; 
	
	double avg_ax = 0.0, avg_ay= 0.0, avg_az = 0.0; 
	double avg_mx = 0.0, avg_my = 0.0 , avg_mz = 0.0;
	double avg_gx = 0.0, avg_gy = 0.0, avg_gz = 0.0 ;
	

	
	for (i = 0; i < DATA_SET; i++)
	{
		FillBytes(sensordata[i], 0, 99, 32) ; 
		sensordata[i][99] = '\0'; 
		ax[i] = 0 ;
		ay[i] = 0 ;
		az[i] = 0;
		mx[i] = 0 ;
		my[i] = 0 ;
		mz[i] = 0; 
		gx[i] = 0 ;
		gy[i] = 0 ;
		gz[i] = 0 ;
		
	}
	
	
	//FillBytes(header, 0, 99, 32) ;
	//header[99] = '\0' ;
	//FillBytes(str_ave_ax, 0, 9, 32} ;
	//str_ave_ax 
	
	begin = Timer() ;
	
	//test_item = InsertTestItem("Sensors Test",NULL,NULL,NULL,NULL);
	
	ret_val = send_dev_cmd(SENSORS_CMD);
	 // ret_val = send_dev_cmd ("$AMT,SENSOR,100*00\r\n") ; 
	
	if (ret_val == FAILURE)
	{
		end = Timer(); 
		testtime = end - begin ;
		//Insert_step_char("Sensors Test", NULL, NULL, NULL, testtime, "FAIL") ;
	
		UpdateTestItem(test_item,NULL,NULL,NULL,NULL,"FAIL");
		return FAILURE;
	}
	
	
	for (i = 0 ; i < NUM_BLANK_LINES; i++)
		ComRdTerm(g_dev_serial_port, emptyline, 1, 10) ;
	
	ComRdTerm(g_dev_serial_port, header, 85,10) ;
	
	for (i = 0 ; i < DATA_SET ; i++)
		ComRdTerm(g_dev_serial_port, sensordata[i], 85, 10) ;
	

	  
	for (i = 0 ; i < DATA_SET ; i++)
		Scan(sensordata[i], "%s>%i[x]%i[x]%i[x]%i[x]%i[x]%i[x]%i[x]%i[x]%i[x]", &ax[i], &ay[i], &az[i], 
			 &mx[i], &my[i], &mz[i], &gx[i], &gy[i], &gz[i] ) ;
	
	//sum = 0; 
	
	testtime =  Timer() - begin ;
	for (i = 0; i < DATA_SET ; i++)
	{
		sum_ax += ax[i] ;
		sum_ay += ay[i] ;
		sum_az += az[i] ;
		sum_mx += mx[i] ;
		sum_my += my[i] ;
		sum_mz += mz[i] ;
		sum_gx += gx[i] ;
		sum_gy += gy[i] ;
		sum_gz += gz[i] ;
		
	} 	
	
	
	
	// for (i = 0 ; i < DATA_SET ; i++)
	//	sum += ax[i] ;
	
	avg_ax = (double) (sum_ax / DATA_SET) ;
	avg_ay = (double) (sum_ay / DATA_SET) ;  
	avg_az = (double) (sum_az / DATA_SET) ;  
	
	avg_mx = (double) (sum_mx / DATA_SET) ;  
	avg_my = (double) (sum_my / DATA_SET) ;  
	avg_mz = (double) (sum_mz / DATA_SET) ;  
	
	avg_gx = (double) (sum_gx / DATA_SET) ;  
	avg_gy = (double) (sum_gy / DATA_SET) ;  
	avg_gz = (double) (sum_gz / DATA_SET) ;  
	

	
	
	
	report_meas_test_item("Accel X-axis Measurement", avg_ax ,test_limits.accel.x.low ,test_limits.accel.x.high) ; 
	report_meas_test_item("Accel Y-axis Measurement", avg_ay ,test_limits.accel.y.low ,test_limits.accel.y.high) ; 
	report_meas_test_item("Accel Z-axis Measurement", avg_az ,test_limits.accel.z.low ,test_limits.accel.z.high) ; 
	
	report_meas_test_item("Magneto X-axis Measurement", avg_mx ,test_limits.compass.x.low , test_limits.compass.x.high ) ; 
	report_meas_test_item("Magneto Y-axis Measurement", avg_my ,test_limits.compass.y.low , test_limits.compass.y.high ) ; 
	report_meas_test_item("Magneto Z-axis Measurement", avg_mz ,test_limits.compass.z.low , test_limits.compass.z.high ) ; 
	
	report_meas_test_item("Gyro X-axis Measurement", avg_gx ,test_limits.gyro.x.low , test_limits.gyro.x.high) ; 
	report_meas_test_item("Gyro Y-axis Measurement", avg_gy ,test_limits.gyro.y.low , test_limits.gyro.y.high ) ; 
	report_meas_test_item("Gyro Z-axis Measurement", avg_gz ,test_limits.gyro.z.low , test_limits.gyro.z.high ) ; 
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	// InsertTestItem("Compass Test",NULL,NULL,NULL,"Testing..."); 
	
	return SUCCESS ;
}   // sensors_test(void)
