#ifndef SEARCH_FINGER_H
#define SEARCH_FINGER_H
#define CONFIRM RB3
extern unsigned char SearchRP[16];
unsigned char SEARCH_FINGER(){
const char string2[]={"TRY AGAIN"};
const char Search[]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x04,0x01,0x00,0x01,0x00,0x09,0x00,0x18};
LCD_CMD(0x01);
sendString("SCAN FINGER");
if (CONFIRM == 1) {
__delay_ms(50);
if (CONFIRM == 1){
while (CONFIRM == 1); 
return 0;
}
}
    jump:
    
if (CONFIRM == 1) {
__delay_ms(50);
if (CONFIRM == 1){
while (CONFIRM == 1); 
return 0;
}
}

    SEND_TX(GenImg,12);
    for(int i=0;i<12;i++){
        GIRP[i]=UART_READ();
    }
   
        if(GIRP[9]!=0X00){
            LCD_CMD(0x01);
         sendString("TRY AGAIN");
         __delay_ms(1000); 
         
         LCD_CMD(0x01);
         sendString("SCAN FINGER");
            goto jump;
        }
    
SEND_TX(ImgToChar_1,13);
for(int i=0;i<12;i++){
        ITCRP[i]=UART_READ();
    }
    if(ITCRP[9]!=0X00){
         sendString("TRY AGAIN");
            goto jump;
        }

    SEND_TX(Search,17);
   
    for(int i=0;i<16;i++){
        SearchRP[i]=UART_READ();
    }
    LCD_CMD(0x01);
    if(SearchRP[9]==0X00){
        return SearchRP[11];
    }
    else 
        return 0;


}
#endif
