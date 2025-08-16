/*
 * INITIALIZE.h
 *
 * History
 *
 * 2024-04-30
 * 
 * 로봇 초기화
 *
 */ 
#include "PinDefine.h"
#include "OpenMV.h"

#include <ARDUINO.h>
#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>
//#include <Time.h>
#include <TimerOne.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_BNO055.h>

Adafruit_ILI9341 LCD = Adafruit_ILI9341(LCD_CS, LCD_DC, LCD_RST);
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x29);

//----------------------------------------------------------------------
// 빛 센서  관련 정의 
//----------------------------------------------------------------------
#define TurnOnLeds  4
volatile unsigned char AdcIndex = 0;
volatile unsigned char LightSensor[13];
volatile unsigned char Threshold[12];
//----------------------------------------------------------------------

#define NEARBALL 70

//----------------------------------------------------------------------
// 초음파 관련 정의 
//----------------------------------------------------------------------
volatile unsigned int UltraSensor[4] = {0,0,0,0};     
volatile unsigned int Pulse_Width_Count=0;
volatile unsigned int ECHO_CNT[4];
volatile unsigned char ECHO_REPLY[4]={0,0,0,0};
volatile unsigned char US_SEQ = 0;
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// 전압 관련 정의 
//----------------------------------------------------------------------
volatile unsigned int MainVoltRAW = 0;
volatile unsigned int MotorVoltRAW = 0;
#define MainVolt (int)(((1.6/26)*MainVoltRAW + (12.6 - 1.6/26*207))*10)
#define MotorVolt (int)(((1.6/26)*MotorVoltRAW + (12.6 - 1.6/26*207))*10)
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// 선 감지 관련 정의 
//----------------------------------------------------------------------
volatile unsigned int Line12 = 0;
volatile unsigned char Line = 0;
volatile bool Escaping = false;  // 경계선에서 탈출을 시도한다는 Flag
volatile unsigned int LineDetected12 = 0;
volatile unsigned char LineDetected = 0;
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// 공격 방향 관련 정의 
//----------------------------------------------------------------------
#define toYellow 0
#define toBlue   1
volatile char AttackTo = toYellow;
volatile bool AttackToYellow = true;
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// 절대 방향 관련 정의 
//----------------------------------------------------------------------
volatile int Heading = 0;
volatile int compass = 0;
//----------------------------------------------------------------------

volatile char menu = 0;

#include "TimerInterrupt.h"
#include "MOTOR.h"
#include "MENU.h"
#include "UserProgram.h"

void PORT_init(void)
{
  DDRA=0x0F;
  PORTA=0xF8;

  DDRB=0x16;
  PORTB=0xFF;

  DDRC=0x0F;
  PORTC=0xFF;

  DDRD=0x8B;
  PORTD=0xFF;

  DDRE=0x18;
  PORTE=0xFF;

  DDRF=0x00;
  PORTF=0x00;

  DDRG=0x03;
  PORTG=0xFF; 

  DDRH=0x5A;
  PORTH=0xFF; 

  DDRJ=0x02;
  PORTJ=0xFF;

  DDRK=0x00;
  PORTK=0x00;

  DDRL=0x1F;
  PORTL=0xFF;
  
}

void PWM_Init(void)
{
  //MOTOR A
  TCCR2A = ((1<<COM2A1)|(1<<COM2A0)|(1<<COM2B1)|(1<<WGM20));
  TCCR2B = ((1<<CS21)|(1<<CS20));
  
  //MOTOR B
  TCCR3A = ((1<<COM3A1)|(1<<COM3A0)|(1<<COM3B1)|(1<<WGM30));
  TCCR3B = ((1<<CS31)|(1<<CS30));
  
  //MOTOR C
  TCCR4A = ((1<<COM4A1)|(1<<COM4A0)|(1<<COM4B1)|(1<<WGM40));
  TCCR4B = ((1<<CS41) | (1<<CS40));
  
  //MOTOR D
  TCCR5A = ((1<<COM5A1)|(1<<COM5A0)|(1<<COM5B1)|(1<<WGM50));
  TCCR5B = ((1<<CS51) | (1<<CS50));

  MOVE(0,0,0);
}

void ADC_init(void)
{
  ADCSRA = 0x00;          //disable adc
  ADMUX  = 0x60;          //select adc CH 0
  ACSR   = 0x80;          //Analog Comparator Disable
  //ADCSRA = 0x83;          //ADC CLOCK Pre-Scaler set to 3 (divide factor is 8) 
  ADCSRA = 0x84;          //ADC CLOCK Pre-Scaler set to 4 (divide factor is 16) 
  //ADCSRA = 0x86;          //ADC CLOCK Pre-Scaler set to 6 (divide factor is 64) 
  ADCSRA = ADCSRA & 0x3f; //ADC disable
  ADCSRA = ADCSRA | 0xc0; //ADC START & ADC Interrupt disable
}

void LED_init(void)
{
  for(int i=0;i<12-TurnOnLeds;i++)
  {
    PORTA &= 0xFE;  //IR_SERIN LOW
    PORTA |= 0x02;  //IR_SRCK HIGH
    PORTA &= 0xFD;  //IR_SRCK LOW
  }
  for(int i=0;i<TurnOnLeds;i++)
  {
    PORTA |= 0x01;  //IR_SERIN HIGH
    PORTA |= 0x02;  //IR_SRCK HIGH
    PORTA &= 0xFD;  //IR_SRCK LOW
  }
  PORTA |= 0x04;  //IR_RCK HIGH
  PORTA &= 0xFB;  //IR RCK LOW
  PORTA &= 0xF7;  //IR_ENABLE LOW
}

void init_devices(void)
{
	MCUCR = 0x00;
	EIMSK = 0x00;
  
  PORT_init();
  LED_init();
  
  PWM_Init();
  ADC_init();

  Serial.begin(115200);
  Serial3.begin(115200);
  
  Wire.begin();
  Wire.setClock(400000);
  delay(100);

  LCD.begin();
  LCD.setCursor(0, 0);
  LCD.setTextColor(ILI9341_WHITE);
  LCD.setTextSize(2);
  LCD.setRotation(0);
  LCD.fillScreen(ILI9341_BLACK);
    
  drawText(0, 0, "SOCCER OPEN", COLOR_BLUE);
  
  if(!bno.begin(Adafruit_BNO055::OPERATION_MODE_IMUPLUS))
  {
    drawText(0, 20, "BNO055 ERROR!", COLOR_RED);
    while(1) ;
  }  
  bno.setExtCrystalUse(true);

  drawText(0, 20, "WAIT 1 SECONDS!", COLOR_RED);

  // 라인 경계선 기준값 읽기.
  for(int  i=0; i<12; i++)
  {
    Threshold[i] = EEPROM.read(i);
  }
  
  AttackTo = EEPROM.read(12);
  if(AttackTo < 0 || AttackTo > 1)
  {
    AttackTo = 0;
    EEPROM.write(12,AttackTo);
  }

  if (AttackToYellow)
  {
    MyGoal = 1;
    TgtGoal = 0;
  }
  else
  {
    MyGoal = 0;
    TgtGoal = 1;          
  }
}
