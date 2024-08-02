#pragma once

#include <config.h>
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <ServoController.hpp>
#include "logo_projeto_bit_map.c"



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
        int error = 0, tempo = millis();
        
        // state machine
        int state = START;
        int last_state = START;
        int state_before_delay = START;
        bool trigger_delay = false;
        
        bool cmd_open_fingers = false; 
        bool cmd_close_fingers = false; 
        bool cmd_open_thumb = false; 
        bool cmd_close_thumb = false; 
        bool cmd_delay = false; 
        bool cmd_reset = false; 
      
        
    public:
        Funcoes(int index):index(index){}

        void draw_funcoes(TFT_eSprite &funcoes, int &index, TFT_eSprite &stackSprite);
        void track_position(int &index, int &button);
        void select(int &index, int &button, TFT_eSPI &d);
        void init_screen(TFT_eSPI &d);
        void execStack( TFT_eSPI &d);
        void check_error();
        //recieves message and color -> show message in screen with the leter in the color 
        void show_pop_up(TFT_eSPI &d,const char* message,int textColor,int text_size);
        void StateMachine();

};

void Funcoes::StateMachine(){

    if(state != DELAY) last_state = state;


    // ------------------------------------------
    // Filter
    // ------------------------------------------

    // any state -> delay
    if(cmd_delay){
        if(state != DELAY) state_before_delay = state;
        trigger_delay = true;
        state = DELAY;
    }

    // any state -> reset
    if(cmd_reset){
        state = START;
    }

    // return from delay
    if(state == DELAY && trigger_delay == false)
        state = state_before_delay;

    // ------------------------------------------
    // Transition
    // ------------------------------------------
    
    switch (state)
    {
    case START:
        
        state = FINGERS_OPEN_THUMB_OPEN;

        cmd_close_fingers = false;
        cmd_open_fingers = false;
        cmd_close_thumb = false;
        cmd_open_thumb = false;
        cmd_delay = false;
        state_before_delay = START;
        
        break;

    case FINGERS_OPEN_THUMB_OPEN:

        if(cmd_close_fingers && cmd_close_thumb){
            state = FINGERS_CLOSE_THUMB_CLOSE;
        }
        else if(cmd_close_fingers){
            state = FINGERS_CLOSE_THUMB_OPEN;
        }
        else if(cmd_close_thumb){
            state = FINGERS_OPEN_THUMB_CLOSE;
        }

        break;

    case FINGERS_CLOSE_THUMB_OPEN:
        
        if(cmd_open_fingers){
            state = FINGERS_OPEN_THUMB_OPEN;
        }
        else if(cmd_close_thumb){
            state = FINGERS_CLOSE_THUMB_CLOSE;
        }
        break;

    case FINGERS_OPEN_THUMB_CLOSE:
        
        if(cmd_open_thumb){
            state = FINGERS_OPEN_THUMB_OPEN;
        }
        break;

    case FINGERS_CLOSE_THUMB_CLOSE:
        
        if(cmd_open_thumb && cmd_open_fingers){
            state = FINGERS_OPEN_THUMB_OPEN;
        }
        else if(cmd_open_thumb){
            state = FINGERS_CLOSE_THUMB_OPEN;
        }
        break;

    case DELAY:
        
        if(trigger_delay) trigger_delay = false;
        break;
    
    default:
        break;
    }

    // ------------------------------------------
    // Output
    // ------------------------------------------

}

void Funcoes::execStack( TFT_eSPI &d){
    int i;
    for(i = 0; i < stackSize; i++){
        switch (stack[i]){
            //Func abrir mao
            case 1:
                show_pop_up(d,"Abrir Mao",TFT_RED,2);
                servo_tumb.control.write(LIM_SUP_DEDAO);
                delay(1000);
                servo_fingers.control.write(LIM_SUP_DEDOS);
                delay(TIME_SERVOS);
                break;
            //Func fecha mao
            case 2:
                show_pop_up(d,"Fecha Mao",TFT_GREEN,2);
                servo_tumb.control.write(LIM_INF_DEDAO);
                delay(1000);
                servo_fingers.control.write(LIM_INF_DEDOS);
                delay(TIME_SERVOS);
                break;
            //Func abrir dedao
            case 3:
                show_pop_up(d,"Abrir Dedao",TFT_SKYBLUE,2);
                servo_tumb.control.write(LIM_SUP_DEDAO);
                delay(TIME_SERVOS);
                break;
            //Func fechar dedao
            case 4:
                show_pop_up(d,"Fecha Dedao",TFT_YELLOW,2);
                servo_tumb.control.write(LIM_INF_DEDAO);
                delay(TIME_SERVOS);
                break;
            //Func abre dedos
            case 5:
                show_pop_up(d,"Abre Dedos",TFT_YELLOW,2);  
                servo_fingers.control.write(LIM_SUP_DEDOS);
                delay(TIME_SERVOS);
                break;
            //Func fecha dedos
            case 6:
                show_pop_up(d,"Fecha Dedos",TFT_BLUE,2);  
                servo_fingers.control.write(LIM_INF_DEDOS);
                delay(TIME_SERVOS);
                break;
            //Func espera
            case 7:
                 show_pop_up(d,"Espera",TFT_VIOLET,2);  
                delay(T_ESPERA);
                break;
        }
    }
    delay(1000);
    servo_tumb.control.write(LIM_SUP_DEDAO);
    delay(500);
    servo_fingers.control.write(LIM_SUP_DEDOS);
    delay(TIME_SERVOS);
}


