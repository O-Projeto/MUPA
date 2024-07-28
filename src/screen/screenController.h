#pragma once

#include "config.h"
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>

#define MAX_STACK_SIZE 10

class Funcoes{
    private:
        int index;
        int stack[MAX_STACK_SIZE]; 
        int stackSize = 0;
        unsigned long lastDebounceTime = 0;
        unsigned long lastDebounceTimeBotoes = 0;
        const unsigned long debounceDelay = 150;
        
    public:
        Funcoes(int index):index(index){}

        void draw_funcoes(TFT_eSprite &funcoes, int &index);
        void track_position(int &index);
        void select(int &index, TFT_eSprite &stack);
        void init_screen(TFT_eSPI &d);

};

void Funcoes::init_screen(TFT_eSPI &d){
    d.fillScreen(TFT_WHITE);
    d.setTextColor(TFT_BLUE);
    d.setTextSize(4);
    // Define a cor e o tamanho do texto
    d.setTextDatum(TC_DATUM);
    d.setCursor(50, 90);
    String name = "O PROJETO\n     HAND ";
    for (int i = 0; i < name.length(); i++) {
        d.print(name[i]);
        delay(100);
    }
    delay(1500);
    d.fillScreen(TFT_WHITE);
    
}

void Funcoes::track_position(int &index){
    unsigned long currentTime = millis();
    if (currentTime - lastDebounceTime > debounceDelay) {
        lastDebounceTime = currentTime;
        if (digitalRead(botao::button_1) == LOW) {
            index++;
            if (index > 6) {
                index = 0;
            }
        }

        if (digitalRead(botao::button_2) == LOW) {
            index--;
            if (index < 0) {
                index = 6;
            }
        }
        

    }
}

void Funcoes::select(int &index, TFT_eSprite &stackSprite) {
    stackSprite.setTextFont(2);
    stackSprite.fillSprite(TFT_WHITE);
    stackSprite.setTextSize(2);

    stackSprite.setCursor(120, 10); 
    stackSprite.println(" Pilha ");
    stackSprite.setTextColor(TFT_BLACK);
    unsigned long timeBotoes = millis();
    if (timeBotoes - lastDebounceTimeBotoes > debounceDelay) {
        lastDebounceTimeBotoes = timeBotoes;
        if (digitalRead(botao::button_3) == LOW) {
            switch (index) {
                case 0:
                    if (stackSize < MAX_STACK_SIZE) {
                        stack[stackSize++] = 1;
                    }
                    break;
                case 1:
                    if (stackSize < MAX_STACK_SIZE) {
                        stack[stackSize++] = 2;
                    }
                    Serial.println("Fecha mão");
                    break;
                case 2:
                    Serial.println("Abrir dedão");
                    if (stackSize < MAX_STACK_SIZE) {
                        stack[stackSize++] = 3;
                    }
                    break;
                case 3:
                    Serial.println("Fecha dedão");
                    if (stackSize < MAX_STACK_SIZE) {
                        stack[stackSize++] = 4;
                    }
                    break;
                case 4:
                    Serial.println("Delay");
                    if (stackSize < MAX_STACK_SIZE) {
                        stack[stackSize++] = 5;
                    }
                    break;
                case 5:
                    if (stackSize > 0) {
                        stackSize--;
                    }
                    Serial.println("Remover");
                    break;
                case 6:
                    stackSize = 0;
                    Serial.println("Executar");
                    break;
            }
        }
    }
    
    // Redesenha a pilha na tela
    stackSprite.fillSprite(TFT_WHITE); 
    stackSprite.setCursor(0, 2); // Ajuste o posicionamento conforme necessário
    stackSprite.setTextColor(TFT_BLACK);
    stackSprite.println("Pilha");
    for (int i = 0; i < stackSize; i++) {
        stackSprite.setCursor(220, 20 + i * 25); // Ajuste o espaçamento conforme necessário
        stackSprite.setTextColor(TFT_BLACK);
        stackSprite.println(stack[i]);
    }
    stackSprite.pushSprite(230, 0);
}


void Funcoes::draw_funcoes(TFT_eSprite &funcoes, int &index){
    funcoes.setTextFont(2);
    funcoes.fillSprite(TFT_WHITE);
    funcoes.setTextSize(2);

    funcoes.setCursor(0, 10); 
    funcoes.setTextColor(TFT_BLACK);
    if (index == 0) {
        funcoes.setTextColor(TFT_WHITE, TFT_BLACK);
        funcoes.println("1.Abrir mao");
        funcoes.setTextColor(TFT_BLACK);
        // Serial.println("pong");
    } else {
        funcoes.println("1.Abrir mao");
    }

    funcoes.setCursor(0, 40); 
    if (index == 1) {
        funcoes.setTextColor(TFT_WHITE, TFT_BLACK);
        funcoes.println("2.Fechar mao");
        funcoes.setTextColor(TFT_BLACK);
        // Serial.println("dino");
    } else {
        funcoes.println("2.Fechar mao");
    }

    funcoes.setCursor(0, 70); 
    if (index == 2) {
        funcoes.setTextColor(TFT_WHITE, TFT_BLACK);
        funcoes.println("3.Abrir dedao");
        funcoes.setTextColor(TFT_BLACK);
        // Serial.println("dino");
    } else {
        funcoes.println("3.Abrir dedao");
    }

    funcoes.setCursor(0, 100); 
     if (index == 3) {
        funcoes.setTextColor(TFT_WHITE, TFT_BLACK);
        funcoes.println("4.Fechar dedao");
        funcoes.setTextColor(TFT_BLACK);
        // Serial.println("dino");
    } else {
        funcoes.println("4.Fechar dedao");
    }

    funcoes.setCursor(0, 130); 
     if (index == 4) {
        funcoes.setTextColor(TFT_PINK, TFT_BLACK);
        funcoes.println("5.Esperar");
        funcoes.setTextColor(TFT_BLACK);
        // Serial.println("dino");
    } else {
        funcoes.println("5.Esperar");
    }

    
    funcoes.setCursor(0, 160); 
     if (index == 5) {
        funcoes.setTextColor(TFT_RED, TFT_BLACK);
        funcoes.println("Remover");
        funcoes.setTextColor(TFT_BLACK);
        // Serial.println("dino");
    } else {
        funcoes.println("Remover");
    }

     funcoes.setCursor(0, 190); 
     if (index == 6) {
        funcoes.setTextColor(TFT_GREEN, TFT_BLACK);
        funcoes.println("Executar");
        funcoes.setTextColor(TFT_BLACK);
        // Serial.println("dino");
    } else {
        funcoes.println("Executar");
    }
    funcoes.pushSprite(0, 0);

}