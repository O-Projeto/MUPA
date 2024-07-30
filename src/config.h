#pragma once
namespace botao{
    int button_1 = 33;
    int button_2 = 26;
    int button_3 = 25;
}

// Servo
#define LIM_SUP_DEDAO 180
#define LIM_INF_DEDAO 0
#define LIM_SUP_DEDOS 180
#define LIM_INF_DEDOS 0
#define TIME_SERVOS 1000

#define SERVO_TUMB_PIN 27
#define SERVO_FINGER_PIN 14 

#define T_ESPERA 1000

#define TFT_CYAN_DARK 0x001F
#define TFT_WINE 0xFA60


// Default color definitions
#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGREY   0xD69A      /* 211, 211, 211 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
#define TFT_PINK        0xFE19      /* 255, 192, 203 */ //Lighter pink, was 0xFC9F
#define TFT_BROWN       0x9A60      /* 150,  75,   0 */
#define TFT_GOLD        0xFEA0      /* 255, 215,   0 */
#define TFT_SILVER      0xC618      /* 192, 192, 192 */
#define TFT_SKYBLUE     0x867D      /* 135, 206, 235 */
#define TFT_VIOLET      0x915C      /* 180,  46, 226 */

const int backColors[] = {TFT_RED, TFT_GREEN, TFT_CYAN, TFT_YELLOW, TFT_WINE, TFT_CYAN_DARK, TFT_MAGENTA};

//Interface
#define MAX_STACK_SIZE 7
#define NUM_COLORS 9

// Codigo das cores são encontrados a partir da conversão do código hexadecimal para decimal
#define PIN_LED 13
#define NUM_COLORS 7
#define AMARELO 16768256 
#define VERMELHO 16515843
#define VERDE 63240
#define AZUL 49911
#define MAGENTA 16711935
#define CIANO 3407871
#define LARANJA 16093234
#define LIMAO 13434777
#define CIANO_ESCURO 31
#define VINHO 15759367

const int colors[] = {VERMELHO, VERDE, AZUL, AMARELO, VINHO, CIANO_ESCURO, MAGENTA, CIANO, LARANJA, LIMAO};
