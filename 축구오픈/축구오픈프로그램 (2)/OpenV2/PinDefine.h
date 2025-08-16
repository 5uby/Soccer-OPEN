/*
 * PinDefine.h
 * 
 * 프로그램에서 쓰이는 신호들의 물리적인 하드웨어 핀을 정의
 * 
 * History
 *
 * 2024-04-30
 * 
 */

//----------------------------------------------------------------------
// 슈터 핀 정의 
//----------------------------------------------------------------------
#define ShooterON  (PORTD &= 0x7F)  //PD7 LOW 출력
#define ShooterOFF (PORTD |= 0x80)  //PD7 HIGH 출력
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// 초음파 센서 관련 핀 정의 
//----------------------------------------------------------------------
#define TRIGER1ON (PORTC |= 0x01)  //PC0 HIGH 출력 //1번 초음파(전방) 센서의 트리거 핀 ON
#define TRIGER2ON (PORTC |= 0x02)  //PC1 HIGH 출력 //2번 초음파(전방) 센서의 트리거 핀 ON
#define TRIGER3ON (PORTC |= 0x04)  //PC2 HIGH 출력 //3번 초음파(전방) 센서의 트리거 핀 ON
#define TRIGER4ON (PORTC |= 0x08)  //PC3 HIGH 출력 //4번 초음파(전방) 센서의 트리거 핀 ON

#define TRIGER1OFF (PORTC &= 0xFE)  //PC0 LOW 출력 //1번 초음파(전방) 센서의 트리거 핀 OFF
#define TRIGER2OFF (PORTC &= 0xFD)  //PC1 LOW 출력 //2번 초음파(전방) 센서의 트리거 핀 OFF
#define TRIGER3OFF (PORTC &= 0xFB)  //PC2 LOW 출력 //3번 초음파(전방) 센서의 트리거 핀 OFF
#define TRIGER4OFF (PORTC &= 0xF7)  //PC3 LOW 출력 //4번 초음파(전방) 센서의 트리거 핀 OFF

#define ECHO1 (PINC & 0x10)      //PC0 입력 //1번 초음파(전방) 센서의 에코 핀
#define ECHO2 (PINC & 0x20)      //PC1 입력 //2번 초음파(우측) 센서의 에코 핀
#define ECHO3 (PINC & 0x40)      //PC2 입력 //3번 초음파(후방) 센서의 에코 핀
#define ECHO4 (PINC & 0x80)      //PC3 입력 //4번 초음파(좌측) 센서의 에코 핀
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// 버튼 키 정의
//----------------------------------------------------------------------
#define KEY_LEFT (~PINL & 0x20)  //PL5  //왼쪽 조작 버튼(PREV)
#define KEY_RIGHT (~PINL & 0x40) //PL6 //오른쪽 조작 버튼(NEXT)
#define KEY_ENTER (~PINL & 0x80) //PL7 //가운데 조작 버튼(ENTER)
#define KEY_F1 (~PINB & 0x20)    //PB5 상판 F1키
#define KEY_F2 (~PINB & 0x40)    //PB6 상판 F2키
#define SUPERTEAM (~PINB & 0x80) //PB7 슈퍼팀경기에서 동작 허용 여부 입력
//----------------------------------------------------------------------
#define KEY_IN (KEY_LEFT | KEY_RIGHT | KEY_F1 | KEY_F2) //Key Pressed without ENTER
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// 시프트 레지스터 핀 정의
//----------------------------------------------------------------------
#define IR_SERIN 22 //PA0
#define IR_SRCK  23 //PA1
#define IR_RCK   24 //PA2
#define IR_ENABLE 25 //PA3
#define IR_SEROUT 39 //PG2
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// For 2.2" TFT LCD With ILI9341.
//----------------------------------------------------------------------
#define LCD_DC  48  //PL1
#define LCD_CS  49  //PL0
#define LCD_RST 47  //PL2
//----------------------------------------------------------------------


/* RGB 16-bit color table definition (RG565) */
#define COLOR_BLACK          0x0000      /*   0,   0,   0 */
#define COLOR_WHITE          0xFFFF      /* 255, 255, 255 */
#define COLOR_BLUE           0x001F      /*   0,   0, 255 */
#define COLOR_GREEN          0x07E0      /*   0, 255,   0 */
#define COLOR_RED            0xF800      /* 255,   0,   0 */
#define COLOR_NAVY           0x000F      /*   0,   0, 128 */
#define COLOR_DARKBLUE       0x0011      /*   0,   0, 139 */
#define COLOR_DARKGREEN      0x03E0      /*   0, 128,   0 */
#define COLOR_DARKCYAN       0x03EF      /*   0, 128, 128 */
#define COLOR_CYAN           0x07FF      /*   0, 255, 255 */
#define COLOR_TURQUOISE      0x471A      /*  64, 224, 208 */
#define COLOR_INDIGO         0x4810      /*  75,   0, 130 */
#define COLOR_DARKRED        0x8000      /* 128,   0,   0 */
#define COLOR_OLIVE          0x7BE0      /* 128, 128,   0 */
#define COLOR_GRAY           0x8410      /* 128, 128, 128 */
#define COLOR_GREY           0x8410      /* 128, 128, 128 */
#define COLOR_SKYBLUE        0x867D      /* 135, 206, 235 */
#define COLOR_BLUEVIOLET     0x895C      /* 138,  43, 226 */
#define COLOR_LIGHTGREEN     0x9772      /* 144, 238, 144 */
#define COLOR_DARKVIOLET     0x901A      /* 148,   0, 211 */
#define COLOR_YELLOWGREEN    0x9E66      /* 154, 205,  50 */
#define COLOR_BROWN          0xA145      /* 165,  42,  42 */
#define COLOR_DARKGRAY       0x7BEF      /* 128, 128, 128 */
#define COLOR_DARKGREY       0x7BEF      /* 128, 128, 128 */
#define COLOR_SIENNA         0xA285      /* 160,  82,  45 */
#define COLOR_LIGHTBLUE      0xAEDC      /* 172, 216, 230 */
#define COLOR_GREENYELLOW    0xAFE5      /* 173, 255,  47 */
#define COLOR_SILVER         0xC618      /* 192, 192, 192 */
#define COLOR_LIGHTGRAY      0xC618      /* 192, 192, 192 */
#define COLOR_LIGHTGREY      0xC618      /* 192, 192, 192 */
#define COLOR_LIGHTCYAN      0xE7FF      /* 224, 255, 255 */
#define COLOR_VIOLET         0xEC1D      /* 238, 130, 238 */
#define COLOR_AZUR           0xF7FF      /* 240, 255, 255 */
#define COLOR_BEIGE          0xF7BB      /* 245, 245, 220 */
#define COLOR_MAGENTA        0xF81F      /* 255,   0, 255 */
#define COLOR_TOMATO         0xFB08      /* 255,  99,  71 */
#define COLOR_GOLD           0xFEA0      /* 255, 215,   0 */
#define COLOR_ORANGE         0xFD20      /* 255, 165,   0 */
#define COLOR_SNOW           0xFFDF      /* 255, 250, 250 */
#define COLOR_YELLOW         0xFFE0      /* 255, 255,   0 */
