#include<xc.h>
#include<stdio.h>
#include <string.h>
#define _XTAL_FREQ 1000000
#include "LibLCDXC8.h"

//segunda seccion
#pragma config WDT=OFF
#pragma config FOSC=INTOSC_EC
#pragma config PBADEN=OFF
#pragma config LVP=OFF

unsigned char Tecla = 0;
float nodec = 0;
unsigned long partdecl = 0;
unsigned long r = 0;
unsigned int n1 = 0;
unsigned int n2 = 0;
unsigned int i = 0;
unsigned char op = ' ';
unsigned char n1c = ' ';
unsigned char n2c = ' ';
unsigned char LeerTeclado(void);

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
    InicializaLCD();
    BorraLCD();
    MensajeLCD_Var("Hola mundo");
    __delay_ms(2000);
    
    while(1){
        LATB=0b00000000;
        Tecla = LeerTeclado();
        BorraLCD();
        if(Tecla=='C'){
            DireccionaLCD(0xC0);
            op = ' ';
            n1c = ' ';
            n2c = ' ';
            n1 = 0;
            n2 = 0;
            i = 0;
            ConfiguraLCD(4);
            InicializaLCD();
            DireccionaLCD(0x80);
            MensajeLCD_Var("                ");
            DireccionaLCD(0xC0);
            DireccionaLCD(0x80);
        }else{
            if(n1c==' '|n2c==' '|op==' '){
                if(op==' ' & (Tecla=='+'|Tecla=='-'|Tecla=='/'|Tecla=='x')){
                DireccionaLCD(0x81);
                EscribeLCD_c(Tecla);
                op = Tecla;
                }else{
                    if(Tecla=='='){
                        if(n1c!=' ' & n2c!=' ' & op==' '){
                    if(n1c==' '){
                        DireccionaLCD(0x80);  
                        EscribeLCD_c(Tecla);
                        n1c=Tecla;
                        n1 = Tecla-'0';
                    }else{
                        if(n2c==' ' & op!=' '){
                            DireccionaLCD(0x82);
                            EscribeLCD_c(Tecla);
                            n2c=Tecla;
                            n2 = Tecla-'0';
                        }
                    }
                    
                }
        }else{
           if(Tecla=='='){
                        if(n1c!=' ' & n2c!=' ' & op!=' '){
                            DireccionaLCD(0x83);
                            EscribeLCD_c('=');
                            switch(op){
                            case '+': r = n1 + n2;
                            nodec = r;
                                break;
                            case '-': r = n1 - n2;
                            nodec = r;
                                break; 
                            case 'x': r = n1*n2;
                            nodec = r;
                                break;
                            case '/': 
                                if(n2!=0){
                                    if(n1!=0){
                                    r = n1/n2;
                                    nodec = n1/n2;
                                }
                                }else{
                                    if(n1 != 0 & n2 == 0) r = 1000;
                                    else if(n1 == 0 & n2 == 0) r = 1001;
                                }
                                break;
                            default:
                                r = 0;
                                break;
                            }
                            EscribeLCD_c(r+'0');
                            }
                            DireccionaLCD(0x84);
                            if (nodec-r){
                                partdecl = (nodec-r)*100;
                                EscribeLCD_c(r+'0');
                                EscribeLCD_c('.');
                                EscribeLCD_c(partdecl+'0');
                            }
                            else if(r>0x51 & r!=1000 & r !=1001){
                                EscribeLCD_c('-');
                                DireccionaLCD(0x85);
                                r = ~r+1;
                                EscribeLCD_c(r+'0'); 
                            }else{
                                if(r>0xA & r!=1000 & r !=1001){
                                    do{
                                        i = i+1;
                                        r = r - 10;
                                    }while(r>0xA);
                                    EscribeLCD_c(i+'0');
                                    DireccionaLCD(0x85);
                                    EscribeLCD_c(r+'0'); 
                                }else{
                                    if(r==1000){
                                            EscribeLCD_c('I');
                                            EscribeLCD_c('N');
                                            EscribeLCD_c('F');
                                    }else{
                                        if(r=1001){
                                            EscribeLCD_c('I');
                                            EscribeLCD_c('N');
                                            EscribeLCD_c('D');
                                        }else{
                                            EscribeLCD_c(r+'0'); 
                                        }
                                    }
                                }
                            }
                        }
                    else{
                        if(n1c==' '){
                        EscribeLCD_c(Tecla);
                        n1c=Tecla;
                        n1 = Tecla-'0';
                    }else{
                        EscribeLCD_c(Tecla);
                        n2c=Tecla;
                        n2 = Tecla-'0';
                    }
                    }
                }
                    }     
        }
        __delay_ms(700);
    }
    }}
    
unsigned char LeerTeclado(void){
    while(RB4==1 && RB5==1 && RB6==1 && RB7==1);
    LATB=0b11111110;
    if(RB4==0) return '1';
    else if(RB5==0) return '2';
    else if(RB6==0) return '3';
    else if(RB7==0) return '+';
    else{
    LATB=0b11111101;
    if(RB4==0) return '4';
    else if(RB5==0) return '5';
    else if(RB6==0) return '6';
    else if(RB7==0) return '-';
    else{
    LATB=0b11111011;
    if(RB4==0) return '7';
    else if(RB5==0) return '8';
    else if(RB6==0) return '9';
    else if(RB7==0) return '/';
    else{
    LATB=0b11110111;
    if(RB4==0) return 'C';
    else if(RB5==0) return '0';
    else if(RB6==0) return '=';
    else if(RB7==0) return 'x';
    }}}
    return '?';
}
/*//Primera seccion
#include<xc.h>
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
/*    RBIF=0;
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
        __delay_ms(100);
        
        MensajeLCD_Var(Tecla);
    }    
}

unsigned char LeerTeclado(void){
    /*LATB = 0b00000000;
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
    return '?';*/
    /*while(RB4==1 && RB5==1 && RB6==1 && RB7==1);//se bloquea al micro mientras no se oprima alg?n bot?n 
    LATB=0b11111110;
    if(RB4==0) return '1';
    else if(RB5==0) return '2';
    else if(RB6==0) return '3';
    else if(RB7==0) return '+';
    else{
    LATB=0b11111101;
    if(RB4==0) return '4';
    else if(RB5==0) return '5';
    else if(RB6==0) return '6';
    else if(RB7==0) return '-';
    else{
    LATB=0b11111011;
    if(RB4==0) return '7';
    else if(RB5==0) return '8';
    else if(RB6==0) return '9';
    else if(RB7==0) return '/';
    else{
    LATB=0b11110111;
    if(RB4==0) return 'C';
    else if(RB5==0) return 0;
    else if(RB6==0) return '=';
    else if(RB7==0) return 'x';
    }}}
    LATB=0b11110000;
    return '?';
}
void interrupt ISR(void){
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
*/
