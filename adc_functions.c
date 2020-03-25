//***********************Temperature Checking & Decision***********************
void temp_check_and_decision()
{

   //********************************************
   if(t1_flag)
   {
      if(lm35_one>=high_temp)
      {
         temp_high=1;
         if(relay_op1==0)
         {
            t1_check_time=0;
            relay_op1=1;
         }
         else if(relay_op1==1)
         {
            if(t1_check_time>=temp_stable_time) //check if condition persist upto 15 mins. t1_check_time=600= 1 minute
            {
               t1_check_time=0;  relay_op1=2;
               condition=0;      sms_resend=0;
            }
         }
      }
      else if(lm35_one<low_temp)
      {
         temp_low=1;
         if(relay_op1==0)
         {
            t1_check_time=0;
            relay_op1=1;
         }
         else if(relay_op1==1)
         {
            if(t1_check_time>=temp_stable_time) //check if condition persist upto SET mins
            {
               t1_check_time=0;
               relay_op1=2;
               condition=0;
               sms_resend=0;
            }
         }
      }
      else
      {
         relay_op1=0;
      }
      }
   
   else  lm35_one=5; /*this Routine will function when T2 Would be deactivated.
   In order make IF-Loop True (Normal-Temp-comparision-loop), LM35_two is forced to
   be normal*/   
   
   //**************************************************************************
   
   if(t2_flag)
   {
      if(lm35_two>=high_temp)
      {
         temp_high=1;
         if(relay_op2==0)  
         {
            t2_check_time=0;
            relay_op2=1;
         }
         else if(relay_op2==1)
         {
//!            if(t2_check_time>=1200)
            if(t2_check_time>=temp_stable_time)
            {
               t2_check_time=0;
               relay_op2=2;
               condition=0;
               sms_resend=0;
            }
         }
      }
      else if(lm35_two<low_temp)
      {
         temp_low=1;
         if(relay_op2==0)    
         {
            t2_check_time=0;
            relay_op2=1;
         }
         if(relay_op2==1)
         {
//!            if(t2_check_time>=1200) //check if condition persist upto 15 mins
            if(t2_check_time>=temp_stable_time) //check if condition persist upto 15 mins
            {
               t2_check_time=0;
               relay_op2=2;
               condition=0;
               sms_resend=0;
            }
         }
      }
      else
      {
         relay_op2=0;
         //t2_check_time=0;
      }
      }
   
   else  lm35_two=5;
   
   //**************************************************************************
   
   if(t3_flag)
   {
      if(lm35_three>=high_temp)
      {
         temp_high=1;
         if(relay_op3==0)   
         {
            t3_check_time=0;
            relay_op3=1;
         }
         else if(relay_op3==1)
         {
//!            if(t3_check_time>=1200) //check if condition persist upto 15 mins
            if(t3_check_time>=temp_stable_time) //check if condition persist upto 15 mins
            {
               t3_check_time=0;
               relay_op3=2;
               condition=0;
               sms_resend=0;
            }
         }
      }
      else if(lm35_three<low_temp)
      {
         temp_low=1;
         if(relay_op3==0)    
         {
            t2_check_time=0;
            relay_op3=1;
         }
         else if(relay_op3==1)
         {
            if(t3_check_time>=temp_stable_time)
            {
               t3_check_time=0;
               relay_op3=2;
               condition=0;
               sms_resend=0;
            }
         }
      }
      else
      {
         relay_op3=0;
      }
   }
   
   else  lm35_three=5;
   
   //********************************************************
   if(t4_flag)
   {
      if(lm35_four>=high_temp)
      {
         temp_high=1;
         if(relay_op4==0)   
         {
            t4_check_time=0;
            relay_op4=1;
         }
         else if(relay_op4==1)
         {
            if(t4_check_time>=temp_stable_time) //check if condition persist upto 15 mins
            {
               t4_check_time=0;
               relay_op4=2;
               condition=0;
               sms_resend=0;
            }
         }
      }
      else if(lm35_four<low_temp)
      {
         temp_low=1;
         if(relay_op4==0)   
         {
            t4_check_time=0;
            relay_op4=1;
         }
         else if(relay_op4==1)
         {
            if(t4_check_time>=temp_stable_time)
            {
               t4_check_time=0;
               relay_op4=2;
               condition=0;
               sms_resend=0;
            }
         }
      }
      else
      {
         relay_op4=0;
      }
   }
   else  lm35_four=5;
   //**************************************************************************

   if((lm35_one>low_temp)&&(lm35_one<high_temp)&&(lm35_two>low_temp)&&
   (lm35_two<high_temp)&&(lm35_three>low_temp)&&(lm35_three<high_temp)&&
   (lm35_four>low_temp)&&(lm35_four<high_temp))
   {
      temp_low=0; temp_high=0;
   }
   
   if((temp_low==1)||(temp_high==1))
   {
      if((relay_op1==2)||(relay_op2==2)||(relay_op3==2)||(relay_op4==2))
      {
         condition_function();
      }
   }
   
   else
   {
     disable_interrupts(INT_RTCC);
     sms_resend_time1=0;
     sms_resend=0;
     condition=0;
     temp_low=0; temp_high=0;
     mute_flag=0;
     output_low(RELAY);   //Turn Relay Off.
     relay_op1=0; relay_op2=0;
     relay_op3=0; relay_op4=0;
   }
}



