#ifndef POWER_SUPPLY_H

#define POWER_SUPPLY_H


#define OUTPUT_ON 1
#define OUTPUT_OFF 0

// #define BL_DIS_BIT 1

//int init_power_supply(int comport,int gpib_address);
int init_power_supply(int gpib_address); 
void close_power_supply(void);

int set_voltage(double voltage);
int set_current(double current);
int meas_voltage(double *voltage);
int meas_current(double *voltage);

int set_output(int output);

int send_ps_command(char *command,char *response);

// int ps_enable_bootloader(int enable);

#endif
