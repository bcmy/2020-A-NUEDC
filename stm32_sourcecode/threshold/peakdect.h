#ifndef __PEAKDETECT_H__
#define __PEAKDETECT_H__
#include "stdint.h"
#define FALSE 0
#define TRUE 1
#define MAXIMA_SEARCH_WINDOW 125
#define MINIMUM_SKIP_WINDOW  130
#define MAX_PEAK_TO_SEARCH   5

#define SAMPLING_RATE             500
#define TWO_SEC_SAMPLES           2 * SAMPLING_RATE
#define QRS_THRESHOLD_FRACTION    0.4


void QRS_Algorithm_Interface(int16_t CurrSample);
void QRS_process_buffer();
void QRS_check_sample_crossing_threshold( uint16_t scaled_result );



#endif