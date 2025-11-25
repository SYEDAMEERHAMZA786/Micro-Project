/*ports used :
 RB0 for menu interrupt
 RB1 for up button
 RB2 for down button
 RB3 for finger interrupt
 RB4 to lock and unlock door
 
 RC0,RC1,PORTD for LCD
 */

#include <xc.h>
#include "LCD.h"
#include "UART_Communication.h"
#include "ADD_FINGER.h"
#include "SEARCH_FINGER.h"
#include "MENU.h"

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 4000000
#define UP    RB1
#define DOWN  RB2
#define CONFIRM RB3
#define TOUCH RB4
#define DOOR RB5

#define one RA0
#define two RA1
#define three RA2
#define four RA3
#define menu 4

const char GenImg[]={0XEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X03,0X01,0X00,0X05};//size12
const char GenImg_ACK[]={0XEF,0X01,0XFF,0XFF,0XFF,0XFF,0X07,0X00,0X03,0X00,0X00,0X0A};//12
const char ImgToChar_1[]={0XEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X04,0X02,0X01,0X00,0X08};//13
const char ImgToChar_2[]={0XEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X04,0X02,0X02,0X00,0X09};//13
const char ImgToChar_ACK[]={0XEF,0X01,0XFF,0XFF,0XFF,0XFF,0X07,0X00,0X03,0X00,0X00,0X0A};//12
const char RegModel[]={0XEF,0X01,0XFF,0XFF,0XFF,0XFF,0X01,0X00,0X03,0X05,0X00,0X09};//12
const char RegModel_ACK[]={0XEF,0X01,0XFF,0XFF,0XFF,0XFF,0X07,0X00,0X03,0X00,0X00,0X0A};//12

unsigned char GIRP[12];
unsigned char ITCRP[12];
unsigned char RMRP[12];
unsigned char SRP[12];
unsigned char SearchRP[15];
unsigned char open=0;
unsigned char current_ID=1;
unsigned char password=0; 

const char menu_string1[] = {"ADD FINGER"};
const char menu_string2[] = {"REMOVE FINGER"};
const char menu_string3[] = {"CHANGE PASSWORD"};
const char menu_string4[] = {"EXIT"};
unsigned char No_Finger[]={"INVALID"};

volatile unsigned char isactive = 0;
unsigned char count = 0;
unsigned char last_count = 255;

void LCD_CMD(unsigned char cmd);
void LCD_DATA(unsigned char data);
void sendString(const char *str);
void MENU_SCREEN(unsigned char var);

void __interrupt() ISR(void)
{
    if(INTF)
    {
        isactive = 1;
        count = 0;
        last_count = 255;
        INTF = 0;
    }
}

void main(void)
{
    UART_Init();
    TRISC0 = 0;
    TRISC1 = 0;
    TRISC2 = 0;
    TRISC3 = 0;
    TRISD = 0x00;
    PORTD = 0x00;
    
    TRISB0 = 1;
    TRISB1 = 1;
    TRISB2 = 1; 
    TRISB3 = 1; 


    ADCON1 = 0x06;
    CMCON = 0x07;
    
    __delay_ms(20);
    LCD_CMD(0x38); 
    LCD_CMD(0x0C); 
    LCD_CMD(0x06); 
    LCD_CMD(0x01); 
    __delay_ms(2);

    INTE = 1;
    INTF = 0;
    GIE = 1;
    INTEDG = 0;
    
    last_count = 255; 
    
    while(1)
    {
   
   if(TOUCH == 1){
       open=SEARCH_FINGER();
       if(open<=0 || open>9){
           sendString("INVALID");
       }
       else {
           
           DOOR=1;
           __delay_ms(10000);
           DOOR=0;
       }
   }
   //    ADD_FINGER(); 
   // __delay_ms(2000); // Show it for 2 seconds
   // LCD_CMD(0x01);
        if(isactive == 0)
        {
           
            if(last_count != 200) 
            {
                LCD_CMD(0x01);
                LCD_CMD(0x80);
                sendString("PRESS INT BTN");
                LCD_CMD(0xC0);
                sendString("TO START MENU");
                
                last_count = 200;
            }
        }

        else
        {
           
            if(count != last_count)
            {
                LCD_CMD(0x01); 
                LCD_CMD(0x80);
                
 
                MENU_SCREEN(count + 1); 
                
                last_count = count; 
            }
            if(UP == 1) 
            {
                __delay_ms(50); 
                if(UP == 1)
                {
                    if(count == 0) count = menu - 1;
                    else count--;
                    
                    while(UP == 1); 
                }
            }

            if(DOWN == 1)
            {
                __delay_ms(50); 
                if(DOWN == 1) 
                {
                    count++;
                    if(count >= menu) count = 0;
                    while(DOWN == 1); 
                }
            }
        }
    }

}










