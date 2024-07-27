#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "config.h"
#include "screenController.h"


TFT_eSPI tft = TFT_eSPI();
TFT_eSprite funcoes = TFT_eSprite(&tft);
TFT_eSprite stack = TFT_eSprite(&tft);

int index_ = 0;

Funcoes funcao(index_);


void setup() {
  Serial.begin(115200);
  tft.init(); // Inicializa o display
  tft.setRotation(3); // Define a rotação do display
  tft.fillScreen(TFT_WHITE); // Preenche a tela com a cor preta

  pinMode(botao::button_1, INPUT);
  pinMode(botao::button_2, INPUT);
  pinMode(botao::button_3, INPUT);

  funcoes.setColorDepth(8);
  funcoes.createSprite(180, 240);

  stack.setColorDepth(8);
  stack.createSprite(70, 240);
}

void loop() {
  funcao.draw_funcoes(funcoes, index_);
  funcao.select(index_, stack);
  funcao.track_position(index_);

}

