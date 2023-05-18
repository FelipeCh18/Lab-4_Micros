#include<xc.h> //Libreria para PIC
#include<stdio.h> //Libreria adicional para manejo de decimales
#include<stdbool.h>
#include<math.h>
#define _XTAL_FREQ 1000000 //Frecuencia de reloj
#include "LibLCDXC8.h" //Libreria para uso de LCD
#pragma config FOSC=INTOSC_EC //Sentencia para usar oscilador externo
#pragma config WDT=OFF //Apagar el perro guardian
#pragma config PBADEN=OFF
#pragma config LVP=ON
#pragma config MCLRE=ON

unsigned char Tecla = 0; //Variable para leer teclado
unsigned long partdecl = 0; //Parte decimal de un resultado
unsigned long r = 0; //Resultado entero de las operaciones
unsigned int n1 = 0; //Primer numero
unsigned int n2 = 0; //Segundo numero
unsigned int i = 0;
unsigned long m = 0;
unsigned char op = ' '; //Operador
unsigned char n1c = ' '; //Primer numero en caracter
unsigned char n2c = ' ';  //Segundo numero en caracter
unsigned int color = 0;
int contador = 0;
int contador_digitos = 0;
int verificador = 0;
bool potencia = false;
bool factorial = false;
int a;
int b;
unsigned long res;
unsigned char LeerTeclado(void);
unsigned long factorial_function(int);//Declarar funcion para lectura de matricial
void ColorRGB(void);
void Imprimir_Resultado(long);

