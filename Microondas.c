/* 
 * File:   Microondas.c
 * Author: Eder/Luis
 *
 * Created on 8 de Janeiro de 2021, 9:30
 */

#include <stdio.h>
#include <stdlib.h>
#include "configuracao.h"
#include <pic18f4520.h>
#include "lcd.h"

/*
 * 
 */
unsigned char unid=0, dez=0, cent=0, mil=0;                                              //variaveis para armazenamento de segundos e minutos
void display (unsigned char u, unsigned char d, unsigned char c, unsigned char m);       //variaveis para armazenamento de segundos e minutos no mapeamento 
char tempo[6];
char cont_tempo=0;

#define _XTAL_FREQ 4000000
#define botao1 PORTCbits.RC1                                //Acrescenta Minutos
#define botao2 PORTCbits.RC2                                //Acrescenta Segundos
#define botao3 PORTCbits.RC3                                //Inicia timer (esquentando)
#define botao  PORTCbits.RC0                                //cancela operação
#define led1   PORTBbits.RB0                                //led indicando os pulsos por segundo
#define led2   PORTBbits.RB6                                //led indicando fim do timer
#define led3   PORTBbits.RB7                                //led indicando Motor
#define tempo_7 10

void main(void) 

{
    config();
    PORTA=0;
    PORTB=0;
    PORTD=0;
    PORTC=0;
    PORTE=0;
    TMR0=6;
    PORTAbits.RA5=1;                                          //ascende LCD
    lcd_init();                                               //inicia LCD
    display(unid, dez, cent, mil);                            //mostra conteudo do Display
    lcd_clear();                                              //limpa lcd
    lcd_puts("Microondas");                                   //mostra o conteudo do LCD
    unid=0, dez=0, cent=0, mil=0;                             //zera conteudo das variáveis do Display
    
    while(1)                                                  //LOOP PRINCIPAL
    {
        display(unid, dez, cent, mil);                        //mostra conteudo do Display
        
        if(!botao1)                                           //ACRESCENTA MINUTOS
        {
            while(!botao1){display(unid, dez, cent, mil);}    //clique no botão1 da placa
            cent++;                                           //incrementa centena
            if(cent>9)                                        //estouro de centena
            {
                cent=0;                                       //zera centena
                mil++;                                        //incrementa milhar
            }
            if(cent==0 && mil==2)                             //looping com limite em 20 minutos
            {
                cent=0;                                       //atribui 0 a centena
                mil=0;                                        //atribui 0 ao milhar
            }
            display(unid,dez,cent,mil);                       //mostra conteudo do Display
        }
        
        if(!botao2)                                           //ACRESCENTA SEGUNDOS
        {
            while(!botao2){display(unid, dez, cent, mil);}    //clique no botão2 da placa
            unid++;                                           //incrementa unidade
            if(unid>9)                                        //estouro de unidade
            {
                unid=0;                                       //zera unidade
                dez++;                                        //incrementa dezena
            }
            if(unid==0 && dez==6)                             //estouro de segundos
            {
                unid=0;                                       //atribui 0 a centena
                dez=0;                                        //atribui 0 ao milhar
            }
            display(unid,dez,cent,mil);                       //mostra conteudo do Display
        }

        if(!botao3)                                           //INICIA TIMER (Botão Confirma)
        {
            while(!botao3){display(unid, dez, cent, mil);}    //clique no botão3 da placa
            TMR0ON=!TMR0ON;                                   //inicia TIMER0
            lcd_clear();                                      //limpa LCD
            lcd_puts("Esquentando!");                         //Indica que está esquentando
            display(unid,dez,cent,mil);                       //mostra conteudo do Display      
            led3=!led3;                                       //led3 - Motor Acionado
        }
        
        if(!botao)                                            //CANCELAR OPERAÇÃO
        {
            while(!botao){display(unid, dez, cent, mil);}     //clique no botão0 da placa
            led1=0;                                           //apaga led1
            led3=!led3;                                       //apaga led3 (Motor)
            unid=0;                                           //zera variáveis
            dez=0;
            cent=0;
            mil=0;
            TMR0ON=0;                                         //reset TIMER0
            display(unid,dez,cent,mil);                       //mostra conteudo do Display
            
            lcd_clear();                                      //limpa LCD
            lcd_puts("Operacao");                             //indica no LCD que a Operação foi cancelada
            lcd_goto(0x40);
            lcd_puts("Cancelada");
            
            __delay_ms(2000);
            lcd_clear();                                      //limpa lcd
            lcd_puts("Microondas");                           //mostra o conteudo do LCD
            
       } 
         
    }//end while

}//end main 

