#ifndef ADD_FINGER_H
#define ADD_FINGER_H
#define CONFIRM RB3
extern unsigned char current_ID;
extern const char GenImg[12];
extern const char ImgToChar_1[13];
extern const char ImgToChar_2[13];
extern const char RegModel[12];

extern unsigned char GIRP[12];//GIRP matlab GenImgRecievedPacket
extern unsigned char ITCRP[12];//ITCRP matlab ImgToCharRecievedPacket
extern unsigned char RMRP[12];//RMRP matlab RegisterModelRecievedPacket
extern unsigned char SRP[12];//SRP matlab StoreRecievedPacket

void ADD_FINGER(){
  
    if (CONFIRM == 1) 
{
    __delay_ms(50);
    if (CONFIRM == 1) 
    {
        while (CONFIRM == 1); 
                return ;

    }
}
    restart: 
    if (CONFIRM == 1) 
{
    __delay_ms(50);
    if (CONFIRM == 1) 
    {
        while (CONFIRM == 1); 
                return ;

    }
}
    
    LCD_CMD(0x01);
    if(current_ID>=9){
        sendString("LIMIT REACHED");
        return;
    }
 

    sendString("PLACE FINGER");
    jump:
    if (CONFIRM == 1) 
{
    __delay_ms(50);
    if (CONFIRM == 1) 
    {
        while (CONFIRM == 1); 
                return ;

    }
}
    //step1
    SEND_TX(GenImg,12);
    
    for(int i=0;i<12;i++){
        GIRP[i]=UART_READ();
    }
    
  
        if(GIRP[9]!=0X00){
         LCD_CMD(0x01);
         sendString("TRY AGAIN");
         __delay_ms(2000);
            goto jump;
    }
    //step2
    SEND_TX(ImgToChar_1,13);
    for(int i=0;i<12;i++){
        ITCRP[i]=UART_READ();
    }
    
   
        if(ITCRP[9]!=0X00){
            goto restart;
        
    }
    //step3
  
    LCD_CMD(0x01);
    sendString("REMOVE FINGER");
    __delay_ms(2000);
    
    //step4
    again:
    LCD_CMD(0x01);
    sendString("PLACE FINGER");
    
    SEND_TX(GenImg,12);
    for(int i=0;i<12;i++){
        GIRP[i]=UART_READ();
    }
    

        if(GIRP[9]!=0X00){
         LCD_CMD(0x01);
         sendString("TRY AGAIN");
         __delay_ms(2000);
         goto again;
        
    }
    //step5
    SEND_TX(ImgToChar_2,13);
    for(int i=0;i<12;i++){
        ITCRP[i]=UART_READ();
    }
    
  
        if(ITCRP[9]!=0X00){
            goto restart;
        
    }
    //step6
        SEND_TX(RegModel,12);
     for(int i=0;i<12;i++){
        RMRP[i]=UART_READ();
    }
    

        if(RMRP[9]!=0X00){
            goto restart;
        
    }    
        
   //step7      
    
    unsigned char id_L = current_ID; 
    unsigned char sum_L = 14 + current_ID;

    unsigned char Store[15] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x06,0x06,0x01, 0, id_L, 0, sum_L};
    
    SEND_TX(Store, 15);

    for(int i=0; i<12; i++){
        SRP[i] = UART_READ();
    }

    LCD_CMD(0x01);
    if(SRP[9] == 0x00){
        sendString("SAVED");
        current_ID++;
    }
    else {
        sendString("ERROR SAVING");
    }
    __delay_ms(2000);
}
#endif
