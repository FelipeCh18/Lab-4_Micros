#include<xc.h> //Libreria para PIC
#include<stdio.h> //Libreria adicional para manejo de decimales
#include<stdbool.h> //Libreria adicional para manejo de booleanos
#define _XTAL_FREQ 1000000 //Frecuencia de reloj
#include "LibLCDXC8.h" //Libreria para uso de LCD
#pragma config FOSC=INTOSC_EC //Sentencia para usar oscilador externo
#pragma config WDT=OFF //Apagar el perro guardian
#pragma config PBADEN=OFF //Apagar funciones análogas
#pragma config LVP=OFF //Apagar el modo de bajo consumo
#pragma config MCLRE=ON //Encender el MCLR

unsigned char Boton = 0; //Variable para leer Teclado
unsigned long Parte_Decimal = 0; //Parte decimal de un resultado
long int Resultado = 0; //Resultado entero de las operaciones
unsigned int Numero_1 = 0; //Primer numero
unsigned int Numero_2 = 0; //Segundo numero
unsigned int i = 0;
unsigned long m = 0;
unsigned char Operador = ' '; //Operador
unsigned char Numero_1-Char = ' '; //Primer numero en caracter
unsigned char Numero_2-Char = ' ';  //Segundo numero en caracter
unsigned int Color = 0;
int Contador = 0;
int Verificador = 0;
bool Potencia = false;
bool Factorial = false;
int a;
int b;
unsigned char LeerTeclado(void);//Declarar funcion para lectura de matricial
unsigned long factorial_function(int);//Declarar funcion para calcular el Factorial
void ColorRGB(void);//Declarar funcion para escritura de colores del RGB
unsigned char heart[]={0,27,31,31,14,4,0,0};
unsigned char persona[]={14,14,4,31,4,4,10,17};
void Imprimir_Resultado(long);//Declarar funcion para imprimir cada resultado