void main(void){
    ADCON1=15; //Deshabilitar funciones analogicas del puerto E
    TRISD=0; //Colocar puerto D como salida
    TRISB=0b11110000; //Colocar puerto B salida/entrada 50/50
    TRISE=0; //Colocar puerto E como salida
    TRISC=0; //Pines para RGB
    RBPU=0; //Activar resistencias pull up
    ConfiguraLCD(4);
    InicializaLCD(); //Funcion para configuracion inicial del LCD
    //Timer0 interrupcion
    T0CON=0b10000010;
    TMR0IF=0;
    TMR0=49911;
    TMR0IE=1;
    GIE=1;
    TMR0ON=1;
    //Fin de configuracion para Timer0
    //Para bajo consumo
    OSCCON = 0b11000100;
    //Fin de bajo conumo
    BorraLCD(); //Limpiar el LCD
    if(TO == 1 && PD == 1){
        BorraLCD();
        MensajeLCD_Var("Energia");
    }else{
        if(POR==0){
        BorraLCD();
        MensajeLCD_Var("MCLR");
    } 
    }
    __delay_ms(2000); //Retraso para evitar errores
    BorraLCD(); 
    while(1){
        LATB=0b00000000;
        verificador = 0;
        Tecla = LeerTeclado();
        ColorRGB();
        if(Tecla=='C'){ //Limpiar la pantalla si se presiona [1][4] 
            DireccionaLCD(0xC0); //Colocar el cursor en la primera posicion de segunda fila
            //Limpiar variables
            op = ' ';
            n1c = ' ';
            n2c = ' ';
            n1 = 0;
            n2 = 0;
            i = 0;
            partdecl = 0;
            BorraLCD();
        }else if(op==' ' & (Tecla=='+'|Tecla=='^') & n1c == ' ' & n2c ==' '){
            potencia=~potencia;
            if(potencia){
                DireccionaLCD(0x80); //Colocar el cursor en la primera posicion de primera fila
                MensajeLCD_Var("Modo Potencia"); //Mandar mensaje vacio para limpiar
                DireccionaLCD(0xC0);
                MensajeLCD_Var("Activado");
                __delay_ms(1000);
                BorraLCD();
                continue;
            }else{
                DireccionaLCD(0x80); //Colocar el cursor en la primera posicion de primera fila
                MensajeLCD_Var("Modo Suma"); //Mandar mensaje vacio para limpiar
                DireccionaLCD(0xC0);
                MensajeLCD_Var("Activado");
                __delay_ms(1000);
                BorraLCD();
                continue;
            }
        }else if(op==' ' & (Tecla=='-'|Tecla=='!') & n1c == ' ' & n2c ==' '){
            factorial=~factorial;
            if(factorial){
                DireccionaLCD(0x80); //Colocar el cursor en la primera posicion de primera fila
                MensajeLCD_Var("Modo Factorial"); //Mandar mensaje vacio para limpiar
                DireccionaLCD(0xC0);
                MensajeLCD_Var("Activado");
                __delay_ms(1000);
                BorraLCD();
                continue;
            }else{
                DireccionaLCD(0x80); //Colocar el cursor en la primera posicion de primera fila
                MensajeLCD_Var("Modo Resta"); //Mandar mensaje vacio para limpiar
                DireccionaLCD(0xC0);
                MensajeLCD_Var("Activado");
                __delay_ms(1000);
                BorraLCD();
                continue;
            }            
        }else{
            if((n1c==' '|n2c==' '|op==' ') & !factorial){ //Funciones si no se ha recibido nada
                if(op==' ' & (Tecla=='+'|Tecla=='-'|Tecla=='/'|Tecla=='x'|Tecla=='^') & n1c != ' ' & n2c ==' '){
                    DireccionaLCD(0x81);
                    EscribeLCD_c(Tecla);
                    op = Tecla;
                }else if(n1c == ' ' & (Tecla!='+'|Tecla!='-'|Tecla!='/'|Tecla!='x'|Tecla!='='|Tecla!='C'|Tecla!='^')){
                    DireccionaLCD(0x80);  
                    EscribeLCD_c(Tecla);
                    n1c=Tecla;
                    n1 = Tecla-'0';
                }else if(n2c==' ' & op!=' ' & n1c != ' ' & (Tecla!='+'|Tecla!='-'|Tecla!='/'|Tecla!='x'|Tecla!='='|Tecla!='C'|Tecla!='^')){
                    DireccionaLCD(0x82);
                    EscribeLCD_c(Tecla);
                    n2c=Tecla;
                    n2 = Tecla-'0';
                }
            }else if ((n1c==' '|op==' ') & factorial){
                if(op==' ' & (Tecla=='+'|Tecla=='-'|Tecla=='/'|Tecla=='x'|Tecla=='^'|Tecla=='!') & n1c != ' ' & n2c ==' '){
                    DireccionaLCD(0x81);
                    EscribeLCD_c(Tecla);
                    op = Tecla;
                }else if(n1c == ' ' & (Tecla!='+'|Tecla!='-'|Tecla!='/'|Tecla!='x'|Tecla!='='|Tecla!='C'|Tecla!='^'|Tecla!='!')){
                    DireccionaLCD(0x80);  
                    EscribeLCD_c(Tecla);
                    n1c=Tecla;
                    n1 = Tecla-'0';
                }
            }else if((Tecla=='=' & n2c!=' ' & op!=' ' & n1c != ' ') & !factorial){
                DireccionaLCD(0x83);
                EscribeLCD_c('=');
                switch(op){
                case '+': 
                    r = n1 + n2;
                    partdecl = r*100;
                    break;
                case '-':
                    r = n1 - n2;
                    partdecl = r*100;
                    break; 
                case 'x': 
                    r = n1*n2;
                    partdecl = r*100;
                    break;
                case '/': 
                    if(n2!=0){
                        if(n1!=0){
                        r = n1/n2; //Parte entera
                        partdecl = (n1*100)/n2; //((n1/n2)-r)*10
                        }
                    }else{
                        if(n1 != 0 & n2 == 0) r = 1000;
                        else if(n1 == 0 & n2 == 0) r = 1001;
                        partdecl = r*100;
                    }
                    break;
                case '^':
                    r=pow(n1,n2);
                    partdecl = r*100;
                    break;   
                default:
                    r = 0;
                    break;
                }
                DireccionaLCD(0x84);
                Imprimir_Resultado(r);
            }else if((Tecla=='=' & op=='!' & n1c != ' ') & factorial) {
                DireccionaLCD(0x82);
                EscribeLCD_c('=');
                r=factorial_function(n1);
                partdecl = r*100;
                DireccionaLCD(0x83);
                Imprimir_Resultado(r);
            }else{
                EscribeLCD_c('W');
            }
        __delay_ms(700);
        }
    }
}

    
unsigned char LeerTeclado(void){
    while(RB4==1 && RB5==1 && RB6==1 && RB7==1);
    verificador = 1;
    LATB=0b11111110;
    if(RB4==0){
        if(!potencia) return '+';
        else if(potencia) return '^';
    }
    else if(RB5==0) return '=';
    else if(RB6==0) return '0';
    else if(RB7==0) return 'C';
    else{
    LATB=0b11111101;
    if(RB4==0){
        if(!factorial) return '-';
        else if(factorial) return '!';
    }
    else if(RB5==0) return '9';
    else if(RB6==0) return '8';
    else if(RB7==0) return '7';
    else{
    LATB=0b11111011;
    if(RB4==0) return 'x';
    else if(RB5==0) return '6';
    else if(RB6==0) return '5';
    else if(RB7==0) return '4';
    else{
    LATB=0b11110111;
    if(RB4==0) return '/';
    else if(RB5==0) return '3';
    else if(RB6==0) return '2';
    else if(RB7==0) return '1';
    }
    }
    }
    return '?';
}

