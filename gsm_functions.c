//*****************************************************************************

void gsm_status()
{
   switch(gsm_pwr_status)
   {
      case 0:  
         output_high(GSM_PWR_KEY); 
         delay_ms(2500);
         output_low(GSM_PWR_KEY);                 
         gsm_pwr_status=1;
         gsm_checking=0;
         gsm_startup_wiating_time1=0;
      break;                     
                                     
      case 1:
         if(gsm_startup_wiating_time1>=150)
         {
            gsm_startup_wiating_time1=0;
            gsm_pwr_status=0;
         } 
      break;
      
      case 2:  
         gsm_startup_wiating_time1=0;
         fprintf(GSM_STREAM,"AT\r");
         gsm_pwr_status=3;
      break;
      
      case 3:  
         if(ok_flag==1)
         {          
            ok_flag=0;
            if(gsm_checking==0)
            {
               delete_sms();
            }
            gsm_pwr_status=4;
            gsm_startup_wiating_time1=0;
            //break;
         }
         else if(gsm_startup_wiating_time1>=100)
         {
            gsm_startup_wiating_time1=0;
            gsm_pwr_status=0;
         }
      break;
      
      case 4:
         gsm_pwr_status=4;
         gsm_checking=0;
         if(gsm_startup_wiating_time1>=3000)
         {
            if((stream==0)&&(sms_stage==0))
            {
               gsm_startup_wiating_time1=0;
               gsm_pwr_status=2;
               gsm_checking=1;
            }
         }
      break;
      
   }
}


//*****************************************************************************

//*****************************************************************************
void identify_gsm_data()
{
   //*****************************Gsm Module Start*****************************
   i=0;  g=2;  verify_count=0;            
   for(g=2; g<=11; g++)
   {
      if(gsm[g]==call_ready[i]) 
      { 
         verify_count++; i++;
         if(verify_count==10)  
         {      
            gsm_pwr_status=2;     // GSM-Mod Started
            lcd_gotoxy(0,1);
            printf(lcd_putc,"                ",);
            lcd_gotoxy(0,1);
            printf(lcd_putc,"GSM-Mod Started");
            lcd_gotoxy(0,2);
            printf(lcd_putc,"                 ",);
            display_type=3;
         }
      }
      else break;//g=12;
   }
   
   //**************************GSM Module Power Down***************************
   i=0;g=2;verify_count=0;
   for(g=2; g<=18; g++)
   {
      if(gsm[g]==power_down[i]) 
      { 
         verify_count++; i++;
         if(verify_count==17)
         {
            gsm_pwr_status=0;
            delay_ms(1000);
         }
      }
      else break; //g=19;
   }
   
   //************************************OK************************************
   i=0;g=5;verify_count=0;      
   for(g=5; g<=6; g++)
   {
      if(gsm[g]==ok[i]) 
      { 
         verify_count++; i++;
         if(verify_count==2)
         {
            ok_flag=1;// ok flag must be high.
         }
      } 
      else break; //g=8;
   }
   //***************************SMS SEND VERIFICATION**************************
   i=0;g=2;verify_count=0;
   for(g=2; g<=6; g++)
   {
      if(gsm[g]==send_verify[i]) 
      { 
         verify_count++; i++;
         if(verify_count==5)
         {                          
            if(pwr_sms==3)
            {
               pwr_sms=4;
               sms_stage=0;
               pwr_sms_time=0;
               display_time1=0;
               lcd_gotoxy(0,1);
               printf(lcd_putc,"                ",);
               lcd_gotoxy(0,1); 
               printf(lcd_putc,"PWR SMS Sent");
               lcd_gotoxy(0,2);
               printf(lcd_putc,"                ",);
               display_type=3;
            }
            else if((write_sms_flag==4)&&(acknowledge_flag==0))
            {
               write_sms_flag=0;
               sms_stage=0;
               new_sms_receive_flag=0;
               query_sms_time=0;
               display_time1=0;
               lcd_gotoxy(0,1);
               printf(lcd_putc,"                ",);
               lcd_gotoxy(0,1); 
               printf(lcd_putc,"Query SMS Sent");
               lcd_gotoxy(0,2);
               printf(lcd_putc,"                ",);
               display_type=3;
               
            }
            else if((write_sms_flag==4)&&(acknowledge_flag==1))
            {
               write_sms_flag=0;
               sms_stage=0;
               new_sms_receive_flag=3;
               acknowledge_flag=2;
               query_sms_time=0;
               display_time1=0;
               lcd_gotoxy(0,1);
               printf(lcd_putc,"                ",);
               lcd_gotoxy(0,1); 
               printf(lcd_putc,"Acknow1 SMS Sent");
               lcd_gotoxy(0,2);
               printf(lcd_putc,"                ",);
               display_type=3;
               
            }
            else if((write_sms_flag==4)&&(acknowledge_flag==2))
            {
               write_sms_flag=0;
               sms_stage=0;
               new_sms_receive_flag=0;
               acknowledge_flag=0;
               query_sms_time=0;
               display_time1=0;
               lcd_gotoxy(0,1);
               printf(lcd_putc,"                ",);
               lcd_gotoxy(0,1); 
               printf(lcd_putc,"Acknow2 SMS Sent");
               lcd_gotoxy(0,2);
               printf(lcd_putc,"                ",);
               display_type=3;
               
            }
            else
            {
               send_sms_flag=3;
               sms_stage=0;
               sms_resend_time1=0;
               display_time1=0;
               lcd_gotoxy(0,1);
               printf(lcd_putc,"                ",);
               lcd_gotoxy(0,1);
               printf(lcd_putc,"SMS Sent");
               lcd_gotoxy(0,2);
               printf(lcd_putc,"                ",);
               display_type=3;
            }
         }
      }
      else  break;//g=7;
      //i++;
   }
   //**********************************New Sms*********************************  
   i=0;verify_count=0;
   for(g=2; g<=6; g++)
   {      
      if(gsm[g]==new_sms[i]) 
      {           
         verify_count++; i++;
         if(verify_count==5)
         {
            find_sms_index();
            new_sms_receive_flag=1;
         }                        
      }
      else  break;//g=7;           
   }
   
   //*********************************RING DROP********************************
   verify_count=0;i=0;g=0;
   for(g=2; g<=5; g++)
   {
      if(gsm[g]==ring[i]) 
      { 
         verify_count++; i=0;
         if(verify_count==4)
         {  
            fprintf(GSM_STREAM,"ATH0\r");
         }
      }
      else  break;//g=6;
      //i++; 
   }
   verify_count=0;  g=0;
}
//*****************************************************************************