void Funcoes::init_screen(TFT_eSPI &d){
    
    d.fillScreen(TFT_WHITE);
    d.setTextColor(TFT_BLUE);
    d.setTextSize(4);
    d.setSwapBytes(true);
    d.pushImage(0,5,320,240,logo_projeto_bit_map);
    delay(1500);
    d.fillScreen(TFT_WHITE);
     
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

void Funcoes::show_pop_up(TFT_eSPI &d, const char* message, int textColor,int text_size) {
    d.fillScreen(TFT_WHITE);
    d.setTextSize(text_size);
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
    //? pq igual a 3 
    if(button == 3){
            if(index <= 6 && stackSize < MAX_STACK_SIZE){
                stack[stackSize++] = index+1;
                check_error();
            }
            else{
                switch (index){
                    case 7:
                        if (stackSize > 0) {
                            stackSize--;

                            check_error();
                        }
                        break;
                    case 8:
                        stackSize = 0;
                        error = 0;
                        break;
                    case 9: 
                        //exec stack
                        
                        // ALE MUDOU AQUI !!!
                        check_error();
                        if(error == 0 && stackSize > 0) execStack(d);
                        else if(error > 0) show_pop_up(d,"ERRO",TFT_RED,4);  
                        
                        // error = 0;
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
        funcoes.setTextColor(TFT_WHITE, TFT_BLACK);
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
        funcoes.setTextColor(TFT_WHITE, TFT_BLACK);
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
        stackSprite.setCursor(200, 20 + i * 25); // Ajuste o espaçamento conforme necessário
        stackSprite.setTextColor(TFT_BLACK, backColors[stack[i]-1]);
        stackSprite.printf(" %d \n", stack[i]);
    }
    // stackSprite.setCursor(50, 210);
    // stackSprite.setTextColor(TFT_BLACK);
    // stackSprite.printf("%d Erros", error);
    if(millis() - tempo > 3000){
        tempo = millis();
        // Serial.println(error);
    }
    if(error!=0){

        for(int j=0; j<error;j++){

        stackSprite.fillRoundRect(75,210-(35*j),20,30,1,TFT_RED);
        }
    }
        stackSprite.pushSprite(220, 0);

}

// check for conditions that cause prog errors 
void Funcoes::check_error(){
    error = 0;
    int current_state = 0,last_state = 1;
    bool finger_closed = false,tumb_closed = false;  
    int state_x;


    Serial.println("------------------------------------");
    Serial.println("NEW CYCLE: ");
    Serial.println("------------------------------------");

    // clean state buffer
    cmd_reset = true;
    StateMachine();
    state_x = state;
    Serial.print("update start: ");
    Serial.println(state_x);
    cmd_reset = false;

    for(int i = 0; i<stackSize; i++){

        int cmd = stack[i];
        // 1 -> abre mao
        // 2 -> fecha mao
        // 3 -> abre dedao
        // 4 -> fecha dedao
        // 5 -> abre dedos
        // 6 -> fecha dedos
        // 7 -> espera

        cmd_open_fingers = (cmd == 1 || cmd == 5);
        cmd_close_fingers = (cmd == 2 || cmd == 6);
        
        cmd_open_thumb = (cmd == 1 || cmd == 3);
        cmd_close_thumb = (cmd == 2 || cmd == 4);

        cmd_delay = (cmd == 7);
        if(state != DELAY) state_x = state;
        StateMachine();
        if(state_x == state && state != DELAY){
            error++;

        }
            Serial.println("---------------------");
            Serial.print("last state: ");
            Serial.println(state_x);
            Serial.print("state: ");
            Serial.println(state);
            Serial.print("state before delay: ");
            Serial.println(state_before_delay);
            Serial.print("error: ");
            Serial.println(error);
        // if(last_state == state && state != DELAY) error++;
        // else if(state_x == DELAY){
        //     if(state == state_before_delay) error++;
        // }

        // current_state = stack[i];

        // if(i > 0){
        //     last_state = stack[i-1];
        // }
        // // check for last relevant state (ignore delay)
        // if(current_state!=7){
        //     if(current_state==1){
        //         finger_closed = false;
        //         tumb_closed = false; 
        //     }
        //      if(current_state==2){
        //         finger_closed = true;
        //         tumb_closed = true; 
        //     }

        //     if(current_state==3){
        //         tumb_closed = false; 
        //     }

            
        //     if(current_state==4){
        
        //         tumb_closed = true; 
        //     }

        //     if(current_state==5){
            

        //         finger_closed = false; 
        //     }
            
        //     if(current_state==6){
        
        //         finger_closed = true; 
        //     }

            
        //     //state 1 dont have any error possible 

        //     //state 2 dont have any error possible 

        //     //state 3 dont have any error possible 

        //     //state 4 dont have any error possible 

        //     //cant open finger if tumb is closed -> mecanical error
        //     if(current_state==5 && tumb_closed){
        //         error = error + 1 ;
        //     }

        //     //cant close finger if tumb is closed -> mecanical error 
        //     if(current_state==6 && tumb_closed){
        //         error = error + 1 ;
        //     }

        //     //cant repeat states 
        //     if(last_state==current_state){
        //          error = error + 1 ;
        //     }

        //     if(current_state == 5 && finger_closed){
        //         error += 1;
        //     }
             
        // }



    }

    //machine_state to check for errors 

}