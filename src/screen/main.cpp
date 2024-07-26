#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI display = TFT_eSPI();
TFT_eSprite side_fuction = TFT_eSprite(&display); //parte que ficará a tela da esquerda com as funções

void setup(){
    Serial.begin(115200);
    display.init();
    display.fillScreen(TFT_WHITE);
    display.setRotation(1);

    side_fuction.setColorDepth(8);
}