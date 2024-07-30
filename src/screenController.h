#pragma once

#include <config.h>
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <ServoController.hpp>
// #include <logo.h>


ServoController servo_tumb = ServoController(SERVO_TUMB_PIN);
ServoController servo_fingers = ServoController(SERVO_FINGER_PIN);

class Funcoes{
    private:
        int index = 0;
        int stack[MAX_STACK_SIZE]; 
        int stackSize = 0;
        //int lastbutton = 1;
        int button = 1;
        unsigned long lastDebounceTime = 0;
        unsigned long lastDebounceTimeBotoes = 0;
        const unsigned long debounceDelay = 50;
        int error = 0;
      
        
    public:
        Funcoes(int index):index(index){}

        void draw_funcoes(TFT_eSprite &funcoes, int &index, TFT_eSprite &stackSprite);
        void track_position(int &index, int &button);
        void select(int &index, int &button, TFT_eSPI &d);
        void init_screen(TFT_eSPI &d);
        void execStack( TFT_eSPI &d);
        //recieves message and color -> show message in screen with the leter in the color 
        void show_pop_up(TFT_eSPI &d,const char* message,int textColor);

};

void Funcoes::execStack( TFT_eSPI &d){
    int i;
    for(i = 0; i < stackSize; i++){
        switch (stack[i]){
            //Func abrir mao
            case 1:
                show_pop_up(d,"Abrir Mao",TFT_RED);
                servo_tumb.control.write(LIM_SUP_DEDAO);
                servo_fingers.control.write(LIM_SUP_DEDOS);
                delay(TIME_SERVOS);
                break;
            //Func fecha mao
            case 2:
                servo_tumb.control.write(LIM_INF_DEDAO);
                servo_fingers.control.write(LIM_INF_DEDOS);
                delay(TIME_SERVOS);
                break;
            //Func abrir dedao
            case 3:
                servo_fingers.control.write(LIM_SUP_DEDAO);
                delay(TIME_SERVOS);
                break;
            //Func fechar dedao
            case 4:
                servo_fingers.control.write(LIM_INF_DEDAO);
                delay(TIME_SERVOS);
                break;
            //Func abre dedos
            case 5:
                servo_tumb.control.write(LIM_SUP_DEDOS);
                delay(TIME_SERVOS);
                break;
            //Func fecha dedos
            case 6:
                servo_tumb.control.write(LIM_INF_DEDOS);
                delay(TIME_SERVOS);
                break;
            //Func espera
            case 7:
                delay(T_ESPERA);
                break;
        }
    }
}