//**************************** condition_function() ****************************
void condition_function()
{
   if(condition==0)
   {
      condition=1;
      sms_resend_time1=0;
   }
   else if(condition==1)
   {
      //if(sms_resend_time1>=3000)  //200= 20 seconds ....... 9000= 15 Min
      //{
         condition=2;
         mute_flag=1;   relay_time=0;
         sms_resend_time1=0;
      //}
   }
   else if(condition==2)
   {
      //if(mute_flag==0)
      //{
         //output_high(RELAY);// Turn Relay ON
         if(!input(MUTE))
         {
            mute_flag=2;
            output_low(RELAY);// Turn Relay OFF. (Relay is turnig ON in Interrupt)
            while(!input(MUTE));
         }
      //}
         
      if(gsm_pwr_status==4)   //when GSM is enabled
      {
         if(sms_resend==0)  //After one try it will be disabled
         {
            if(stream==0)  //To check if GSM-Moduled is not sending data.
            {
               if((sms_stage==0)&&(num_format==1))
               {
                  sms_resend=1;                 
                  sms_resend_time1=0;
                  send_sms_count=0;          //It sends Directly to Periodi-Sms-Sending Routing without Verification. Change it to"send_sms_count=0;"                                 
                  g=0;
                  sms_stage=1;
                  for(i=0;i<=23;i++)
                  {
                     fprintf(GSM_STREAM,"%c",auth_client[i]);
                     if(i==23){ g=0; send_sms_flag=2;  }
                  }
               }  //if(sms_stage==0)
            }  //if(stream==0)
         }  //(sms_resend==0)
            
         if(send_sms_count==0)            
         {
            //enable_interrupts(INT_RTCC);
            if(sms_resend_time1>=150)
            {
               sms_resend_time1=0;
               if(send_sms_flag==3)
               {
                  send_sms_count=1;
               }
               else
               {
                  //disable_interrupts(INT_RTCC);
                  sms_resend_time1=0;
                  sms_resend=0;
                  sms_stage=0;
               }
            }
         }
         if(send_sms_count==1)
         {
            if(sms_resend_time1>=18000)
            //if(sms_resend_time1>=1200)
            {
               sms_resend_time1=0;
               sms_resend=0;
            }
         }
      }  //if(gsm_pwr_status==4)
      Else
      {
         if(gsm_checking==0)
         {
            sms_resend_time1=0;
            sms_resend=0;
         }
      }
   }  //else if(condition==2)
}



