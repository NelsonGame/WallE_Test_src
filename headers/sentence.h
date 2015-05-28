
#ifndef __NMEA_SENTENCE_H__
#define __NMEA_SENTENCE_H__

typedef struct _nmeaGPGSV
{
    int     sentence_count; /**< Total number of messages of this type in this cycle */
    int     sentence_index; /**< Message number */
    int     sat_count;  /**< Total number of satellites in view */
	
}   nmea_GSV ;

/***************************
 typedef struct SVdata
{								  
	int vid ;
	int cn ; 
    
}   SV ;
 *********************/
#define MAX_SENTENCE  6
#define MAX_SAT 20

#endif

