/*
 * OpenV2.c
 *
 * Created: 2024-04-11
 * Author: 
 *
 * History
 *
 */ 

#include "Initialize.h"


#define DEBUG true
//#define DEBUG false

#define BNO055_SAMPLERATE_DELAY_MS 10.0

//************************************************************************************
void ReadHeading()   // 상대 방향값. (-179 ~ 180)
{
    sensors_event_t event;

    bno.getEvent(&event);    
    
    int h = event.orientation.x;    
    delay(BNO055_SAMPLERATE_DELAY_MS);
    
    //if(h < 0) h += 360;
    if(h > 180) h -= 360;

    Heading = h;

    h += 180;
    if(h > 360) h -= 360;
    if(h < 0) h += 360;
    
    compass = h;

}

void read_BNO055(void)
{
  ReadHeading();
}


void setup(void) 
{
  
  init_devices();

  //시작 화면
  LCD.fillScreen(COLOR_BLACK);
  LCD.setTextSize(3);

  for (int i = 0;i < 7;i++){
    drawText(65, 120, "UPLOAD", COLOR_WHITE);
    drawText(30, 180, "SUCCESSFUL", COLOR_WHITE);
  }

  Timer1.initialize(59);  //타이머 인터럽트 주기 59us. (초음파 물체 감지거리 1cm  :  59us * 340m = 2.001 cm)
  Timer1.attachInterrupt(TimerInterrupt); //59us 간격으로 Scan_Sensors() 실행.

  ClearLineStatus();

}


void loop(void)
{
  keyCheck();
  if(KEY_F1)  ATTACK();
  if(KEY_F2)  DEFENCE();
  
  switch(menu)
  {
    case 0: DisplayStatus();
            break;
    case 1: ActuatorTest();
            break;
    case 2: Calibration();
            break;
  }
}
