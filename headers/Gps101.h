#ifndef GPS_101_H
#define GPS_101_H

int init_gps_sim(int serial_port_num);
int close_gps_sim(void);

int gps_sim_set_pwr(double power);
int gps_sim_set_sat(int sat_num);
//int gps_sim_set_output(int output);
int gps_sim_set_output(char* output);  
int gps_sim_set_dplr(int doppler);
int gps_sim_set_test(int testNum);

#endif
