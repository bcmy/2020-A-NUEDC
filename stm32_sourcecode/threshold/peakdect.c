
#include "peakdect.h"

unsigned int sample_count = 0 ;
int QRS_Second_Prev_Sample = 0 ;
int QRS_Prev_Sample = 0 ;
int QRS_Current_Sample = 0 ;
int QRS_Next_Sample = 0 ;
int QRS_Second_Next_Sample = 0 ;

uint16_t QRS_B4_Buffer_ptr = 0 ;
int16_t QRS_Threshold_Old = 0;
int16_t QRS_Threshold_New = 0;

unsigned char first_peak_detect = FALSE ;

volatile uint16_t QRS_Heart_Rate = 0 ;

unsigned int sample_index[MAX_PEAK_TO_SEARCH + 2] ;
unsigned char Start_Sample_Count_Flag = 0;

volatile uint8_t global_HeartRate = 0;

void QRS_Algorithm_Interface(int16_t CurrSample)
{
  //  static FILE *fp = fopen("ecgData.txt", "w");
  static int16_t prev_data[128] = {0};
  int16_t i;
  long Mac = 0;
  prev_data[0] = CurrSample;

  for ( i = 127; i > 0; i--)
  {
    Mac += prev_data[i];
    prev_data[i] = prev_data[i - 1];

  }
  Mac += CurrSample;
  Mac = Mac >> 2;
  CurrSample = (int16_t) Mac;
  QRS_Second_Prev_Sample = QRS_Prev_Sample ;
  QRS_Prev_Sample = QRS_Current_Sample ;
  QRS_Current_Sample = QRS_Next_Sample ;
  QRS_Next_Sample = QRS_Second_Next_Sample ;
  QRS_Second_Next_Sample = CurrSample ;



  QRS_process_buffer();
}

static void QRS_process_buffer(void)
{

  int16_t first_derivative = 0 ;
  int16_t scaled_result = 0 ;

  static int16_t Max = 0 ;

  /* calculating first derivative*/
  first_derivative = QRS_Next_Sample - QRS_Prev_Sample  ;

//printf("%d\n",first_derivative);
  /*taking the absolute value*/

  if (first_derivative < 0)
  {
    first_derivative = -(first_derivative);
  }


  scaled_result = first_derivative;
	
  if ( scaled_result > Max )
  {
    Max = scaled_result ;
  }


  QRS_B4_Buffer_ptr++;
  if (QRS_B4_Buffer_ptr ==  TWO_SEC_SAMPLES)
  {
    QRS_Threshold_Old = ((Max * 7) / 10 ) ;
    QRS_Threshold_New = QRS_Threshold_Old ;
   // if (Max > 70)
      first_peak_detect = TRUE ;
    Max = 0;
 
    //  ecg_wave_buff[0] = first_derivative;
    QRS_B4_Buffer_ptr = 0;


  }


  if ( TRUE == first_peak_detect )       //如果检测到第一个尖峰
  {
    QRS_check_sample_crossing_threshold( scaled_result ) ;
  }


}





