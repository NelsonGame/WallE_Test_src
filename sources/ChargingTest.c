#include <userint.h>
#include "ansi_c.h"
#include "TestCommon.h"
#include "ChargingTest.h"
#include "DevComm.h"
#include "PowerSupply.h"
#include "TestItems.h"


int test_charging(void)
{
	double current;
	
	meas_current(&current);
	
	report_meas_test_item("Charging Current",current, test_limits.charging_test.charge_current.low,test_limits.charging_test.charge_current.high);
	
	
	return SUCCESS;
}
