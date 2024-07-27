#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "config.h"


TFT_eSPI tft = TFT_eSPI(); // Inicializa o objeto TFT
int currentSelection = 1; // Variável para rastrear a seleção atual do menu


void drawMenu() {
  tft.fillScreen(TFT_BLACK); // Limpa a tela
  for (int i = 1; i <= 7; i++) {
    if (i == currentSelection) {
      tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Cor de texto amarela para a seleção atual
    } else {
      tft.setTextColor(TFT_WHITE, TFT_BLACK); // Cor de texto branca para os outros itens
    }
    tft.drawString(String(i), 10, 20 * i, 4); // Desenha o número na posição correta
  }
} 

void setup() {
  tft.init(); // Inicializa o display
  tft.setRotation(1); // Define a rotação do display
  tft.fillScreen(TFT_BLACK); // Preenche a tela com a cor preta
  drawMenu(); // Desenha o menu
}

void loop() {
//   if (digitalRead(botao::button_1) == LOW) { // Verifica se o botão UP foi pressionado
//     currentSelection--;
//     if (currentSelection < 1) currentSelection = 7;
//     drawMenu();
//     delay(200); // Debounce
//   }
//   if (digitalRead(botao::button_2) == LOW) { // Verifica se o botão DOWN foi pressionado
//     currentSelection++;
//     if (currentSelection > 7) currentSelection = 1;
//     drawMenu();
//     delay(200); // Debounce
//   }

}

