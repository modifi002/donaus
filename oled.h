#include <Arduino.h>
#include <U8x8lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

U8X8_SSD1327_EA_W128128_HW_I2C u8x8( /* reset=*/ U8X8_PIN_NONE);
int levelfan = 3;


void setoled(void) {
  //Serial.setDebugOutput(true);
  u8x8.begin();
}
void readoled(void) {
  //  u8x8.clear();
  if (wificonnect == true) {
    u8x8.setFont(u8x8_font_pxplusibmcgathin_r);
    u8x8.setCursor(5, 0);
    u8x8.print("conect");
  }
  ////////////////////////////////////////////////////////////////////////////////////////
  u8x8.setFont(u8x8_font_artosserif8_r   );
  u8x8.setCursor(3, 2);
  u8x8.print("Volts");
  u8x8.setCursor(11, 2);
  u8x8.print("Amp");
  ///////////////////////////////////////////////A/////////////////////////////////////////
  u8x8.setFont(u8x8_font_8x13B_1x2_r);
  u8x8.setCursor(0, 4);
  u8x8.print("A");
  u8x8.setCursor(3, 4);
  u8x8.print("230");
  u8x8.setCursor(11, 4);
  u8x8.print("3.5");
  ////////////////////////////////////////////////B//////////////////////////////////////////
  u8x8.setCursor(0, 6);
  u8x8.print("B");
  u8x8.setCursor(3, 6);
  u8x8.print("225");
  u8x8.setCursor(11, 6);
  u8x8.print("3.0");
  ////////////////////////////////////////////////C/////////////////////////////////////////////
  u8x8.setCursor(0, 8);
  u8x8.print("C");
  u8x8.setCursor(3, 8);
  u8x8.print("231");
  u8x8.setCursor(11, 8);
  u8x8.print("3.5");
  //////////////////////////////////////////////////////////////////////////////////////////////////
  u8x8.setFont(u8x8_font_artosserif8_r  );
  u8x8.setCursor(2, 11);
  u8x8.print("Unit");
  u8x8.setCursor(7, 11);
  u8x8.print("123456");
  delay(1000);
}

void display_data(int id, float volt, float amp, float power, float unit){
  
}
