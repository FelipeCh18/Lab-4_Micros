#include <xc.h>
#include <stdio.h>//le permite al compilador buscar la librería del micro que esté trabajando
#define _XTAL_FREQ 1000000 //Hay que definir la frecuencia de reloj 1MHz
//Segunda sección -- Configuración
#include "LibLCDXC8.h"
#pragma config FOSC=INTOSC_EC //Oscilador
#pragma config WDT=OFF
#pragma config PBADEN = OFF//Perro guardian off
#pragma config LVP=OFF
//Tercera sección -- variables globales
//unsigned char a;
//void __interrupt(low_priority) ISRL(void);
unsigned char LeerTeclado(void); 
unsigned char puerto;
unsigned char first = ' ';
unsigned char op = ' ';
unsigned char second = ' ';
unsigned char selection = ' ';

int firstInt = 0;
int secondInt = 0;
int resInt = 0;

double resDouble = 0;
double decimalsDouble = 0;

char res[200];
char calc[16]= "Calculadora";
char borrar[16]= "                ";
char error[16] = "Error";
void main(void){
    //RBIE --> bit de máscara
    //RBIF --> bit de bandera
    //RBI¨--> bit de configuración de prioridad
    //RB4 a RB7 son los pines de interrupción, deben ser configurados como ENTRADAS o si no se deshabilita la interrupción
    /* Sin importar por cual pin se detecte el cambio (flanco de subida o bajada) se dispara el mismo evento de interrupción
      El puerto B permite habilitar una resistencia de pull-up para eliminar la alta impedancia -- bit RBPU -- 0 las habilita, y 1 las deshabilita
     Con 0 habilita resistencias de Pull-up para TODO el puerto B
     Se recomienda colocar un retardo después de habilitarlas -- para asegurar la estabilización de la habilitación del pullup
     Consideraciones: 
     -- Se debe discriminar cual de los dos eventos dispara la interrupción (flanco de subida o bajada)
     -- Se debe poner un código para buscar cuál de los pines cambió -- algoritmo de búsqueda
     -- Para borrar la bandera de interrupción se debe leer primero el puerto B
     -- cargar una variable con el valor del puerto b para borrar la bandera*/
    
    //Ejemplo para configurar pines y la interrupción y se espere la interrupción
    
    ADCON1 = 15; //Quita todas las funciones análogas
    
    //Configuración puertos/pines como entradas o salidas
    TRISD = 0;
    TRISE = 0;
    TRISB = 0b11110000;
    
    //Configuración de los valores iniciales de los puertos de salida
    LATD = 0;
    LATE = 0;
    LATB = 0;
    __delay_ms(100);
    RBPU = 0;           //Resistencias PullUp
    __delay_ms(100);    //Delay transición
      
    //Configuración del TIMER0 para un led de funcionamiento intermitente 1 segundo y del Puerto B para interrupciones.
        //Timer
    T0CON = 0b00000001; //ver Datasheet. Preset = 4
    TMR0 = 3036;        //Precarga
    TMR0IF = 0;         //Interrupt flag
    TMR0IE = 1;         // Interrupt enable    
    GIE = 1;            //Global Interrupt enable
    TMR0ON = 1;         //Enable el Timer0
  
    //Configuración LCD
    __delay_ms(1000); //delay para estabilizar la configuración del PIC
    ConfiguraLCD(4);
    InicializaLCD();
    BorraLCD();
    MensajeLCD_Word(calc);
    
    DireccionaLCD(0xC0);
    
    while(1){
        selection = LeerTeclado();
        __delay_ms(50); //delay para evitar multiples pulasciones de una misma tecla por rebote
        if(selection == 'e'){ //Si se detectó "borrar"
            first = ' ';
            op = ' ';
            second = ' ';
            DireccionaLCD(0xC0);
            MensajeLCD_Word(borrar);
            DireccionaLCD(0xC0);
        }else if(selection == '-' || selection == '+' || selection == '*' || selection == '/'){ //Si se detectó una operación
            op = selection;
            DireccionaLCD(0xC1);
            EscribeLCD_c(op);
        }else if(selection == '='){ //Si se detecta "igual"
            __delay_ms(50);
           if(first != ' ' && op != ' ' && second != ' '){
               firstInt = first - '0';  //convierte a enteros
               secondInt = second - '0';
               
               DireccionaLCD(0xC3);  
               EscribeLCD_c('=');

               if(op == '-'){ //Resta
                   resDouble = firstInt-secondInt;
               }
               if(op == '+'){ //Suma
                   resDouble = firstInt+secondInt;
               }
               if(op == '/'){ //División
                   if(second != '0' && secondInt != 0){ //Verificación de división por 0
                   resDouble = (double)firstInt/(double)secondInt;
                   resInt = firstInt/secondInt;     //Muestra o no decimales según el resultado
                   decimalsDouble = resDouble - resInt;
                   }
               }
               if(op == '*'){ //multiplicaión
                   resDouble = firstInt*secondInt;
               }
               if(op == '/'){ //Ajusta la visualización del resultado
                   if(second == '0' || secondInt == 0){ //Si hay división en 0, da error.
                       sprintf(res, "%s", error); 
                   }
                   else if(decimalsDouble != (double)0){ //Si la división no es exacta muestra decimales.
                     sprintf(res, "%1.8g", resDouble);   
                   }else{
                     sprintf(res, "%d", resInt); //Si es exacta solo muestra un entero.
                   }    
               } 
               else{
                   sprintf(res, "%1.0f", resDouble); //Para todas las demás operaciones solo muestra enteros.
               }
               
               DireccionaLCD(0xC4);
               MensajeLCD_Word(res); 
                    
             }else{}
        }else{
            if ( first == ' '){
                first = selection;
                __delay_ms(120); //delay anti-rebote nuevamente
                DireccionaLCD(0xC0);
                EscribeLCD_c(first);
            } 
            else if( first != ' ' && second == ' '){
                second = selection; 
                __delay_ms(10);
                DireccionaLCD(0xC2);
                EscribeLCD_c(second);
            }
            if(first != ' ' && second != ' '){
                __delay_ms(10);
            }   
        } 
    }  
}
unsigned char LeerTeclado(void){
    LATB = 0b00000000;
    while(PORTB == 0b11110000){
    }
    LATB = 0b00000111; //fila 1
    puerto = PORTB;
    if ((puerto&0b11110000) == 0b11100000){ //columna 1
        return '1';
    }
    if ((puerto&0b11110000) == 0b11010000){ //columna 2
        return '2';
    }
    if ((puerto&0b11110000) == 0b10110000){ //columna 3
        return '3';
    }
    if ((puerto&0b11110000) == 0b01110000){ //columna 4
        return '+';
    }
    
    LATB = 0b00001011; //fila 2
    puerto = PORTB;
    if ((puerto&0b11110000) == 0b11100000){ //columna 1
        return '4';
    }
    if ((puerto&0b11110000) == 0b11010000){ //columna 2
        return '5';
    }
    if ((puerto&0b11110000) == 0b10110000){ //columna 3
        return '6';
    }
    if ((puerto&0b11110000) == 0b01110000){ //columna 4
        return '-';
    }
    
    LATB = 0b00001101; //fila 3
    puerto = PORTB;
    if ((puerto&0b11110000) == 0b11100000){ //columna 1
        return '7';
    }
    if ((puerto&0b11110000) == 0b11010000){ //columna 2
        return '8';
    }
    if ((puerto&0b11110000) == 0b10110000){ //columna 3
        return '9';
    }
    if ((puerto&0b11110000) == 0b01110000){ //columna 4
        return '/';
    }
    
    LATB = 0b00001110; //fila 4
    puerto = PORTB;
    if ((puerto&0b11110000) == 0b11100000){ //columna 1
        return 'e';
    }
    if ((puerto&0b11110000) == 0b11010000){ //columna 2
        return '0';
    }
    if ((puerto&0b11110000) == 0b10110000){ //columna 3
        return '=';
    }
    if ((puerto&0b11110000) == 0b01110000){ //columna 4
        return '*';
    }
    return '?';
}
void __interrupt() ISR(void){
    if(TMR0IF == 1){
        TMR0 = 3036;
        TMR0IF = 0;
        LATD0 = !LATD0;
    }
}

