#ifndef SERIAL_DEVICE_H
#define SERIAL_DEVICE_H

int serial_send(int g_port_number,char *data,int len);
int serial_waitfor(int g_port_number,char *prompt,char *data,double g_timeout_value);
int serial_open(int com_port_num,int baud);
int serial_close(int g_port_number);


#endif


