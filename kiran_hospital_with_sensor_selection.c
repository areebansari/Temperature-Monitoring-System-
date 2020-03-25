//*****************************************************************************
//********************************KIRAN HOSPITAL*******************************
//***************************TEMPERATURE CONTROLLER****************************
//******************************ADC & GSM ROUTINE******************************
//*****************************************************************************
#include "kiran_hospital_with_sensor_selection.h"
#include "interrupts.c"                      
#include "gsm_functions.c"                                  
#include "adc_functions.c"                             
#include "lcd_and_power_routines.c"    
#include "menu_functions.c"

//*********************************MAIN ROUTINE********************************
void main()
{
//!   setup_adc_ports(sAN0|sAN1|sAN2|sAN3);  
   setup_adc_ports(ALL_ANALOG);
//!   setup_adc_ports(AN0);
   setup_adc(ADC_CLOCK_INTERNAL);
   output_low(RELAY);                  
   
//!   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_128);
//!   set_timer0(26474);
//!   enable_interrupts(INT_RTCC);
      
   enable_interrupts(INT_RDA);
   
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
   set_timer1(3036);
   enable_interrupts(INT_TIMER1);                          
   
   enable_interrupts(GLOBAL);
   
   stream=0;
   pwr_sms=0;
   pwr_condition=0;
   pwr_sms_time=0;
   pwr_sms_send_count1=0;
   
   lcd_init();
   delay_ms(10);
   
//********** Acquiring data from EEPROM Routine ************

//!   //Acquiring Sensors, GSM & Alarm Settings
   t1_flag = read_eeprom(0);
   t2_flag = read_eeprom(1);
   t3_flag = read_eeprom(2);
   t4_flag = read_eeprom(3);
   gsm_flag = read_eeprom(4);
   relay_flag = read_eeprom(5);
//!   
//!   //********************************************************
   for(i=9; i<=21; i++)
   {
      auth_client[i]=read_eeprom(i);
      if((auth_client[i]=='+')||(auth_client[i]=='9')||(auth_client[i]=='8')
      ||(auth_client[i]=='7')||(auth_client[i]=='6')||(auth_client[i]=='5')
      ||(auth_client[i]=='4')||(auth_client[i]=='3')||(auth_client[i]=='2')
      ||(auth_client[i]=='1')||(auth_client[i]=='0'))
      {
         num_format=1;
      }
      else
      {
         num_format=0;//number format
         i=0;
         display_time1=0;
         lcd_gotoxy(0,1);
         printf(lcd_putc,"Number Not saved");
         lcd_gotoxy(0,2);
         printf(lcd_putc,"                 ",);
         delay_ms(2000);
         //display_type=3;
         break;
      }
   } 
//********************************************************   
   while(TRUE)
   {      
      power_status_check();      
      if(gsm_flag)  gsm_status();
      scan_ad_ch();
      menu_input_sense();
      if(!menu_scroll_count)     show_ad_screens();
      while(stream==1)
      {                                    
         if(prv_z==z)
         {
            e++;
            if(e==25000) //25000 (done)
            {  
               e=0;
               if(pwr_sms==2)
               {
                  if(pwr_flag==1)
                  {
                     fprintf(GSM_STREAM,"System's Power Failed\r\n");
                     
                  }                    
                  else if(pwr_flag==0) //else if(pwr_type==2)
                  {
                     fprintf(GSM_STREAM,"System's Power Recovered\r\n");
                  }
                  
                  fprintf(GSM_STREAM,"%c",ctrl_z);
                  g=0;
                  pwr_sms=3;
                  pwr_sms_time=0;
               }//if(pwr_sms==1) 
               
               //**********************************************************
               if(send_sms_flag==2)
               {
                  for(g=0; g<=prv_g; g++)
                  {
                     if(sms2[g]=='>')
                     {
                        fprintf(GSM_STREAM,"TEMPERATURE WARNING\r\n");
                        if(temp_low==1)
                        {
                           if(lm35_one<2.5)
                           {
                              fprintf(GSM_STREAM,"T1: VERY LOW\r\n");
                           }
                           if(lm35_two<2.5)
                           {
                              fprintf(GSM_STREAM,"T2: VERY LOW\r\n");
                           }
                           if(lm35_three<2.5)
                           {
                              fprintf(GSM_STREAM,"T3: VERY LOW\r\n");
                           }
                           if(lm35_four<2.5)
                           {
                              fprintf(GSM_STREAM,"T4: VERY LOW\r\n");
                           }
                           
                           temp_low=0;
                        }
                        if(temp_high==1)
                        {
                           if(lm35_one>9.5)
                           {
                              fprintf(GSM_STREAM,"T1: VERY HIGH\r\n");
                           }
                           if(lm35_two>9.5)
                           {
                              fprintf(GSM_STREAM,"T2: VERY HIGH\r\n");
                           }
                           if(lm35_three>9.5)
                           {
                              fprintf(GSM_STREAM,"T3: VERY HIGH\r\n");
                           }
                           if(lm35_four>9.5)
                           {
                              fprintf(GSM_STREAM,"T4: VERY HIGH\r\n");
                           }
                           
                           temp_high=0;
                        }
                        fprintf(GSM_STREAM,"%c",ctrl_z);
                        sms_resend_time1=0;
                        break;
                     }         
                  }             
                  //enable_interrupts(INT_RTCC);
                  send_sms_flag=0;
                  for(g=0; g<=199; g++)
                  {
                     sms2[g]=0;
                  }
                  
                  //enable_interrupts(INT_RTCC);
               }//if(send_sms_flag==2)

//*****************************************************************************
               
               if(new_sms_receive_flag==2)
               { 
                  i=0; verify_count=0;
                  for(g=g1_text; g<=g2_text; g++)  //If "Report" Received
                  {
                     if(sms[g]==auth_text[i])
                     {
                        verify_count++; i++;
                        if(verify_count==6)
                        { 
                           report_confirm_flag=1;
                        }
                     }  //If
                     else 
                     { 
                        break;                           
                     }//Else
                  }  //For Loop
                  
                  i=0; verify_count=0;
                  for(g=a1_text; g<=a2_text; g++)  //If "Authorize" Text Scan
                  {
                     if(sms[g]==auth_text2[i])
                     {
                        verify_count++; i++;
                        if(verify_count==9)
                        {
                           authrize_confirm_flag=1;
                        }
                     }
                     else
                     {
                        i=0; verify_count=0;
                        sms_stage=0;
                        new_sms_receive_flag=0;
                        break;
                     }
                  }
                  //********************************************************
                  if(report_confirm_flag==1)
                  {
                     report_confirm_flag=0;
                     i=9; //i=0;
                     verify_count=0;
                     for(g=g1_num; g<=g2_num; g++)
                     {
                        if(sms[g]==auth_client[i])
                        {
                           verify_count++; i++;        
                           if(verify_count==13)
                           {
                              i=0; verify_count=0;
                              new_sms_receive_flag=3;
                              sms_stage=0;
                              Break;
                           }
                        }
                        Else
                        {
                           i=0; verify_count=0;
                           new_sms_receive_flag=0;
                           sms_stage=0;
                           Break;
                        }
                     }
                  }
                  else if(authrize_confirm_flag==1)
                  {
                     authrize_confirm_flag=0;
                     i=0;verify_count=0;
                     for(g=g1_num; g<=g2_num; g++)
                     {
                        if(sms[g]==auth_num[i])
                        {
                           verify_count++; i++;        
                           if(verify_count==13)
                           {
                              verify_count=0;
                              g=a2_text;
                              for(g=a2_text+1; g<=prv_g; g++)
                              {
                                 if(sms[g]=='+')
                                 {
                                    a1_num=g;   //85
                                    break;
                                 }
                                 else if((sms[g]!='+')&&(g==prv_g+1))
                                 {
                                    a1_num=0;
                                    break;
                                 }
                              } //For Loop to check'+'
                              new_sms_receive_flag=0;
                              sms_stage=0;
                              
                              BREAK;
                           }  //if(verify_count==13), to check sender's number
                        }  //if(sms[g]==auth_num[i])
                        
                        Else
                        {
                           new_sms_receive_flag=0;
                           sms_stage=0;
                           Break;
                        }
                     } //for(g=g1_num; g<=g2_num; g++)
                  
                     if(a1_num!=0)
                     {
                        verify_count=0;
                        for(g=a1_num; g<=a1_num+12; g++) 
//checking each character of Received Number to be in format of a number.
                        {
                           if((sms[g]=='+')||(sms[g]=='9')||(sms[g]=='8')
                           ||(sms[g]=='7')||(sms[g]=='6')||(sms[g]=='5')
                           ||(sms[g]=='4')||(sms[g]=='3')||(sms[g]=='2')
                           ||(sms[g]=='1')
                           ||(sms[g]=='0'))
                           {
                              verify_count++;
                              if(verify_count==13)
                              {
                                 num_format=1;
                                 g=a1_num;   i=9;
                                 //Replacing Received Number to auth_client.
                                 for(g=a1_num; g<=a1_num+12; g++) 
                                 {
                                    auth_client[i]=sms[g];
                                    write_eeprom(i,sms[g]);
                                    i++;
                                 }
                                 acknowledge_flag=1;
                                 new_sms_receive_flag=3;
                                 break;
                              }
                              if((g==a1_num+12)&&(verify_count!=13))
                              {
                                 num_format=0;//number format
                                 verify_count=0;
                                 break;
                              }
                           }
                        }
                     }  //if(a1_num!=0)
                  }  //else if(authrize_confirm_flag==1)
                  //********************************************************
                  
                  if(new_sms_count1==30)
                  { 
                     delete_sms();
                     delay_ms(500);
                     new_sms_count1=0;
                  }
                  i=0;  verify_count=0;
                  for(g=0; g<=199; g++)
                  {
                     sms[g]=0;
                  }
                  
               }//if(new_sms_receive_flag==1)
               
//*****************************************************************************
                  
               if(write_sms_flag==3)
               {
                  if((write_sms_flag==3)&&(acknowledge_flag==1))
                  {
                     fprintf(GSM_STREAM,"You have been Subscribed for AEI-Temperature Monitoring System\r\n");
                  }
                  if((write_sms_flag==3)&&(acknowledge_flag==2))
                  {
                     fprintf(GSM_STREAM,"New Client has been Successfully Subscribed\r\n");
                  }
                  else if((write_sms_flag==3)&&(acknowledge_flag==0))
                  {
                     fprintf(GSM_STREAM,"Temperature Report:\r\n");
                     if(lm35_one<2.5)
                     { 
                        fprintf(GSM_STREAM,"T1: Very Low. %f *c\r\n",lm35_one);
                     }
                     else if((lm35_one>2.5)&&(lm35_one<4))
                     { 
                        fprintf(GSM_STREAM,"T1: Low. %f *c\r\n",lm35_one);
                     }
                     else if((lm35_one>=4)&&(lm35_one<=7))
                     { 
                        fprintf(GSM_STREAM,"T1: Normal. %f *c\r\n",lm35_one);
                     }
                     else if((lm35_one>7)&&(lm35_one<=9))
                     { 
                        fprintf(GSM_STREAM,"T1: High. %f *c\r\n",lm35_one);
                     }
                     else if(lm35_one>9)
                     { 
                        fprintf(GSM_STREAM,"T1: Very High. %f *c\r\n",lm35_one);
                     }
                        
                     if(lm35_two<2.5)
                     { 
                        fprintf(GSM_STREAM,"T2: Very Low. %f *c\r\n",lm35_two);
                     }
                     else if((lm35_two>2.5)&&(lm35_two<4))
                     { 
                        fprintf(GSM_STREAM,"T2: Low. %f *c\r\n",lm35_two);
                     }
                     else if((lm35_two>=4)&&(lm35_two<=7))
                     { 
                        fprintf(GSM_STREAM,"T2: Normal. %f *c\r\n",lm35_two);
                     }
                     else if((lm35_two>7)&&(lm35_two<=9))
                     { 
                        fprintf(GSM_STREAM,"T2: High. %f *c\r\n",lm35_two);
                     }
                     else if(lm35_two>9)
                     { 
                        fprintf(GSM_STREAM,"T2: Very High. %f *c\r\n",lm35_two);
                     }
                        
                     if(lm35_three<2.5)
                     { 
                        fprintf(GSM_STREAM,"T3: Very Low. %f *c\r\n",lm35_three);
                     }
                     else if((lm35_three>2.5)&&(lm35_three<4))
                     { 
                        fprintf(GSM_STREAM,"T3: Low. %f *c\r\n",lm35_three);
                     }
                     else if((lm35_three>=4)&&(lm35_three<=7))
                     { 
                        fprintf(GSM_STREAM,"T3: Normal. %f *c\r\n",lm35_three);
                     }
                     else if((lm35_three>7)&&(lm35_three<=9))
                     { 
                        fprintf(GSM_STREAM,"T3: High. %f *c\r\n",lm35_three);
                     }
                     else if(lm35_three>9)
                     { 
                        fprintf(GSM_STREAM,"T3: Very High. %f *c\r\n",lm35_three);
                     }
                        
                     if(lm35_four<2.5)
                     { 
                        fprintf(GSM_STREAM,"T4: Very Low. %f *c\r\n",lm35_four);                  
                     }
                     else if((lm35_four>2.5)&&(lm35_four<4))
                     { 
                        fprintf(GSM_STREAM,"T4: Low. %f *c\r\n",lm35_four);
                     }
                     else if((lm35_four>=4)&&(lm35_four<=7))
                     { 
                        fprintf(GSM_STREAM,"T4: Normal. %f *c\r\n",lm35_four);
                     }
                     else if((lm35_four>7)&&(lm35_four<=9))
                     { 
                        fprintf(GSM_STREAM,"T4: High. %f *c\r\n",lm35_four);
                     }
                     else if(lm35_four>9)
                     { 
                        fprintf(GSM_STREAM,"T4: Very High. %f *c\r\n",lm35_four);
                     }
                  }                                
                  fprintf(GSM_STREAM,"%c",ctrl_z);       
                  write_sms_flag=4;
                  for(g=0; g<=199; g++)
                  {
                     sms4[g]=0;
                  }
               }  //if(write_sms_flag==3)
               //**********************************************************

               else
               {
                  
                  identify_gsm_data();
                  for(g=0; g<=199; g++)
                  {
                     gsm[g]=0;
                  }
               }
               //receive_data=0; //For test purspose only.
               g=0;
               stream=0;
            }//if(e==25000)
         }//if(prv_z==z)
         else
         {     
            prv_z=z;
            e=0;
         }
      }//while(stream==1) 
      
      temp_check_and_decision();      
      send_query_sms();
   } 
}                                                     
//*****************************************************************************