//*****************************ADC Channel Scanning****************************
void scan_ad_ch()
{                       
   if(adc_flag==0)
   {
      //*********************************************************************
      if(t1_flag)    // True when T1 is activated by menu
      {
         if((sms_stage==0)&&(stream==0))
         {
            set_adc_channel(0);
            delay_us(10);
            f_1=0;
            measure_adc_values();
            lm35_one=f_1;
         }
      }
      
      if(t2_flag) // True when T2 is activated by menu
      {
         if((sms_stage==0)&&(stream==0))
         {
            set_adc_channel(1);
            delay_us(10);
            f_1=0;
            measure_adc_values();
            lm35_two=f_1;
         }
      }
      
      if(t3_flag) // True when T3 is activated by menu
      {
         if((sms_stage==0)&&(stream==0))
         {
            set_adc_channel(2);
            delay_us(10);
            f_1=0;
            measure_adc_values();
            lm35_three=f_1;
         }
      }
      
      if(t4_flag)    // True when T4 is activated by menu
      {
         if((sms_stage==0)&&(stream==0))
         {
            set_adc_channel(3);
            delay_us(10);
            f_1=0;
            measure_adc_values();
            lm35_four=f_1;
         }
      }
      //**********************************************************************
      adc_time=0;
      adc_flag=1;
   }
   
   

}
//*****************************************************************************

void measure_adc_values()
{
   av_1=read_adc();
   adc_to_temp_f1=av_1*4.88;
   adc_to_temp_conversion();
   f_1=adc_to_temp_f1;

}

//*********************ADC VALUE TO TEMPRATURE CONVERSION**********************

