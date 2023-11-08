# LGT8F328P-Displays

## under construction !

LGT8F328P Package for Arduino IDE :
https://github.com/dbuezas/lgt8fx


- Display gmg12864
- Library u8g2
- tested with :
  - LGT8F328P LQFP48 "Nano Style", purple
  - LGT8F328P LQFP32 "Pro Mini Style" 3.3V, green and USB TTL adapter

works fine.

Connections for LCD Display GMG12864-06D Ver:2.2 :
| TFT  | GPIO | Description     |
| :--- | ---: | :-------------- |
| SI   |   11 | SPI MOSI        |
| SCL  |   13 | SPI SCLK        |
| CS   |    7 | CS              |
| RS   |    8 | DC              |
| RSE  |  RST | RESET Pin       |
| VDD  |      | 3.3V            |
| VSS  |      | GND             |
| A    |      | LED A -> 47Ω -> 3.3V |
| K    |      | LED K -> GND         |

Configuration u8g2 library :
````java
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_ST7565_ERC12864_ALT_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 7, /* dc=*/ 8, /* reset=*/ U8X8_PIN_NONE);

#define CONTRAST 75  // !!!

void setup(void) {
  u8g2.begin();
  u8g2.setContrast(CONTRAST); 
}

void loop(void) {
  //...
}  
````


- Touch-Display ST7735
- Library TFT_eSPI
- tested with : LGT8F328P LQFP32 "Pro Mini Style" 3.3V, green and USB TTL adapter

Display and touch work fine with modification of TFT_eSPI\Processors\TFT_eSPI_Generic.h :

Replace :

````java
...   
    #ifdef __AVR__ // AVR processors do not have 16 bit transfer
      #define tft_Write_8(C)   {SPDR=(C); while (!(SPSR&_BV(SPIF)));}
`````
with :
````java
...
    #ifdef __AVR__ // AVR processors do not have 16 bit transfer
      #ifdef __LGT8FX8P__
        #define tft_Write_8(C)   {SPDR=(C); asm volatile("nop"); while((SPFR & _BV(RDEMPT))); SPFR=_BV(RDEMPT)|_BV(WREMPT); }
      #else
        #define tft_Write_8(C)   {SPDR=(C); while (!(SPSR&_BV(SPIF)));}
      #endif  
...
````

Solution found here :
- https://github.com/dbuezas/lgt8fx/blob/master/lgt8f/libraries/SPI/src/SPI.h
- https://github.com/adafruit/Adafruit-GFX-Library/pull/313/commits/14aa943754aac8825e75f9ff75c308bf7ada7ab4

SPI-Transfer for AVR and LGT8FX8P in SPI.h :
````java
inline static uint8_t transfer(uint8_t data) {
  uint8_t rcvd;
	  
  SPDR = data;
  asm volatile("nop");
  #if defined(__LGT8FX8P__)
    while((SPFR & _BV(RDEMPT)));
    rcvd = SPDR;
    SPFR = _BV(RDEMPT) | _BV(WREMPT);
  #else
    while(!(SPSR & _BV(SPIF)));
    rcvd = SPDR;
  #endif

  return rcvd;
}
````


Connections for ST7735 :
| GPIO | TFT   | Touch | SD-Card | Description      |
| ---: | :---- | :---- | :------ | :--------------- |
| 11   | SDA   | T_DIN | SD_MOSI | MOSI             |
| 12   |       | T_DO  | SD_MISO | MISO             |
| 13   | SCK   | T-CLK | SD_SCK  | CLK              |
|  8   | A0    |       |         | DC               |
| RST  | RESET |       |         | RST              |
|  7   | CS    |       |         | CS-TFT           |
|  4   |       | T_CS  |         | CS-Touch         |
|      |       |       | SD_CS   | CS-SD (not used) |
|      |       | T_IRQ |         | IRQ (not used)   |
|      | GND   |       |         | GND              |
|      | VCC   |       |         | 3.3V             |
|      | LED   |       |         | 3.3V             |


Links :

https://wolles-elektronikkiste.de/minievb-boards-ein-ueberblick

https://wolles-elektronikkiste.de/lgt8f328p-lqfp32-boards