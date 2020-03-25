//*********************Interrupt to capture GSM Module Data********************
#int_RDA
void RDA_isr(void)                                    
{
   
   if(g>199)   g=0;
     
   if(new_sms_receive_flag==2)      
   {
      sms[g]=fgetc(GSM_STREAM);                 
      prv_g=g;                                      
   }
   
   else if(send_sms_flag==2)
   { 
      sms2[g]=fgetc(GSM_STREAM);
      prv_g=g;
   }
   
   else if(pwr_sms==1)
   {
      sms3[g]=fgetc(GSM_STREAM);
      prv_g=g;
      if(sms3[g]=='>') { pwr_sms=2; }
   }
   
   else if(write_sms_flag==2)
   {
      sms4[g]=fgetc(GSM_STREAM);
      prv_g=g;
      if(sms4[g]=='>') { write_sms_flag=3;  }
   } 
   
   else                      
   {                        
      gsm[g]=fgetc(GSM_STREAM);
      prv_g=g;
   }
   
   g++;
   z=~z;
   stream=1;
} 
//**************************************************************************

                                                      
//!#int_TIMER0
//!void  TIMER0_isr(void) 
//!{

//!   sms_resend_time1++;
//!  
//!   set_timer0(26474);
//!}
//***************************************************************************** 

     
#int_TIMER1                                                                   
void  TIMER1_isr(void) 
{
   if((pwr_flag==1)||(relay_flag==FALSE))
   {
      output_low(RELAY);
   }
   else
   {
      if(mute_flag==1)
      {
         if(relay_time<=20)
         {
            output_high(RELAY);// Turn Relay ON
         }
         else if((relay_time>20)&&(relay_time<=80))
         {
            output_low(RELAY);// Turn Relay OFF
            if(relay_time>=80)
            {
               relay_time=0;
            }
         }
         if(!input(MUTE))
         {
            mute_flag=2;
            output_low(RELAY);// Turn Relay OFF
            while(!input(MUTE));
         }
      }
   }
   
   //**************************
   if(adc_flag==1)
   {
      adc_time++;
      if(adc_time>=ADC_FETCHING_TIME)
      {  
         adc_flag=0;
         adc_time=0;      
      }
   }
   
   display_time1++;
   gsm_startup_wiating_time1++;
   pwr_sms_time++;
   query_sms_time++;
   relay_time++;
   
   t1_check_time++;
   t2_check_time++;
   t3_check_time++;
   t4_check_time++;
   
   if(menu_scroll_count)   menu_time++;
   
   sms_resend_time1++;
   
   set_timer1(3036);
}
//*****************************************************************************
