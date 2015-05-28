#ifndef DEVCOMM_H
#define DEVCOMM_H

// int DetectUsbDevice(char *SerialNumber);

int send_dev_cmd(char *cmd);
int get_dev_response(char *response);
int set_dev_timeout(int timeout);
int init_dev(void);
int close_dev(void);

int get_dev_com_port(int *p_com_port_num);
int get_dev_line(char *data);
void flush_dev(void);

#endif