void adc_to_temp_conversion()
{
   adc_to_temp_f2=adc_to_temp_f1;
   
   if((adc_to_temp_f2>0)&&(adc_to_temp_f2<39.00))              /*0'C*/
   {
      adc_to_temp_f2=((adc_to_temp_f2-0)/39);
      adc_to_temp_f1=(0+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>39.00)&&(adc_to_temp_f2<79.00))     /*1'C*/
   {
      adc_to_temp_f2=((adc_to_temp_f2-39)/40);
      adc_to_temp_f1=(1+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>79.00)&&(adc_to_temp_f2<119.00))    /*2'C*/  
   {
      adc_to_temp_f2=((adc_to_temp_f2-79)/40);                
      adc_to_temp_f1=(2+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>119.00)&&(adc_to_temp_f2<158.00))   /*3'C*/      
   {                                                        
      adc_to_temp_f2=((adc_to_temp_f2-119)/39);
      adc_to_temp_f1=(3+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>158.00)&&(adc_to_temp_f2<198.00))   /*4'C*/   
   {                                                          
      adc_to_temp_f2=((adc_to_temp_f2-158)/40);
      adc_to_temp_f1=(4+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>198.00)&&(adc_to_temp_f2<238.00))   /*5'C*/  
   {                                                          
      adc_to_temp_f2=((adc_to_temp_f2-198)/40);
      adc_to_temp_f1=(5+adc_to_temp_f2);    
   }
   else if((adc_to_temp_f2>238.00)&&(adc_to_temp_f2<277.00))   /*6'C*/  
   {                                                          
      adc_to_temp_f2=((adc_to_temp_f2-238)/39);
      adc_to_temp_f1=(6+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>277.00)&&(adc_to_temp_f2<317.00))   /*7'C*/  
   {                                                          
      adc_to_temp_f2=((adc_to_temp_f2-277)/40);
      adc_to_temp_f1=(7+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>317.00)&&(adc_to_temp_f2<357.00))   /*8'C*/  
   {                                                          
      adc_to_temp_f2=((adc_to_temp_f2-317)/40);
      adc_to_temp_f1=(8+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>357.00)&&(adc_to_temp_f2<397.00))   /*9'C*/    
   {                                                          
      adc_to_temp_f2=((adc_to_temp_f2-357)/40);
      adc_to_temp_f1=(9+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>397.00)&&(adc_to_temp_f2<437.00))   /*10'C*/ 
   {                                                          
      adc_to_temp_f2=((adc_to_temp_f2-397)/40);
      adc_to_temp_f1=(10+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>437.00)&&(adc_to_temp_f2<477.00))   /*11'C*/ 
   {        
      adc_to_temp_f2=((adc_to_temp_f2-437)/40);
      adc_to_temp_f1=(11+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>477.00)&&(adc_to_temp_f2<517.00))   /*12'C*/ 
   {
      adc_to_temp_f2=((adc_to_temp_f2-477)/40);
      adc_to_temp_f1=(12+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>517.00)&&(adc_to_temp_f2<557.00))   /*13'C*/ 
   {                                                          
      adc_to_temp_f2=((adc_to_temp_f2-517)/40);
      adc_to_temp_f1=(13+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>557.00)&&(adc_to_temp_f2<597.00))   /*14'C*/ 
   {                                                          
      adc_to_temp_f2=((adc_to_temp_f2-557)/40);
      adc_to_temp_f1=(14+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>597.00)&&(adc_to_temp_f2<637.00))   /*15'C*/  
   {
      adc_to_temp_f2=((adc_to_temp_f2-597)/40);
      adc_to_temp_f1=(15+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>637.00)&&(adc_to_temp_f2<677.00))   /*16'C*/   
   {
      adc_to_temp_f2=((adc_to_temp_f2-637)/40);
      adc_to_temp_f1=(16+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>677.00)&&(adc_to_temp_f2<718.00))   /*17'C*/       
   {
      adc_to_temp_f2=((adc_to_temp_f2-677)/41);
      adc_to_temp_f1=(17+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>718.00)&&(adc_to_temp_f2<758.00))   /*18'C*/       
   {
      adc_to_temp_f2=((adc_to_temp_f2-718)/40);
      adc_to_temp_f1=(18+adc_to_temp_f2);
   }                                                                   
   else if((adc_to_temp_f2>758.00)&&(adc_to_temp_f2<799.00))   /*19'C*/       
   {
      adc_to_temp_f2=((adc_to_temp_f2-758)/41);
      adc_to_temp_f1=(19+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>799.00)&&(adc_to_temp_f2<838.00))   /*20'C*/  
   {
      adc_to_temp_f2=((adc_to_temp_f2-799)/39);
      adc_to_temp_f1=(20+adc_to_temp_f2);
   }                                                                
   else if((adc_to_temp_f2>838.00)&&(adc_to_temp_f2<879.00))   /*21'C*/ 
   {
      adc_to_temp_f2=((adc_to_temp_f2-838)/41);
      adc_to_temp_f1=(21+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>879.00)&&(adc_to_temp_f2<919.00))   /*22'C*/  
   {
      adc_to_temp_f2=((adc_to_temp_f2-879)/40);
      adc_to_temp_f1=(22+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>919.00)&&(adc_to_temp_f2<960.00))   /*23'C*/      
   {
      adc_to_temp_f2=((adc_to_temp_f2-919)/41);
      adc_to_temp_f1=(23+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>960.00)&&(adc_to_temp_f2<1000.00))  /*24'C*/     
   {
      adc_to_temp_f2=((adc_to_temp_f2-960)/40);
      adc_to_temp_f1=(24+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1000.00)&&(adc_to_temp_f2<1041.00)) /*25'C*/
   {                                                           
      adc_to_temp_f2=((adc_to_temp_f2-1000)/41);
      adc_to_temp_f1=(25+adc_to_temp_f2);
   }                                                              
   else if((adc_to_temp_f2>1041.00)&&(adc_to_temp_f2<1081.00)) /*26'C*/ 
   {
      adc_to_temp_f2=((adc_to_temp_f2-1041)/40);
      adc_to_temp_f1=(26+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1081.00)&&(adc_to_temp_f2<1122.00)) /*27'C*/    
   {
      adc_to_temp_f2=((adc_to_temp_f2-1081)/41);
      adc_to_temp_f1=(27+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1122.00)&&(adc_to_temp_f2<1163.00)) /*28'C*/    
   {
      adc_to_temp_f2=((adc_to_temp_f2-1122)/41);
      adc_to_temp_f1=(28+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1163.00)&&(adc_to_temp_f2<1203.00)) /*29'C*/    
   {
      adc_to_temp_f2=((adc_to_temp_f2-1163)/40);
      adc_to_temp_f1=(29+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1203.00)&&(adc_to_temp_f2<1244))    /*30'C*/       
   {
      adc_to_temp_f2=((adc_to_temp_f2-1203)/41);
      adc_to_temp_f1=(30+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1244.00)&&(adc_to_temp_f2<1285))    /*31'C*/       
   {
      adc_to_temp_f2=((adc_to_temp_f2-1244)/41);
      adc_to_temp_f1=(31+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1285.00)&&(adc_to_temp_f2<1326))    /*32'C*/ 
   {                                                             
      adc_to_temp_f2=((adc_to_temp_f2-1285)/41);
      adc_to_temp_f1=(32+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1326.00)&&(adc_to_temp_f2<1366))    /*33'C*/       
   {
      adc_to_temp_f2=((adc_to_temp_f2-1326)/40);
      adc_to_temp_f1=(33+adc_to_temp_f2);
   }                                                              
   else if((adc_to_temp_f2>1366.00)&&(adc_to_temp_f2<1407))    /*34'C*/    
   {                                                             
      adc_to_temp_f2=((adc_to_temp_f2-1366)/41);
      adc_to_temp_f1=(34+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1407.00)&&(adc_to_temp_f2<1448))    /*35'C*/       
   {
      adc_to_temp_f2=((adc_to_temp_f2-1407)/41);
      adc_to_temp_f1=(35+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1448.00)&&(adc_to_temp_f2<1489))    /*36'C*/       
   {
      adc_to_temp_f2=((adc_to_temp_f2-1448)/41);
      adc_to_temp_f1=(36+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1489.00)&&(adc_to_temp_f2<1530))    /*37'C*/       
   {
      adc_to_temp_f2=((adc_to_temp_f2-1489)/41);
      adc_to_temp_f1=(37+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1530.00)&&(adc_to_temp_f2<1571))    /*38'C*/       
   {
      adc_to_temp_f2=((adc_to_temp_f2-1530)/41);
      adc_to_temp_f1=(38+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1571.00)&&(adc_to_temp_f2<1612))    /*39'C*/       
   {
      adc_to_temp_f2=((adc_to_temp_f2-1571)/41);
      adc_to_temp_f1=(39+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1612.00)&&(adc_to_temp_f2<1653))    /*40'C*/       
   {
      adc_to_temp_f2=((adc_to_temp_f2-1612)/41);
      adc_to_temp_f1=(40+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1653.00)&&(adc_to_temp_f2<1694))    /*41'C*/       
   {
      adc_to_temp_f2=((adc_to_temp_f2-1653)/41);
      adc_to_temp_f1=(41+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1694.00)&&(adc_to_temp_f2<1735))    /*42'C*/       
   {
      adc_to_temp_f2=((adc_to_temp_f2-1694)/41);
      adc_to_temp_f1=(42+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1735.00)&&(adc_to_temp_f2<1776))    /*43'C*/       
   {
      adc_to_temp_f2=((adc_to_temp_f2-1735)/41);
      adc_to_temp_f1=(43+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1776.00)&&(adc_to_temp_f2<1817))    /*44'C*/       
   {
      adc_to_temp_f2=((adc_to_temp_f2-1776)/41);
      adc_to_temp_f1=(44+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1817.00)&&(adc_to_temp_f2<1858))    /*45'C*/       
   {
      adc_to_temp_f2=((adc_to_temp_f2-1817)/41);
      adc_to_temp_f1=(45+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1858.00)&&(adc_to_temp_f2<1899))    /*46'C*/       
   {
      adc_to_temp_f2=((adc_to_temp_f2-1858)/41);
      adc_to_temp_f1=(46+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1899.00)&&(adc_to_temp_f2<1941))    /*47'C*/       
   {
      adc_to_temp_f2=((adc_to_temp_f2-1899)/42);
      adc_to_temp_f1=(47+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1941.00)&&(adc_to_temp_f2<1982))    /*48'C*/       
   {
      adc_to_temp_f2=((adc_to_temp_f2-1941)/41);
      adc_to_temp_f1=(48+adc_to_temp_f2);
   }
   else if((adc_to_temp_f2>1982.00)&&(adc_to_temp_f2<2023))    /*49'C*/       
   {
      adc_to_temp_f2=((adc_to_temp_f2-1982)/41);
      adc_to_temp_f1=(49+adc_to_temp_f2);
   }
   else if(adc_to_temp_f2>2023)
   {
      adc_to_temp_f1=0xFFFF;
   }
}
//*****************************************************************************


