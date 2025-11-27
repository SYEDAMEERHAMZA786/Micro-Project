#ifndef DELETE_FINGER_H
#define DELETE_FINGER_H
#define CONFIRM RB3
extern unsigned char DRP[12];
void delete_Fingerprint() 
{
 if (CONFIRM == 1) {
__delay_ms(50);
if (CONFIRM == 1){
while (CONFIRM == 1); 
return;
}
}
    unsigned char Delete[] = {0XEF, 0X01, 0XFF, 0XFF, 0XFF, 0XFF, 0X01, 0X00, 0X07, 0X0C, 0X00, 0X00, 0X00, 0X01, 0X00, 0X00};
    unsigned char id=SEARCH_FINGER();
    Delete[11]=id;
    if(id<=0 || id>9){
        LCD_CMD(0x01);
        sendString("NO MATCH!");
        __delay_ms(2000);
        return;
    }
    unsigned char sum=0X15+id;
    Delete[15]=sum;

    SEND_TX(Delete,16);
for(int i=0;i<12;i++){
        DRP[i]=UART_READ();
    }
    
    
        if(DRP[9]==0X00){
            LCD_CMD(0x01);
         sendString("DELETED!");
         __delay_ms(2000);
         return;
            
        }

    
}
#endif