void atualiza (void)                                //TRATAMENTO DE SEGUNDO E MINUTOS A CADA PULSO DE CLOCK
{
    
    if(unid==0 && dez>0 )                           //estouro de unidade
    {
        unid=10;                                    //atribui 10 a unidade e 9 contando com o decremento  
        dez--;                                      //decrementa dezena após estoudo de unidade
    }
    
    if(unid==0 && dez==0 && cent>0)                 //estouro de dezena
    {
        unid=10;                                    //atribui 10 a unidade e 9 contando com o decremento  
        dez=5;                                      //atribui 5 a dezena após estouro de dezena
        cent--;                                     //decrementa centena após estouro de dezena  
    }
    
    if(unid==0 && dez==0 && cent==0 && mil>0)       //estouro de centena
    {
        unid=10;                                    //atribui 10 a unidade e 9 contando com o decremento 
        dez=5;                                      //atribui 5 a dezena após estouro de centena
        cent=9;                                     //atribui 9 a centena após estouro de centena
        mil--;                                      //decrementa milhar
    }
    
    if(unid==0 && dez==0 && cent==0 && mil==0)      //estouro de milhar
    {
        unid=10;                                    //atribui 10 a unidade e 9 contando com o decremento  
        dez=5;                                      //atribui 5 a dezena após estouro de milhar
        cent=9;                                     //atribui 9 a centena após estouro de milhar
    }
    
    unid--;                                         //decrementa unidade
    
    if(unid==0 && dez==0 && cent==0 && mil==0 )     //condição de parada
    {
        led3=!led3;                                 //apaga led3 (Motor)
        lcd_clear();                                //limpa conteudo do lcd
        lcd_puts("BIP! BIP!");                      //escreve no LCD 
        led2=!led2;                                 //acende led2
        unid=0, dez=0, cent=0, mil=0;               //zera variáveis de amostragem display
        display(unid, dez, cent, mil);              //mostra conteudo do display
        TMR0ON=0;                                   //Para o TIMER0
        
        __delay_ms(3000);
        led2=!led2;                                       //apaga led2
        lcd_clear();                                      //limpa lcd
        lcd_puts("Microondas");                           //mostra o conteudo do LCD
    }  
    display(unid, dez, cent, mil);                  //mostra conteudo do display
    
}//end void atualiza


void display (unsigned char u,unsigned char d,unsigned char c,unsigned char m)  //MAPEAMENTO/ATUALIZAÇÃO DO DISPLAY
{
    const char tabela []={
    
    /*0*/   0B00111111,
    /*1*/   0B00000110,
    /*2*/   0B01011011,
    /*3*/   0B01001111,
    /*4*/   0B01100110,
    /*5*/   0B01101101,
    /*6*/   0B01111101,
    /*7*/   0B00000111,
    /*8*/   0B01111111,
    /*9*/   0B01101111,                
    };
    
    PORTD=tabela[u];
    LATB3 = 1;                     //sintaxe para definir bit do port através do Latch do port
    __delay_ms(10);
    LATB3 = 0;  
    PORTD=tabela[d];
    LATB4 = 1;    
    __delay_ms(10);
    LATB4 = 0;
    PORTD=tabela[c];
    LATB2 = 1;
    __delay_ms(10);
    LATB2 = 0;
    PORTD=tabela[m];
    LATB1 = 1;
    __delay_ms(10);
    LATB1 = 0;
}//end void display

void __interrupt(high_priority) temp (void) //ROTINA DE INTERRUPÇÃO
{
    if(TMR0IF)              //verifica se a TMR0IF (flag de estouro) está ativa
    {
        TMR0IF=0;           //limpa a flag TMR0IF (flag de estouro) 
        cont_tempo++;       //incrementa variavel auxiliar
        TMR0=6;             //reinicia timer TMR0
        if(cont_tempo>250)  //verifica se a varivel auxiliar chegou no estouro de bits
        {
            cont_tempo=0;   //zera variavel auxiliar
            led1=!led1;     //pisca led1
            atualiza();     //atualiza conteudo do Display
        }
    }
}//end void __interrupt(high_priority)