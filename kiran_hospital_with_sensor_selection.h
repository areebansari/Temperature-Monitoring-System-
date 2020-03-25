//!#include <18f26k22.h>
//!#device adc=10
#include <18f252.h>
//!#device PASS_STRINGS=IN_RAM
#device adc=10
#fuses PUT,NOWDT,HS,NOLVP,PROTECT
#use delay(CRYSTAL=20M)                                     

#use rs232(baud=19200, xmit=PIN_C6, rcv=PIN_C7, stream=GSM_STREAM)
//!#use rs232(baud=19200, xmit=PIN_B6, rcv=PIN_B7, stream=PORT1)

#include <stdlib.h>
#include <string.h>
#include "flex_lcd_16.c"

#define menu_scroll                PIN_C0 // To be changed.
#define parameter_function_input   PIN_C3

//!#define GREEN_LED    PIN_C1
#define RELAY        PIN_C2   // output of Relay
#define RED_LED      PIN_A4
#define MUTE         PIN_C1   // Input of Mute Push Button
#define AC_PWR_KEY   PIN_C4   // Input of AC-Power to be monitored if fails
#define GSM_PWR_KEY  PIN_C5   // output of Relay.

#define FASTER_BUT_MORE_ROM
#define TOINT(C) (C-'0')

#define  ADC_FETCHING_TIME  3                            


//**********************************Variables**********************************
int  t1_flag=1, t2_flag=1, t3_flag=1, t4_flag=1, gsm_flag=0, relay_flag=1;

int   gsm_pwr_status=0,gsm_pwr_try=0,new_sms_receive_flag=0,verify_count=0,i=0,
      new_sms_count1=0,write_sms_flag=0,condition=0,send_sms_flag=1,                    
      sms_resend=0,send_sms_count=0,mute_flag=0,sms_stage=0,temp_high=0,temp_low=0,          
      display_time1=0,display_time2=0,display_type=1,pwr_sms=0,prv_g=0,                       
      pwr_sms_send_count1=0,g=0,stream=0,pwr_flag=0,g1_num=0,g2_num=0,
      g1_text=0,g2_text=0,a1_text=0,a2_text=0,a1_num=0,a2_num=0,
      pwr_sms_time=0, pwr_condition=0, num_format=0, acknowledge_flag=0,
      report_confirm_flag=0,authrize_confirm_flag=0,a_loop=0,adc_flag=0,
      high_diff=0,low_diff=0,same=0,new_value=0,adc_time=0,relay_time=0,
      relay_op1=0,relay_op2=0,relay_op3=0,relay_op4=0,gsm_checking=0,
      menu_scroll_count=0,menu_time=0;
      
float low_temp=1,       // Lower Temperature Range; Below which Alarm is raised
      high_temp=9.5;    // Higher Temperature Range; Beyond which Alarm is raised

long  temp_stable_time=9000; //check if condition persist upto 15 mins. 10= 1 sec, 600=1 min, 
      
long  gsm_startup_wiating_time1=0,gsm_startup_wiating_time2=0,av_1=0,e=0,
      sms_resend_time1=0,query_sms_time=0,t1_check_time=0,t2_check_time=0,
      t3_check_time=0,t4_check_time=0;
                                                                            
float lm35_one=0, lm35_two=0, lm35_three=0, lm35_four=0, adc_to_temp_f1=0,
      adc_to_temp_f2=0,f_1=0,f_2=0,f_5=0;

short z=1,prv_z=0,ok_flag=0;
                                                                     
char ok[2]={'O','K'};
char ring[4]={'R','I','N','G'};
char new_sms[5]={'+','C','M','T','I'};
char send_verify[5]={'+','C','M','G','S'};
char auth_text[6]={'R','e','p','o','r','t'};
char call_ready[10]={'C','a','l','l',' ','R','e','a','d','y'};
//!char auth_num[13]={'+','9','2','3','3','1','2','1','0','7','2','7','3'};
char auth_num[13]={'+','9','2','3','3','6','3','1','1','1','5','6','3'};
char auth_client[24]={'A','T','+','C','M','G','S','=','"','+','9','2','3','3',
                     '1','2','1','0','7','2','7','3','"','\r'};
//char auth_client[13]={0};
char auth_text2[9]={'A','u','t','h','o','r','i','z','e'};
char power_down[17]={'N','O','R','M','A','L',' ','P','O','W','E','R',' ',
                     'D','O','W','N'};

char sms[200]={0},sms2[200]={0},sms3[200]={0},sms4[200]={0},gsm[200]={0},
     ctrl_z=0x1A;
//*****************************************************************************

//**********************************Functions**********************************
void gsm_status(void);  // Turns On, and monitors working of GSM Module.
void delete_sms(void);  // Delete all text messages from inbox.
void find_sms_index(void); // It saves incoming Text number.      
void identify_gsm_data(void); //Monitors any data send by GSM-Mod to controller

void scan_ad_ch(void);  //Scans analogue channels
void measure_adc_values(void);//It measure adc value.
void adc_to_temp_conversion(void);//converts adc value to temperature.

void temp_check_and_decision(void);//Monitors temperature and take decision
void condition_function(void);//Raises Alarm and sent sms.

void power_status_check(void);//Monitors Power Failur & Reconvery.
void power_sms_condition(void);//Generates sms on either condition.

void send_query_sms(void);//sends current temperature report in response to a query.

void show_ad_screens(void);//Displays temperature and notifications.

void menu_input_sense(void);// Senses push buttons to active/deactive a sensor, GSM-Module, Relay etc.
void menu_screens(void);
void menu_parameter_function(void);

//*****************************************************************************
