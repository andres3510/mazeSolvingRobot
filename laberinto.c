#INCLUDE <16F887.H>
#FUSES NOWDT, NOPUT,INTRC_IO,NOPROTECT, NOBROWNOUT,NOWRT
#USE DELAY(internal = 8MHZ)

#include <LCD.c>
#use fast_io(b)
#use fast_io(c)
#use fast_io(a)
#use fast_io(d)

// DECLARACION DE LAS VARIABLES DEL PROGRAMA

float distancia_cm()
{
   float distancia=0;
   float tiempo=0;
   
   output_HIGH(pin_b1);
   delay_us(10);
   output_LOW(pin_b1);
   set_timer1(0);
   while(!input(pin_b0))
   {
       tiempo=get_timer1();
       if(tiempo > 65500){
           break;
       }
   }
   set_timer1(0);
   while(input(pin_b0))
   {
       tiempo=get_timer1();
       if(tiempo > 65500){
           break;
       }
   }
   tiempo=get_timer1();
   
   distancia=tiempo/58;

   return(distancia);
}

float distancia_cm_R()
{
   float distancia=0;
   float tiempo=0;
   
   output_HIGH(pin_b3);
   delay_us(10);
   output_LOW(pin_b3);
   set_timer1(0);
   while(!input(pin_b2))
   {
       tiempo=get_timer1();
       if(tiempo > 65500){
           break;
       }
   }
   set_timer1(0);
   while(input(pin_b2))
   {
       tiempo=get_timer1();
       if(tiempo > 65500){
           break;
       }
   }
   tiempo=get_timer1();
   
   distancia=tiempo/58;

   return(distancia);
}

float distancia_cm_L()
{
   float distancia=0;
   float tiempo=0;
   
   output_HIGH(pin_b5);
   delay_us(10);
   output_LOW(pin_b5);
   set_timer1(0);
   while(!input(pin_b4))
   {
       tiempo=get_timer1();
       if(tiempo > 65500){
           break;
       }
   }
   set_timer1(0);
   while(input(pin_b4))
   {
       tiempo=get_timer1();
       if(tiempo > 65500){
           break;
       }
   }
   tiempo=get_timer1();
   
   distancia=tiempo/58;

   return(distancia);
}
//--------------------------funciones---------------------------------

void avance_low()
{
    set_pwm1_duty((int16)400); // 210
    output_low(pin_a0);
    output_high(pin_a1);
    set_pwm2_duty((int16)400); // 225
    output_low(pin_a2);
    output_high(pin_a3);
}
void izquierda()
{
    set_pwm1_duty((int16)300); //270
    output_low(pin_a0);
    output_high(pin_a1);
    set_pwm2_duty((int16)0); //240
    output_low(pin_a2);
    output_high(pin_a3);
}
void derecha()
{ 
   set_pwm1_duty((int16)0);
   output_low(pin_a0);
   output_high(pin_a1);
   set_pwm2_duty((int16)300); //310
   output_low(pin_a2);
   output_high(pin_a3);
}
void retroceso()
{  
   set_pwm1_duty((int16)800);
   output_high(pin_a0);
   output_low(pin_a1);
   set_pwm2_duty((int16)800);
   output_high(pin_a2);
   output_low(pin_a3);
}
void giro()
{  
   set_pwm1_duty((int16)240); //240
   output_low(pin_a0);
   output_high(pin_a1);
   set_pwm2_duty((int16)330); //330
   output_high(pin_a2);
   output_low(pin_a3);
}
void giro_L()
{  
   set_pwm1_duty((int16)240); //240
   output_high(pin_a0);
   output_low(pin_a1);
   set_pwm2_duty((int16)330); //330
   output_low(pin_a2);
   output_high(pin_a3);
}
void pare()
{  
   set_pwm1_duty((int16)0);
   output_low(pin_a0);
   output_low(pin_a1);
   set_pwm2_duty((int16)0);
   output_low(pin_a2);
   output_low(pin_a3);
}

//---------------------funcion principal----------------------------------
void main()
{              
   lcd_init();
   set_tris_b(0b01010101);
   port_b_pullups(255);
   output_b(0);
   set_tris_c(0);
   output_c(0);
   
   set_tris_a(0);
   output_a(0);
   
   set_tris_d(0);
   output_d(255);
   
   // Configuracion de la interrupcion por captura del ccp1
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_2);
   set_timer1(0);
   
   setup_timer_2(T2_DIV_BY_16, 125,1);
   
   setup_ccp1( CCP_PWM );
   setup_ccp2( CCP_PWM );
   set_pwm1_duty((int16)0);
   set_pwm2_duty((int16)0);

    // Variable para almacenar el estado de dirección
   // unsigned int direccion = 1; // 1: Avance, 2: Derecha, 3: Atrás, 4: Izquierda
   
   while(true)
   {
      float frente = distancia_cm();
      delay_ms(10);
      float ladoR = distancia_cm_R();
      delay_ms(10);
      float ladoL = distancia_cm_L();
      delay_ms(10);
      
      if(frente > 10)
      {
          avance_low();
          output_high(pin_e0);
      }else{
          output_low(pin_e0);
      }
      
      if(ladoR > 10)
      {
          output_high(pin_e1);
      }else{
          output_low(pin_e1);
      }
      
      if(ladol > 10)
      {
          output_high(pin_e2);
      }else{
          output_low(pin_e2);
      }
      
      lcd_gotoxy(1,1);
      printf(lcd_putc,"L%0.1f", ladoL); 
      lcd_gotoxy(6,1);
      printf(lcd_putc,"R%0.1f", ladoR); 
      lcd_gotoxy(12,1);
      printf(lcd_putc,"F%0.1f", frente); 
      
       if(frente<=7.5 && ladoR<12.0)
      {
         pare();
         delay_ms(300);
         retroceso();
         delay_ms(1000);
         pare();
         delay_ms(450);
         giro();
         delay_ms(420);
         
      }
      else if (frente<=7.5 && ladoL<12.0)
      {
         pare();
         delay_ms(300);
         retroceso();
         delay_ms(1000);
         pare();
         delay_ms(450);
         giro_L();
         delay_ms(420);
      }
      else if(frente<=7.5)
      {
         pare();
         delay_ms(300);
         retroceso();
         delay_ms(1000);
         pare();
         delay_ms(450);
      }
      else if(ladoR<=8.5)
      {
         derecha();
         delay_ms(50);
      }
      else if(ladoL <= 7.5)
      {
         izquierda();
         delay_ms(50);
      }
      else if(ladoR>=12.5)
      {
         derecha();
         delay_ms(100);
         avance_low();
         delay_ms(150);
         izquierda();
         delay_ms(250);
      }
      else
      {
         avance_low();
         delay_ms(50);
      }
   }
}