void main(void){
    ADCON1=15; //Deshabilitar funciones analogicas del puerto E
    TRISD=0; //Colocar puerto D como salida
    TRISB=0b11110000; //Colocar puerto B salida/entrada 50/50
    TRISE=0; //Colocar puerto E como salida
    TRISC=0; //Pines para RGB
    RBPU=0; //Activar resistencias pull up
    ConfiguraLCD(4);//Configurar LCD para operacion en 4 bits
    InicializaLCD(); //Funcion para configuracion inicial del LCD
    //Timer0 interrupcion
    T0CON=0b10000010;//Configuracion del modo temporizador y del prescaler
    TMR0IF=0;//Bandera del Timer0
    TMR0=49911;//Precarga del Timer0
    TMR0IE=1;//Habilitacion del Timer0
    GIE=1;//Habilitacion global de interrupciones
    TMR0ON=1;//Encendido del Timer0
    //Fin de configuracion para Timer0
    //Para bajo consumo
    OSCCON = 0b11000100;//Configuracion del oscilador para entrar en modo IDLE ante instruccion SLEEP y ajuste de frecuencia
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
    NuevoCaracter(0,heart);
    NuevoCaracter(1,persona);
    EscribeLCD_c(0);
    __delay_ms(500);
    EscribeLCD_c(1);
    while(1){
        LATB=0b00000000;
        Verificador = 0;
        Boton = LeerTeclado();
        ColorRGB();
        if(Boton=='C'){ //Limpiar la pantalla si se presiona [1][4]  //Colocar el cursor en la primera posicion de segunda fila
            //Limpiar variables
            Operador = ' ';
            Numero_1-Char = ' ';
            Numero_2-Char = ' ';
            Numero_1 = 0;
            Numero_2 = 0;
            i = 0;
            Parte_Decimal = 0;
            BorraLCD();
        }else if(Operador==' ' & (Boton=='+'|Boton=='^') & Numero_1-Char == ' ' & Numero_2-Char ==' '){
            Potencia=~Potencia;
            if(Potencia){
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
        }else if(Operador==' ' & (Boton=='-'|Boton=='!') & Numero_1-Char == ' ' & Numero_2-Char ==' '){
            Factorial=~Factorial;
            if(Factorial){
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
            if((Numero_1-Char==' '|Numero_2-Char==' '|Operador==' ') & !Factorial){ //Funciones si no se ha recibido nada
                if(Operador==' ' & (Boton=='+'|Boton=='-'|Boton=='/'|Boton=='x'|Boton=='^') & Numero_1-Char != ' ' & Numero_2-Char ==' '){
                    DireccionaLCD(0x81);
                    EscribeLCD_c(Boton);
                    Operador = Boton;
                }else if(Numero_1-Char == ' ' & (Boton!='+'|Boton!='-'|Boton!='/'|Boton!='x'|Boton!='='|Boton!='C'|Boton!='^')){
                    DireccionaLCD(0x80);  
                    EscribeLCD_c(Boton);
                    Numero_1-Char=Boton;
                    Numero_1 = Boton-'0';
                }else if(Numero_2-Char==' ' & Operador!=' ' & Numero_1-Char != ' ' & (Boton!='+'|Boton!='-'|Boton!='/'|Boton!='x'|Boton!='='|Boton!='C'|Boton!='^')){
                    DireccionaLCD(0x82);
                    EscribeLCD_c(Boton);
                    Numero_2-Char=Boton;
                    Numero_2 = Boton-'0';
                }
            }else if ((Numero_1-Char==' '|Operador==' ') & Factorial){
                if(Operador==' ' & (Boton=='+'|Boton=='-'|Boton=='/'|Boton=='x'|Boton=='^'|Boton=='!') & Numero_1-Char != ' ' & Numero_2-Char ==' '){
                    DireccionaLCD(0x81);
                    EscribeLCD_c(Boton);
                    Operador = Boton;
                }else if(Numero_1-Char == ' ' & (Boton!='+'|Boton!='-'|Boton!='/'|Boton!='x'|Boton!='='|Boton!='C'|Boton!='^'|Boton!='!')){
                    DireccionaLCD(0x80);  
                    EscribeLCD_c(Boton);
                    Numero_1-Char=Boton;
                    Numero_1 = Boton-'0';
                }
            }else if((Boton=='=' & Numero_2-Char!=' ' & Operador!=' ' & Numero_1-Char != ' ') & !Factorial){
                DireccionaLCD(0x83);
                EscribeLCD_c('=');
                switch(Operador){
                case '+': 
                    Resultado = Numero_1 + Numero_2;
                    Parte_Decimal = Resultado*100;
                    break;
                case '-':
                    Resultado = Numero_1 - Numero_2;
                    Parte_Decimal = Resultado*100;
                    break; 
                case 'x': 
                    Resultado = Numero_1*Numero_2;
                    Parte_Decimal = Resultado*100;
                    break;
                case '/': 
                    if(Numero_2!=0){
                        if(Numero_1!=0){
                        Resultado = Numero_1/Numero_2; //Parte entera
                        Parte_Decimal = (Numero_1*100)/Numero_2; //((Numero_1/Numero_2)-r)*10
                        }
                    }else{
                        if(Numero_1 != 0 & Numero_2 == 0) Resultado = 1000;
                        else if(Numero_1 == 0 & Numero_2 == 0) Resultado = 1001;
                        Parte_Decimal = Resultado*100;
                    }
                    break;
                case '^':
                    Resultado=Numero_1;
                    for(i=1;i<Numero_2;i++){
                        Resultado=Resultado*Numero_1;
                    }
                    Parte_Decimal = Resultado*100;
                    break;   
                default:
                    Resultado = 0;
                    break;
                }
                DireccionaLCD(0x84);
                Imprimir_Resultado(Resultado);
            }else if((Boton=='=' & Operador=='!' & Numero_1-Char != ' ') & Factorial) {
                DireccionaLCD(0x82);
                EscribeLCD_c('=');
                Resultado=factorial_function(Numero_1);
                Parte_Decimal = Resultado*100;
                Imprimir_Resultado(Resultado);
            }else{
                EscribeLCD_c('W');
            }
        __delay_ms(700);
        }
    }
}

    
unsigned char LeerTeclado(void){
    while(RB4==1 && RB5==1 && RB6==1 && RB7==1);
    Verificador = 1;
    LATB=0b11111110;
    if(RB4==0){
        if(!Potencia) return '+';
        else if(Potencia) return '^';
    }
    else if(RB5==0) return '=';
    else if(RB6==0) return '0';
    else if(RB7==0) return 'C';
    else{
    LATB=0b11111101;
    if(RB4==0){
        if(!Factorial) return '-';
        else if(Factorial) return '!';
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
    if(Color==8) Color = 0;
    switch(Color){
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
    Color += 1;
}

unsigned long factorial_function(int a){
    long fact = 1;
    for (b = a; b > 1; b--){
        fact = fact * b;
    }
    
    return fact;
}

void __interrupt() ISR(void){
    if(TMR0IF==1){
        if(!Verificador) Contador = Contador +1;
        else Contador = 0;
        TMR0IF=0;
        LATE2 = !LATE2;
        TMR0 = 49911;
    }
    
    if(Contador == 20){
        if(!Verificador){   
            LATC7 = !LATC7;
            BorraLCD();
            DireccionaLCD(0x82);
            MensajeLCD_Var("LVP Activado");
            SLEEP();
            while(1);
        }
    }
    
}

void Imprimir_Resultado(long Resultado){
    long Potencia = 10;
    bool flag=false;
    if ((Parte_Decimal-(Resultado*100))>0 | (Parte_Decimal-(Resultado*100))<0){
        for (int i = 0; i < 3; i++) {
            Resultado = Numero_1 / Numero_2;
            m = Numero_1 % Numero_2;
            EscribeLCD_c(Resultado+'0');
            if(i == 0 && m != 0) {
                EscribeLCD_c(',');
            }
                        
            if (m != 0) {
                Numero_1 = m * 10;
            } else {
                break;
            }
                                              
        }
    }else{
        if(Resultado>0x8000 & Resultado!=1000 & Resultado !=1001){
            EscribeLCD_c('-');
            DireccionaLCD(0x85);
            Resultado = ~Resultado+1;
            EscribeLCD_c(Resultado+'0'); 
        }else{
            if(Resultado>=0xA & Resultado!=1000 & Resultado !=1001){
                for(int j=9;j>=0;j--){
                    Potencia = 10;
                    for(int k=j-1;k>0;k--){   
                        Potencia = Potencia*10;
                    }
                    if(Resultado/Potencia>0){
                        flag = true;
                        EscribeLCD_c(Resultado/Potencia+48);
                        Resultado=Resultado%Potencia;
                    }else if( (Resultado/Potencia)==0 && flag){
                        if(Resultado<10) EscribeLCD_c(Resultado+48);
                        else EscribeLCD_c(48);
                    }
                }
            }else{
                if(Resultado==1000){
                    MensajeLCD_Var("Infinito");
                }else{
                    if(Resultado==1001){
                        DireccionaLCD(0xC1);
                        MensajeLCD_Var("Indeterminado");
                    }else{
                        EscribeLCD_c(Resultado+'0'); 
                    }
                }
            } 
        }
    }
}