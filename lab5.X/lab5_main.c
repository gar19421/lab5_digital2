/*
 * File:   lab3.c
 * Author: Brandon Garrido
 *
 * Created on July 26, 2021, 9:39 PM
 */


#include <xc.h>
#define __XTAL_FREQ 8000000
#include <stdint.h>


#include "USART.h"
#include "ADC.h"
//#include "LCD.h"

//******************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT    // Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)

#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//******************************************************************************
//                                  Variables
//******************************************************************************
  uint8_t ADC1;
  uint8_t ADC2;
  
  uint8_t bandera;
  uint8_t RC_temp;
  
  uint8_t contador;
  uint8_t contador_unidad = 0;
  uint8_t contador_decena = 0;
  uint8_t contador_centena = 0;
  
  uint8_t cont=0;
  
  uint8_t guia = 0x00;
  uint8_t decenas_temp;
  uint8_t var_temp;
  
  uint8_t val_USART;
  
uint8_t u_flag = 1;
uint8_t d_flag = 0;
uint8_t c_flag = 0;
uint8_t unidad = 0;
uint8_t decena = 0;
 
  
//******************************************************************************
                        //  Prototipos
//******************************************************************************
void setup (void);
//void displayLCD(void);


void main(void) {
    setup();
    while(1){  
        if(cont > 15){ //Se reinicia el contador después de 45ms y se enciende
         cont = 0; //el enable para enviar datos via USART
         TXIE = 1; 
        }
        
        contador_centena = PORTA / 100; // se divide dentro de la base 
        decenas_temp = PORTA % 100; // y luego se obtiene el modulo para ver 
        contador_decena = decenas_temp / 10; // el residuo y guardar el valor en la variable
        contador_unidad = PORTA % 10;
        
        if (PORTEbits.RE1 == 1){
            val_USART = 0;
            contador = 0;
            PORTE = 0;
            u_flag = 1;
            d_flag = 0;
            c_flag = 0;
        }
    }
        
}



void __interrupt() isr(void){
  
    if(INTCONbits.RBIF){
        if (PORTBbits.RB0 == 0){ // se realiza las acciones de contador
            PORTA++;
        }else if (PORTBbits.RB1 == 0){
            PORTA--;
        }
        INTCONbits.RBIF = 0;
    }
       
    
    if (INTCONbits.T0IF){           // INTERRUPCION TMR0
        cont++;
        INTCONbits.T0IF = 0;        // Limpiar la bandera de interrupción TMR0
    }   
    
    
    
    if(PIR1bits.RCIF == 1){ //Empieza a recibir datos del USART
        //0x0A para el salto de linea \n
        if (RCREG ==  0x0D){
            PORTD = contador;
            PORTE = 2;
        }
        
        if (RCREG !=  0x0D){
        RC_temp = RCREG;
        
            switch(RC_temp){
                case 48:
                    val_USART = 0;
                    break;
                case 49:
                    val_USART = 1;
                    break;
                case 50:
                    val_USART = 2;
                    break;
                case 51:
                    val_USART = 3;
                    break;
                case 52:
                    val_USART = 4;
                    break;
                case 53:
                    val_USART = 5;
                    break;
                case 54:
                    val_USART = 6;
                    break;
                case 55:
                    val_USART = 7;
                    break;
                case 56:
                    val_USART = 8;
                    break;       
                case 57:
                    val_USART = 9;
                    break;
            }  
            
            if (u_flag == 1){
                contador = val_USART;
                unidad = val_USART;
                u_flag = 0;
                d_flag = 1;
            }
            else if (d_flag == 1){
                contador = (unidad*10)+val_USART;
                decena = val_USART;
                d_flag = 0;
                c_flag = 1;
            }
            else if (c_flag == 1){
                contador = (unidad*100)+(decena*10)+val_USART;
                d_flag = 0;
                c_flag = 1;
            } 
        }
    }
    
    if (TXIF == 1){
        //enviar datos
        
        if(guia==0x00){
            TXREG = contador_centena + 48;
            guia = 0x01;
        }else if(guia==0x01){
            TXREG = contador_decena + 48;
            guia =0x02;
        } else if(guia==0x02){
            TXREG = contador_unidad + 48;
            guia = 0x03;
        }else if(guia==0x03){
            TXREG = 0x0D;
            guia = 0x00;
        }
        TXIF = 0; //Se limpia la bandera
    } 
     
     

}



void setup(void){
    
    //Configuración de reloj
    OSCCONbits.IRCF2 =1 ; // IRCF = 111 (8MHz) 
    OSCCONbits.IRCF1 =1 ;
    OSCCONbits.IRCF0 =1 ;
    OSCCONbits.SCS = 1; // Habilitar reloj interno
    
    ANSEL = 0x00;
    ANSELH = 0x00;
    //Configuración puertos I/O
    TRISA = 0x00;
    TRISB = 0x03;
    TRISD = 0;
    TRISE = 0;
        
    OPTION_REGbits.nRBPU =  0 ; // se habilita el pull up interno en PORTB
    WPUB = 0x03;  // se habilita los pull ups para los pines RB0 y RB1 
    
    PORTA = 0x00;
    PORTB = 0x03;
    PORTD = 0x00;
    PORTE = 0x00;
    
    
    //Habilitar interrupciones globales
    INTCONbits.GIE = 1;     
    INTCONbits.T0IE = 1;           
    INTCONbits.T0IF = 0;
    
    //Configuración de TX y RX
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    
    BAUDCTLbits.BRG16 = 1;
    
    SPBRG = 207;
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;//Configuración del USART y Baud Rate
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    
    TXSTAbits.TXEN = 1; 
    
    
    INTCONbits.RBIE = 1; // habilitar banderas de interrupción puertos B
    INTCONbits.RBIF = 0;
    //configuración iocb
    IOCB = 0x03; // setear interrupciones en los pines RB0 y RB1 
    INTCONbits.RBIF = 0;
      
    
    //Configuración TMR0
    OPTION_REGbits.T0CS = 0;        // TMR0 Clock source
    OPTION_REGbits.PSA = 0;         // Prescaler a tmr0
    OPTION_REGbits.PS = 0b111;        // prescaler 1:256
    TMR0 = 10;
    
    
    
    
  
    
}

