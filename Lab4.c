//Primera seccion
#include<xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _XTAL_FREQ 1000000
#include "LibLCDXC8.h"

//segunda seccion
#pragma config WDT=OFF
#pragma config FOSC=INTOSC_EC
#pragma config PBADEN=OFF
#pragma config LVP=OFF

//tercera seccion
unsigned char Tecla;
unsigned char LeerTeclado(void);
void interrupt ISR(void);

//cuarta seccion

void main(void){
    unsigned char i;
    ADCON1=15;
    TRISD = 0;
    TRISE = 0;
    TRISB = 0b11110000;
    LATD = 0;
    LATE = 0;
    LATB = 0;
    __delay_ms(100);
    RBPU = 0;
    __delay_ms(100);
    /*TMR0=3036;
    T0CON=0b00000001;
    TMR0IF=0;
    TMR0IE=1;
    TMR0ON=1;*/
    RBIF=0;
    RBIE=1;
    GIE=1;
    __delay_ms(1000);
    ConfiguraLCD(4);
    InicializaLCD();
    BorraLCD();
    MensajeLCD_Var("Hola mundo");  
    DireccionaLCD(0xC0);
    MensajeLCD_Var("7");
    __delay_ms(1000);
    while(1){
        Tecla=LeerTeclado();
        
        MensajeLCD_Var(Tecla);
    }    
}

unsigned char LeerTeclado(void){
    LATB = 0b00000000;
    while(PORTB == 0b11110000){
    }
    LATB = 0b0111; //fila 1
    if ((PORTB&0b11110000) == 0b11100000){ //columna 1
        return '1';
    }
    if ((PORTB&0b11110000) == 0b11010000){ //columna 2
        return '2';
    }
    if ((PORTB&0b11110000) == 0b10110000){ //columna 3
        return '3';
    }
    if ((PORTB&0b11110000) == 0b01110000){ //columna 4
        return '+';
    }
    
    LATB = 0b1011; //fila 2
    if ((PORTB&0b11110000) == 0b11100000){ //columna 1
        return '4';
    }
    if ((PORTB&0b11110000) == 0b11010000){ //columna 2
        return '5';
    }
    if ((PORTB&0b11110000) == 0b10110000){ //columna 3
        return '6';
    }
    if ((PORTB&0b11110000) == 0b01110000){ //columna 4
        return '-';
    }
    
    LATB = 0b1101; //fila 3
    if ((PORTB&0b11110000) == 0b11100000){ //columna 1
        return '7';
    }
    if ((PORTB&0b11110000) == 0b11010000){ //columna 2
        return '8';
    }
    if ((PORTB&0b11110000) == 0b10110000){ //columna 3
        return '9';
    }
    if ((PORTB&0b11110000) == 0b01110000){ //columna 4
        return '*';
    }
    
    LATB = 0b1110; //fila 4
    if ((PORTB&0b11110000) == 0b11100000){ //columna 1
        return 'e';
    }
    if ((PORTB&0b11110000) == 0b11010000){ //columna 2
        return '0';
    }
    if ((PORTB&0b11110000) == 0b10110000){ //columna 3
        return '=';
    }
    if ((PORTB&0b11110000) == 0b01110000){ //columna 4
        return '/';
    }
    return '?';
}
void __interrupt ISR(void){
    if(RBIF==1){
        if(PORTB!=0b11110000){
            Tecla=16;
            LATB=0b11111110;
            if(RB4==0) Tecla="0";
            else if(RB5==0) Tecla="1";
            else if(RB6==0) Tecla="2";
            else if(RB7==0) Tecla="3";
            else{
                LATB=0b11111101;
                if(RB4==0) Tecla="4";
                else if(RB5==0) Tecla="5";
                else if(RB6==0) Tecla="6";
                else if(RB7==0) Tecla="7";
                else{
                    LATB=0b11111011;
                    if(RB4==0) Tecla="8";
                    else if(RB5==0) Tecla="9";
                    else if(RB6==0) Tecla="10";
                    else if(RB7==0) Tecla="11";
                    else{
                        LATB=0b11110111;
                        if(RB4==0) Tecla="12";
                        else if(RB5==0) Tecla="13";
                        else if(RB6==0) Tecla="14";
                        else if(RB7==0) Tecla="15";
                    }
                }
            }
            LATB=0b11110000;
        }
        __delay_ms(100);
        RBIF=0;
    }
    if(TMR0IF==1){
        TMR0IF=0;
        TMR0=3036;
        LATD1=LATD1^1;
    }
}
