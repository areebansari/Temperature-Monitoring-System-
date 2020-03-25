//*********************Function to show parameters on LCD*********************
void show_ad_screens()
{
   switch(display_type)
   {
      case 1:
      if(display_time1>=2)
      {
         display_time1=0;
         lcd_gotoxy(0,1);
         printf(lcd_putc,"                ",);
         
         lcd_gotoxy(0,1);
         if(t1_flag) printf(lcd_putc,"1:%f ",lm35_one);
         else  printf(lcd_putc,"1:D.Act ");
         
         lcd_gotoxy(9,1);
         if(t2_flag) printf(lcd_putc,"2:%f",lm35_two);
         else  printf(lcd_putc,"2:D.Act ");
         
         lcd_gotoxy(0,2);
         printf(lcd_putc,"                ",);
         
         lcd_gotoxy(0,2);
         if(t3_flag) printf(lcd_putc,"3:%f ",lm35_three);
         else  printf(lcd_putc,"3:D.Act ");
         
         lcd_gotoxy(9,2);
         if(t4_flag) printf(lcd_putc,"4:%f",lm35_four);
         else  printf(lcd_putc,"4:D.Act ");
         
         display_time2++;
         
         if(display_time2>=15)   
         { 
            display_time2=0;
            if(pwr_flag==1)
            {  
               display_time1=0;
               lcd_gotoxy(0,1);
               printf(lcd_putc,"                ",);
               lcd_gotoxy(0,1);                 //trial purpose
               printf(lcd_putc,"  Power Failed  ");
               
               lcd_gotoxy(0,2);
               printf(lcd_putc,"                ",);
               display_type=3;
            }
            else  display_type=1;
         }
      }
      break;
          
      case 3:// Delay to display Random Notifications.
      if(display_time1>=3)
      {
         display_time1=0;
         display_time2++;
         if(display_time2>=8)
         { 
            display_time2=0; 
            display_type=1;
         }
      }
      break;
   }
}

//***************************Function to check Power***************************
void power_status_check()
{
   
   if(input(AC_PWR_KEY))   //WHEN AC POWER IS OFF
   {
      pwr_flag=1;
      if(pwr_sms_send_count1==0)
      {  
         power_sms_condition();
      }
      else if(pwr_sms_send_count1==1)
      {
         pwr_sms=0;
         pwr_condition=0;
         pwr_sms_time=0;
      }
   }
   
   else //WHEN AC POWER IS ON
   {
      pwr_flag=0;
      if(pwr_sms_send_count1==1)
      {  
         power_sms_condition();
      }
      else if(pwr_sms_send_count1==0)
      {
         pwr_sms=0;
         pwr_condition=0;
         pwr_sms_time=0;
         pwr_sms_send_count1=0;
      }
   }
}
//*****************************************************************************

void power_sms_condition(void)
{
   if(gsm_pwr_status==4)
   {
      if(pwr_condition==0)
      {
         pwr_condition=1;
         pwr_sms_time=0;
      }
      else if(pwr_condition==1)
      {
         if(pwr_sms_time>=50)
         {
            pwr_sms_time=0;
            pwr_condition=2;
            pwr_sms=0;
         }                     
      }   
      else if(pwr_condition==2)
      {
         if((stream==0)&&(sms_stage==0)&&(pwr_sms==0)&&(num_format==1))
         { 
            pwr_sms_time=0;
            sms_stage=1;
            g=0;
            for(i=0;i<=23;i++)
            {
               fprintf(GSM_STREAM,"%c",auth_client[i]);
               if(i==23){ g=0; pwr_sms=1;  }
            }
            
         }
         else if(pwr_sms==3)
         {
            if(pwr_sms_time>=100)
            {
               pwr_sms_time=0;
               pwr_sms=0;
               sms_stage=0;
               pwr_sms_send_count1=0;
            }
         }
         else if(pwr_sms==4)
         {
            if(pwr_flag==0)
            {
               pwr_sms_send_count1=0;
            }
            else if(pwr_flag==1)
            {
               pwr_sms_send_count1=1;
            }
         }
      }
   }
}
