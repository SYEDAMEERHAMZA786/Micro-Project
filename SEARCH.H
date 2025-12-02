#ifndef SEARCH_FINGER_H
#define SEARCH_FINGER_H

#define CONFIRM RB3

extern unsigned char SearchRP[16];
extern unsigned char GIRP[12];
extern unsigned char ITCRP[12];
extern const char GenImg[12];
extern const char ImgToChar_1[13];

unsigned char SEARCH_FINGER() {
    const char Search[] = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF,0x01, 0x00, 0x08, 0x04, 0x01, 0x00,0x01, 0x00, 0x09, 0x00, 0x18 };
    
    unsigned char retry = 0;
    
    LCD_CMD(0x01);
    sendString("PLACE FINGER");
    __delay_ms(1000);
    
    while (1) {
        SEND_TX(GenImg, 12);
        
        for (int i = 0; i < 12; i++) {
            GIRP[i] = UART_READ();
        }
        
        if (GIRP[9] == 0x00) {
            break;
        } else {
            LCD_CMD(0x01);
            sendString("TRY AGAIN");
            __delay_ms(1000);
            
            retry++;
            if (retry >= 3) {
                LCD_CMD(0x01);
                sendString("TOO MANY TRIES");
                __delay_ms(2000);
                return 0;
            }
            
            LCD_CMD(0x01);
            sendString("PLACE FINGER");
            __delay_ms(500);
        }
    }
    
    SEND_TX(ImgToChar_1, 13);
    for (int i = 0; i < 12; i++) {
        ITCRP[i] = UART_READ();
    }
    
    if (ITCRP[9] != 0x00) {
        LCD_CMD(0x01);
        sendString("ERROR");
        __delay_ms(2000);
        return 0;
    }
    
    LCD_CMD(0x01);
    sendString("SEARCHING");
    
    SEND_TX(Search, 17);
    
    for (int i = 0; i < 16; i++) {
        SearchRP[i] = UART_READ();
    }
    
    LCD_CMD(0x01);
    
    if (SearchRP[9] == 0x00) {
        sendString("FOUND");
        __delay_ms(2000);
        return SearchRP[11];
    } else {
        sendString("NOT FOUND");
        __delay_ms(2000);
        return 0;
    }
}

#endif