static void QRS_check_sample_crossing_threshold( uint16_t scaled_result )
{
  /* array to hold the sample indexes S1,S2,S3 etc */

  static uint16_t s_array_index = 0 ;
  static uint16_t m_array_index = 0 ;

  static unsigned char threshold_crossed = FALSE ;
  static uint16_t maxima_search = 0 ;
  static unsigned char peak_detected = FALSE ;
  static uint16_t skip_window = 0 ;
  static long maxima_sum = 0 ;
  static unsigned int peak = 0;
  static unsigned int sample_sum = 0;
  static unsigned int nopeak = 0;
  uint16_t Max = 0 ;
  uint16_t HRAvg;
  uint16_t  RRinterval = 0;

  if ( TRUE == threshold_crossed  )
  {
    /*
    Once the sample value crosses the threshold check for the
    maxima value till MAXIMA_SEARCH_WINDOW samples are received
    */
    sample_count ++ ;        //采样值计数+1
    maxima_search ++ ;

    if ( scaled_result > peak )
    {
      peak = scaled_result ;
    }

    if ( maxima_search >= MAXIMA_SEARCH_WINDOW )
    {
      // Store the maxima values for each peak
      maxima_sum += peak ;
      maxima_search = 0 ;


      threshold_crossed = FALSE ;
      peak_detected = TRUE ;
    }

  }
  else if ( TRUE == peak_detected )
  {
    /*
    Once the sample value goes below the threshold
    skip the samples untill the SKIP WINDOW criteria is meet
    */
    sample_count ++ ;
    skip_window ++ ;

    if ( skip_window >= MINIMUM_SKIP_WINDOW )
    {
      skip_window = 0 ;
      peak_detected = FALSE ;
    }

    if ( m_array_index == MAX_PEAK_TO_SEARCH )
    {
      sample_sum = sample_sum / (MAX_PEAK_TO_SEARCH - 1);
      HRAvg =  (uint16_t) sample_sum  ;

      // Compute HR without checking LeadOffStatus
      QRS_Heart_Rate = (uint16_t) 60 *  SAMPLING_RATE;
      QRS_Heart_Rate =  QRS_Heart_Rate / HRAvg ;
      if (QRS_Heart_Rate > 250)
        QRS_Heart_Rate = 250 ;
 //     Serial.println(QRS_Heart_Rate);

      /* Setting the Current HR value in the ECG_Info structure*/
      maxima_sum =  maxima_sum / MAX_PEAK_TO_SEARCH;
      Max = (int16_t) maxima_sum ;
      /*  calculating the new QRS_Threshold based on the maxima obtained in 4 peaks */
      maxima_sum = Max * 7;
      maxima_sum = maxima_sum / 10;
      QRS_Threshold_New = (int16_t)maxima_sum;

      /* Limiting the QRS Threshold to be in the permissible range*/
      if (QRS_Threshold_New > (4 * QRS_Threshold_Old))
      {
        QRS_Threshold_New = QRS_Threshold_Old;
      }

      sample_count = 0 ;
      s_array_index = 0 ;
      m_array_index = 0 ;
      maxima_sum = 0 ;
      sample_index[0] = 0 ;
      sample_index[1] = 0 ;
      sample_index[2] = 0 ;
      sample_index[3] = 0 ;
      Start_Sample_Count_Flag = 0;

      sample_sum = 0;
    }
  }
  else if ( scaled_result > QRS_Threshold_New )
  {
    /*
      If the sample value crosses the threshold then store the sample index
    */
    Start_Sample_Count_Flag = 1;
    sample_count ++ ;
    m_array_index++;
    threshold_crossed = TRUE ;
    peak = scaled_result ;
    nopeak = 0;

    /*  storing sample index*/
    sample_index[ s_array_index ] = sample_count ;
    if ( s_array_index >= 1 )
    {
      sample_sum += sample_index[ s_array_index ] - sample_index[ s_array_index - 1 ] ;
    }
    s_array_index ++ ;
  }

  else if (( scaled_result < QRS_Threshold_New ) && (Start_Sample_Count_Flag == 1))
  {
    sample_count ++ ;
    nopeak++;
    if (nopeak > (3 * SAMPLING_RATE))
    {
      sample_count = 0 ;
      s_array_index = 0 ;
      m_array_index = 0 ;
      maxima_sum = 0 ;
      sample_index[0] = 0 ;
      sample_index[1] = 0 ;
      sample_index[2] = 0 ;
      sample_index[3] = 0 ;
      Start_Sample_Count_Flag = 0;
      peak_detected = FALSE ;
      sample_sum = 0;

      first_peak_detect = FALSE;
      nopeak = 0;

      QRS_Heart_Rate = 0;

    }
  }
  else
  {
    nopeak++;
    if (nopeak > (3 * SAMPLING_RATE))
    {
      /* Reset heart rate computation sate variable in case of no peak found in 3 seconds */
      sample_count = 0 ;
      s_array_index = 0 ;
      m_array_index = 0 ;
      maxima_sum = 0 ;
      sample_index[0] = 0 ;
      sample_index[1] = 0 ;
      sample_index[2] = 0 ;
      sample_index[3] = 0 ;
      Start_Sample_Count_Flag = 0;
      peak_detected = FALSE ;
      sample_sum = 0;
      first_peak_detect = FALSE;
      nopeak = 0;
      QRS_Heart_Rate = 0;
    }
  }

  global_HeartRate = (uint8_t)QRS_Heart_Rate;
  //Serial.println(global_HeartRate);

}