void Funcoes::init_screen(TFT_eSPI &d){
    
    d.fillScreen(TFT_WHITE);
    d.setTextColor(TFT_BLUE);
    d.setTextSize(4);
    d.setSwapBytes(true);
    //d.pushImage(20,20,196,240,logo);
    delay(2000);
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

void Funcoes::show_pop_up(TFT_eSPI &d, const char* message, int textColor) {
    d.fillScreen(TFT_WHITE);
    d.setTextColor(textColor);
    d.setTextDatum(TC_DATUM);
    d.drawCentreString(message, 160, 80, 4);
    delay(2000);
    d.fillScreen(TFT_WHITE);
}
void Funcoes::track_position(int &index, int &button){
    switch(button){
        case 1:
                index++;
                if (index > NUM_COLORS) {
                    index = 0;
                }
                break;
        case 2:
                index--;
                if (index < 0) {
                    index = NUM_COLORS;
                }
                break;
    }
}

void Funcoes::select(int &index, int &button, TFT_eSPI &d) {
    int i, j;
    if(button == 3){
            if(index <= 6 && stackSize < MAX_STACK_SIZE){
                stack[stackSize++] = index+1;
            }
            else{
                switch (index){
                    case 7:
                        if (stackSize > 0) {
                            stackSize--;
                        }
                        break;
                    case 8:
                        stackSize = 0;
                        error = 0;
                        break;
                    case 9: 
                        //exec stack
                        execStack(d);
                        break;
                }
            }
    }

}


void Funcoes::draw_funcoes(TFT_eSprite &funcoes, int &index, TFT_eSprite &stackSprite){
    stackSprite.setTextFont(2);
    stackSprite.fillSprite(TFT_WHITE);
    stackSprite.setTextSize(2);

    stackSprite.setCursor(120, 10); 
    stackSprite.printf("Pilha");
    stackSprite.setTextColor(TFT_BLACK);

    funcoes.setTextFont(2);
    funcoes.fillSprite(TFT_WHITE);
    funcoes.setTextSize(2);

    funcoes.setCursor(0, 10); 
    funcoes.setTextColor(TFT_BLACK);
    if(index == 9){
        funcoes.println(" 4.Abrir dedao");
        funcoes.setCursor(0, 40);
        funcoes.println(" 5.Abrir dedos");
        funcoes.setCursor(0, 70);
        funcoes.println(" 6.Fechar dedos");
        funcoes.setCursor(0, 100);
        funcoes.println(" 7.Esperar");
        funcoes.setCursor(0, 130);
        funcoes.println(" Remover");
        funcoes.setCursor(0, 160);
        funcoes.println(" Remover tudo");
        funcoes.setCursor(0, 190);
        funcoes.setTextColor(TFT_GREEN, TFT_BLACK);
        funcoes.println(" Executar");
        funcoes.setTextColor(TFT_BLACK);

    }
    else if(index == 8){
        funcoes.println(" 3.Abrir dedao");
        funcoes.setCursor(0, 40);
        funcoes.println(" 4.Fechar dedao");
        funcoes.setCursor(0, 70);
        funcoes.println(" 5.Abrir dedos");
        funcoes.setCursor(0, 100);
        funcoes.println(" 6.Fechar dedos");
        funcoes.setCursor(0, 130);
        funcoes.println(" 7.Esperar");
        funcoes.setCursor(0, 160);
        funcoes.println(" Remover");
        funcoes.setCursor(0, 190);
        funcoes.setTextColor(TFT_RED, TFT_BLACK);
        funcoes.println(" Remover tudo");
        funcoes.setTextColor(TFT_BLACK);

    }
    else if(index == 7){
        funcoes.println(" 2.Fechar mao");
        funcoes.setCursor(0, 40);
        funcoes.println(" 3.Abrir dedao");
        funcoes.setCursor(0, 70);
        funcoes.println(" 4.Fechar dedao");
        funcoes.setCursor(0, 100);
        funcoes.println(" 5.Abrir dedos");
        funcoes.setCursor(0, 130);
        funcoes.println(" 6.Fechar dedos");
        funcoes.setCursor(0, 160);
        funcoes.println(" 7.Esperar");
        funcoes.setCursor(0, 190);
        funcoes.setTextColor(TFT_RED, TFT_BLACK);
        funcoes.println(" Remover");
        funcoes.setTextColor(TFT_BLACK);

    }
    else{
        if (index == 0) {
            funcoes.setTextColor(TFT_WHITE, TFT_RED);
            funcoes.println(" 1.Abrir mao");
            funcoes.setTextColor(TFT_BLACK);
            // Serial.println("pong");
        } else {
            funcoes.println(" 1.Abrir mao");
        }

        funcoes.setCursor(0, 40); 
        if (index == 1) {
            funcoes.setTextColor(TFT_BLACK, TFT_GREEN);
            funcoes.println(" 2.Fechar mao");
            funcoes.setTextColor(TFT_BLACK);
            // Serial.println("dino");
        } else {
            funcoes.println(" 2.Fechar mao");
        }

        funcoes.setCursor(0, 70); 
        if (index == 2) {
            funcoes.setTextColor(TFT_BLACK, TFT_CYAN);
            funcoes.println(" 3.Abrir dedao");
            funcoes.setTextColor(TFT_BLACK);
            // Serial.println("dino");
        } else {
            funcoes.println(" 3.Abrir dedao");
        }

        funcoes.setCursor(0, 100); 
        if (index == 3) {
            funcoes.setTextColor(TFT_BLACK, TFT_YELLOW);
            funcoes.println(" 4.Fechar dedao");
            funcoes.setTextColor(TFT_BLACK);
            // Serial.println("dino");
        } else {
            funcoes.println(" 4.Fechar dedao");
        }

        funcoes.setCursor(0, 130); 
        if (index == 4) {
            funcoes.setTextColor(TFT_WHITE, TFT_WINE);
            funcoes.println(" 5.Abrir dedos");
            funcoes.setTextColor(TFT_BLACK);
            // Serial.println("dino");
        } else {
            funcoes.println(" 5.Abrir dedos");
        }

        
        funcoes.setCursor(0, 160); 
        if (index == 5) {
            funcoes.setTextColor(TFT_WHITE, TFT_CYAN_DARK);
            funcoes.println(" 6.Fechar dedos");
            funcoes.setTextColor(TFT_BLACK);
            // Serial.println("dino");
        } else {
            funcoes.println(" 6.Fechar dedos");
        }

        funcoes.setCursor(0, 190); 
        if (index == 6) {
            funcoes.setTextColor(TFT_WHITE, TFT_MAGENTA);
            funcoes.println(" 7.Esperar");
            funcoes.setTextColor(TFT_BLACK);
            // Serial.println("dino");
        } else {
            funcoes.println(" 7.Esperar");
        }
    }
    funcoes.pushSprite(0, 0);

        // Redesenha a pilha na tela
    stackSprite.fillSprite(TFT_WHITE); 
    stackSprite.setCursor(0, 2); // Ajuste o posicionamento conforme necessário
    stackSprite.setTextColor(TFT_BLACK);
    stackSprite.printf("Pilha");
    for (int i = 0; i < stackSize; i++) {
        stackSprite.setCursor(220, 20 + i * 25); // Ajuste o espaçamento conforme necessário
        stackSprite.setTextColor(TFT_BLACK, backColors[stack[i]-1]);
        stackSprite.printf(" %d \n", stack[i]);
    }
    stackSprite.setCursor(0, 210);
    stackSprite.setTextColor(TFT_BLACK);
    stackSprite.printf("%d Erros", error);
    stackSprite.pushSprite(220, 0);

}