//**********************function to find sms index (0-30)**********************
void find_sms_index()
{
   new_sms_count1=0; //For Trial
   if(gsm[15]==0x0D)//index is less than 10
   {
      new_sms_count1=(TOINT(gsm[14]));
      g1_num=33; g2_num=45;   //Boundry of Sms Sender's Number.
      g1_text=75; g2_text=80; //"Report" Boundries
      a1_text=75; a2_text=83; //"Authorize" Boundries
      //a1_num=85; a2_num=97;   //Boundries of Received Number to be set as Reciepent of Reports & Warnings
   }                                      
   else if(gsm[16]==0x0D)//index is greater than 10 
   {
      new_sms_count1=(((TOINT(gsm[14]))*10) + TOINT(gsm[15]));
      g1_num=34; g2_num=46;   //Boundry of Sms Sender's Number.
      g1_text=76; g2_text=81; //"Report" Boundries
      a1_text=76; a2_text=84; //"Authorize" Boundries
      //a1_num=86; a2_num=98;   //Boundries of Received Number to be set as Reciepent of Reports & Warnings
   }
   
   lcd_gotoxy(0,1);
   printf(lcd_putc,"SMS #: %u        ",new_sms_count1);
   lcd_gotoxy(0,2);
   printf(lcd_putc,"                 ",);
   delay_ms(1000);  
}

//*****************************************************************************
                                                                               
//****************************function to delete sms*************************** 
void delete_sms()
{
   lcd_gotoxy(0,1);
   printf(lcd_putc," Deleting Inbox ");
   lcd_gotoxy(0,2);
   printf(lcd_putc,"                 ",);
   for(i=1; i<=30; i++)
   {                
       fprintf(GSM_STREAM,"AT+CMGD=%u\r",i);
       delay_ms(400);
   }
   i=0;
   printf(lcd_putc," Inbox Deleted  ");
   lcd_gotoxy(0,2);
   printf(lcd_putc,"                 ",);
   display_type=3;
} 
//*****************************************************************************

void send_query_sms()
{
   if(new_sms_receive_flag==0)
   {
      query_sms_time=0;
   }
   else if(new_sms_receive_flag==1)
   {  
      if(stream==0)
      {
         if(sms_stage==0)
         {
            
            g=0;
            sms_stage=1;
            fprintf(GSM_STREAM,"AT+CMGR=%u\r",new_sms_count1);
            new_sms_receive_flag=2;
         }  //if(sms_stage==0)
      }  //if(stream==0)
   }
   else if(new_sms_receive_flag==3)
   {
      if(stream==0)
      {
         if(sms_stage==0)
         {
            if(num_format==1)
            {
               if(acknowledge_flag<2)
               {
                  sms_stage=1;
                  for(i=0;i<=23;i++)
                  {
                     fprintf(GSM_STREAM,"%c",auth_client[i]);
                     if(i==23)
                     { 
                        g=0;
                        write_sms_flag=2;
                     }
                  }
               }
               else if(acknowledge_flag==2)  //Sends Acknowledgment msg to AEI
               {
                  sms_stage=1;
//!                  fprintf(GSM_STREAM,"AT+CMGS=\"+923312107273\"\r");
                  fprintf(GSM_STREAM,"AT+CMGS=\"+923363111563\"\r");
                  g=0;
                  write_sms_flag=2;
               }
               new_sms_receive_flag=4;
               query_sms_time=0;
            }
         }  //if(sms_stage==0)
      }  //if(stream==0)
      
   }
   else if(new_sms_receive_flag==4)
   {
      if(query_sms_time>=100)
      {
         sms_stage=0;
         new_sms_receive_flag=1;
         query_sms_time=0;
      }
   }

}
