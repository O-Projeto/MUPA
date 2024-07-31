#include <Arduino.h>
#include "ServoController.hpp"
#include "led_rgb.h"
#include "screenController.h"

#include <TFT_eSPI.h>
#include <SPI.h>



led_rgb LED;


TFT_eSPI tft = TFT_eSPI();
TFT_eSprite funcoes = TFT_eSprite(&tft);
TFT_eSprite stack = TFT_eSprite(&tft);

int index_ = 0;

Funcoes funcao(index_);

// Button
int button_press = 0, last_button = 1, option = 0;
int lastOption = 1;



TaskHandle_t Task1;



void TaskLed(void *PvParameters);

int readButton(void);


void setup() {
    Serial.begin(115200);

    servo_tumb.SetupServo();
    servo_fingers.SetupServo();

    LED.init();

    tft.init();
    tft.setRotation(3);
    tft.fillScreen(TFT_WHITE);

    funcoes.setColorDepth(8);
    funcoes.createSprite(220, 240);

    stack.setColorDepth(8);
    stack.createSprite(100, 240);  
    funcao.init_screen(tft);

    pinMode(botao::button_1, INPUT_PULLUP);
    pinMode(botao::button_2, INPUT_PULLUP);
    pinMode(botao::button_3, INPUT_PULLUP);


    xTaskCreatePinnedToCore(
                    TaskLed,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */
    
}

void loop() {
    //? update image ? 
    funcao.draw_funcoes(funcoes, index_, stack);
    if((button_press = readButton()) && (button_press != last_button)){
        // funcao.check_error();
        funcao.track_position(index_, button_press);
        funcao.select(index_, button_press, tft);

    }
    last_button = button_press;
    
}

void TaskLed(void *PvParameters){

  //? pq precisa de uma task pra isso ? 
  while (1)
  {
   
    LED.print_color(colors[index_]);
  
  }
}


// Lê se um botão for pressionado
//? retorna oq ? 

int readButton(void){
    int button_high = 0;
    int tempo = 0;
    if(!digitalRead(botao::button_1) && !button_high){
        tempo = millis();
        button_high = 1;
    }
    if(!digitalRead(botao::button_2) && !button_high){
        tempo = millis();
        button_high = 2;
    }
    if(!digitalRead(botao::button_3) && !button_high){
        tempo = millis();
        button_high = 3;
    }
    while(button_high && (millis() - tempo) < 50){
        if(digitalRead(botao::button_1) && digitalRead(botao::button_2) && digitalRead(botao::button_3)){
            return 0;
        }
    }
    return button_high;
}