void ColorRGB(void){
    if(color==8) color = 0;
    switch(color){
        case 0:
            LATC = 0b00000000;
            break;
        case 1:
            LATC = 0b00000101;
            break;
        case 2:
            LATC = 0b00000110;
            break;
        case 3:
            LATC = 0b00000011;
            break;
        case 4:
            LATC = 0b00000010;
            break;
        case 5:
            LATC = 0b00000110;
            break;
        case 6:
            LATC = 0b00000100;
            break; 
        case 7:
            LATC = 0b00000111;
            break;   
        default:
            break;
    }
    color += 1;
}

unsigned long factorial_function(int a){
    int fact = 1;
    for (b = a; b > 1; b--){
        fact = fact * b;
    }
    return fact;
}

void __interrupt() ISR(void){
    if(TMR0IF==1){
        if(!verificador) contador = contador +1;
        else contador = 0;
        TMR0IF=0;
        LATE2 = !LATE2;
        TMR0 = 49911;
    }
    
    if(contador == 20){
        if(!verificador){   
            LATC7 = !LATC7;
            BorraLCD();
            DireccionaLCD(0x82);
            MensajeLCD_Var("LVP Activado");
            SLEEP();
        }
    }
    
}

void Imprimir_Resultado(long r){
    if ((partdecl-(r*100))>0 | (partdecl-(r*100))<0){
        for (int i = 0; i < 3; i++) {
            r = n1 / n2;
            m = n1 % n2;
            EscribeLCD_c(r+'0');
            if(i == 0 && m != 0) {
                EscribeLCD_c(',');
            }
                        
            if (m != 0) {
                n1 = m * 10;
            } else {
                break;
            }
                                              
        }
    }else if(r>0xFFFF & r!=1000 & r !=1001){
        DireccionaLCD(0x85);
        MensajeLCD_Var("Imposible");
        DireccionaLCD(0xC0);
        MensajeLCD_Var("de calcular");
    }else{
        if(r>=0xA & r!=1000 & r !=1001){
            contador_digitos=0;
            res=r;
            do{
                res=res/10;
            }while(res>1);
            EscribeLCD_n16(r,contador_digitos);
//            do{
//                i = i+1;
//                r = r - 10;
//            }while(r>=0xA);
//            EscribeLCD_c(i+'0');
//            DireccionaLCD(0x85);
//            EscribeLCD_c(r+'0');
//            int c, i, n;
//            int digitos[10];
//            unsigned long long int num;
//            num = 0;
//            for( n = 0; n < 10 && (c = getchar()) != '\n'; n++ ){
//                digitos[n] = (c - '0')+48;
//                num *= 10;
//                num += digitos[n];
//            }
//            MensajeLCD_Var(digitos);
        }else{
            if(r==1000){
                MensajeLCD_Var("Infinito");
            }else{
                if(r==1001){
                    DireccionaLCD(0xC1);
                    MensajeLCD_Var("Indeterminado");
                }else{
                    EscribeLCD_c(r+'0'); 
                }
            }
        }
    }
}