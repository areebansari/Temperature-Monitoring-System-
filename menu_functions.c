/* Also save this settings of each parameter in EEPROM */

void menu_input_sense()
{
   if(!input(menu_scroll))
   {
      while(!input(menu_scroll)) { delay_ms(10);  }
      if(menu_scroll_count==7)   menu_scroll_count=0;
      menu_scroll_count++;
      menu_time=0;
      menu_screens();
   }
   if(menu_scroll_count)
   {
      if(!input(parameter_function_input))
      {
         while(!input(parameter_function_input)) { delay_ms(10);  }
         IF(menu_scroll_count==7)
         {
            menu_scroll_count=0; menu_time=100; 
         }
         ELSE
         {
            menu_parameter_function();
            menu_time=0;
            menu_screens();
         }
      }
   }

   //For Trial Purpose, Untill Bit-routine is not working. The reading part is
   //written above the main() routine.
   if(menu_time>=100)      //menu_time=100 is equal to 10 Sec.
   { 
      menu_scroll_count=0; menu_time=0;
      write_eeprom(0, t1_flag);
      write_eeprom(1, t2_flag);
      write_eeprom(2, t3_flag);
      write_eeprom(3, t4_flag);
      write_eeprom(4, gsm_flag);
      write_eeprom(5, relay_flag);
   }
//!   output_high(PIN_C5);
//!   output_low(PIN_C5);
}


//*******************************************************
void menu_parameter_function()
{
   if(menu_scroll_count==1)   //Means T1 is being shown.
   {
      if(t1_flag) t1_flag=0;
      else t1_flag=1;
      
//!      if(t1_flag==1) { t1_flag=0;  }
//!      else if(t1_flag==0) { t1_flag=1;}
   }
   if(menu_scroll_count==2)   //Means T2 is being shown.
   {
      if(t2_flag) t2_flag=0;
      else t2_flag=1;
//!      if(t2_flag==TRUE) { t2_flag=FALSE;  }
//!      else if(t2_flag==FALSE) { t2_flag=TRUE; }
   }
   if(menu_scroll_count==3)   //Means T3 is being shown.
   {
         if(t3_flag) t3_flag=0;
      else t3_flag=1;
//!      if(t3_flag==TRUE) { t3_flag=FALSE;  }
//!      else if(t3_flag==FALSE) { t3_flag=TRUE; }
   }
   if(menu_scroll_count==4)   //Means T4 is being shown.
   {
         if(t4_flag) t4_flag=0;
      else t4_flag=1;
//!      if(t4_flag==TRUE) { t4_flag=FALSE;  }
//!      else if(t4_flag==FALSE) { t4_flag=TRUE; }
   }
   if(menu_scroll_count==5)   //Means T4 is being shown.
   {
         if(gsm_flag) gsm_flag=0;
      else gsm_flag=1;
//!      if(gsm_flag==TRUE) { gsm_flag=FALSE;  }
//!      else if(gsm_flag==FALSE) { gsm_flag=TRUE; }
   }
   if(menu_scroll_count==6)   //Means T4 is being shown.
   {
         if(relay_flag) relay_flag=0;
      else relay_flag=1;
//!      if(relay_flag==TRUE) { relay_flag=FALSE;  }
//!      else if(relay_flag==FALSE) { relay_flag=TRUE; }
   }
}

//************************************************************
void menu_screens()
{
   lcd_gotoxy(0,1);
   printf(lcd_putc,"                ",);
   lcd_gotoxy(0,2);
   printf(lcd_putc,"                ",);
   
   lcd_gotoxy(0,1);
   if(menu_scroll_count==1)   //Means T1 is being shown.
   {
      if(t1_flag) printf(lcd_putc,"T1: Active");     //trial purpose
      
      else    printf(lcd_putc,"T1: Deactive");     //trial purpose
      
   }
   
   ELSE if(menu_scroll_count==2)   //Means T2 is being shown.
   {
      if(t2_flag) printf(lcd_putc,"T2: Active");
      
      else  printf(lcd_putc,"T2: Deactive");
   }   
   
   ELSE if(menu_scroll_count==3)   //Means T3 is being shown.
   {
      if(t3_flag)  printf(lcd_putc,"T3: Active");
      
      else  printf(lcd_putc,"T3: Deactive");
   }
   
   ELSE if(menu_scroll_count==4)   //Means T4 is being shown.
   {
      if(t4_flag) printf(lcd_putc,"T4: Active");
      
      else  printf(lcd_putc,"T4: Deactive");
   }
   
   ELSE if(menu_scroll_count==5)   //Means GSM Status is being shown.
   {
      if(gsm_flag)   printf(lcd_putc,"GSM: Active");

      else  printf(lcd_putc,"GSM: Deactive");
   }
   
   ELSE if(menu_scroll_count==6)   //Means Alarm Status is being shown.
   {
      if(relay_flag) printf(lcd_putc,"ALARM: Active");

      else  printf(lcd_putc,"ALARM: Deactive");
   }
   
   ELSE if(menu_scroll_count==7)
   {
      printf(lcd_putc,"BACK");
   }
}
