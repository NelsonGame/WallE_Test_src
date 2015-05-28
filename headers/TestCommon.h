

#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#define SUCCESS 0
#define FAILURE 1

/****************
#define PASS 0
#define FAIL 1

*****************/

#define PASS 1
#define FAIL 0

#define YES 1
#define NO 0

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


#define GPS_TEST_CMD		"$AMT,GPS,1*00\r\n" 
#define SENSORS_TEST_CMD	"$AMT,SENSOR,100*00\r\n" 
#define SOUNDER_TEST_CMD	"$AMT,OBJECT,SOUNDER,TEST*00\r\n" 
#define PIMIC_TEST_CMD		"$AMT,OBJECT,POWER*00\r\n" 
#define MFI_TEST_CMD		"$AMT,OBJECT,MFI*00\r\n" 
#define CR 13
#define LF 10

extern long long pcba_id ;
extern long long module_id ;
extern char TestDate[15] ;
extern char TestTime[10] ;
extern int g_dev_serial_port ; 


int UpdateTestItem(int test_item,char *test_name,char *meas_val,char *low_limit,char *high_limit,char *result);
int InsertTestItem(char *test_name,char *meas_val,char *low_limit,char *high_limit,char *result);

int report_meas_test_item(char *test_name,double dbl_meas_value,double dbl_low_limit,double dbl_high_limit);
void display_sernum(void);

char *get_chip_id_string(void);

extern int g_abort_test_flag;

void SetStatusLine(char *text);

int ConfirmMessage(char *title,char *message);
int ShowMessage(char *title,char *message);

int Insert_step_char(char *test_name, char *meas_val, char *low_limit, char *high_limit, float testtime, char *result) ;
int Insert_step(char *test_name, double meas_val, double low_limit, double high_limit, float testtime, char *result) ; 
									
/*************************************************/
/***** function calls for testing purpose  ******/
/************************************************/

int gas_gauge_read(void) ; 
int MFI_test(void) ; 
int nmea_parser(void) ; 
int pwr_btn_test(void) ; 

int reset_test(void) ;   
int main_on_sig_test(void); 
char* getTime(void);

char* getDate(void) ;
void write_log_data(char *data) ;

#endif
