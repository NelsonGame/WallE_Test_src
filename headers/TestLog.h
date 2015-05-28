#ifndef TEST_LOG_H
#define TEST_LOG_H

int InitTestLog(char *serial_num,char *factory_serial_number, int g_module_level_test);
int AddTestLog(char *test_item,char *meas_value,char *low_limit,char *high_limit,char *result);
int CloseTestLog(void);

#endif
