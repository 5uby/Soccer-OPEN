/*
 * Motor.h
 *
 * History
 *
 * 2024-04-30
 * 
 *  모터 구동 관련 함수들.
 *
 */
 #define KP 1.0 //1.0 
volatile unsigned int OldDirA =0;
volatile unsigned int OldDirB =0;
volatile unsigned int OldDirC =0;
volatile unsigned int OldDirD =0;

void drawText(uint16_t x, uint16_t y, String s, uint16_t color);
void ReadHeading(void);

#define Voltage 100

void SHOOT(unsigned int ms)
{
  if(ms > 100) ms = 100;
  
  ShooterON;
  double start = millis();
  while((millis()-start < ms))
  {
//    if(LineDetected)  dir_move(0,0);
//    else dir_move(0,100);
    
    delay(1);
  }
//  if(LineDetected) dir_move(0,0);
//  else  dir_move(0,60);
  ShooterOFF;
}

void MOTORA(int ma)
{
  int tmp = abs(ma); 

  tmp=tmp*255/100;
  if(tmp>250) tmp=250;
  
  if(ma<0)
  {
    TCCR2A |= 0x20; //Set COM2B1, OC2B PWM MODE (Non Inverting)
  }
  else
  {
    TCCR2A &= 0xDF; //Clear COM2B1, OC2B(PH6) Normal Port Operation
    PORTH &= 0xBF;  //OutPut LOW to PH6
  }

  OCR2A = tmp;
  OCR2B = tmp;
}

void MOTORB(int mb)
{
  int tmp = abs(mb); 

  tmp=tmp*255/100;
  if(tmp>250) tmp=250;
  
  if(mb<0)
  {
    TCCR3A |= 0x20; //Set COM3B1, OC3B PWM MODE (Non Inverting)
  }
  else
  {
    TCCR3A &= 0xDF; //Clear COM3B1, OC3B(PE4) Normal Port Operation
    PORTE &= 0xEF;  //OutPut LOW to PE4
  }

  OCR3A = tmp;
  OCR3B = tmp;
}

void MOTORC(int mc)
{
  int tmp = abs(mc); 

  tmp=tmp*255/100;
  if(tmp>250) tmp=250;
  
  if(mc<0)
  {
    TCCR4A |= 0x20; //Set COM4B1, OC4B PWM MODE (Non Inverting)
  }
  else
  {
    TCCR4A &= 0xDF; //Clear COM4B1, OC4B(PH4) Normal Port Operation
    PORTH &= 0xEF;  //OutPut LOW to PH4
  }

  OCR4A = tmp;
  OCR4B = tmp;
}

void MOTORD(int md)
{
  int tmp = abs(md); 

  tmp=tmp*255/100;
  if(tmp>250) tmp=250;
  
  if(md<0)
  {
    TCCR5A |= 0x20; //Set COM5B1, OC5B PWM MODE (Non Inverting)
  }
  else
  {
    TCCR5A &= 0xDF; //Clear COM5B1, OC5B(PL4) Normal Port Operation
    PORTL &= 0xEF;  //OutPut LOW to PL4
  }

  OCR5A = tmp;
  OCR5B = tmp;
}

void MOVE(int ma, int mb, int mc)
{
  MOTORA(ma);
  MOTORB(mb);
  MOTORC(mc);
}

void SetToFront()
{
  ReadHeading();  //컴파스 정보 읽기

  while(compass < 175 || compass > 185)
  {
    ReadHeading();  //컴파스 정보 읽기
    
    int _pwr = (compass-180)/2;
    
    if(_pwr < 0)  _pwr -= 15;
    else          _pwr += 15;
    
    MOVE(_pwr, _pwr, _pwr);

  }
  MOVE(0,0,0);
}

void CompassMove(int ma, int mb, int mc)
{
  ReadHeading();

  if(Heading < -179) Heading = -179;
  if(Heading > 180) Heading = 180;
  
  int Proportial = (int)((double)Heading * KP);
  if (Proportial > 20)      Proportial = 20;
  else if(Proportial < -20) Proportial = -20;
  
  if (Heading > 100)
  {
    MOVE(50, 50, 50);
  }
  else if (Heading < -100)
  {
    MOVE(-50, -50, -50);
  }
  else
  {
    MOVE(ma + Proportial, mb + Proportial, mc + Proportial);
  }
}

void DirMove(int dir, int power)
{
  if(power == 0)
  {
    SetToFront();
  }
  else
  {
    switch (dir)
    {
      case 0:
        CompassMove(power, 0, -power);
        break;
      case 1:
        CompassMove(power * 0.5747, power * 0.5747, -power);
        break;
      case 2:
        CompassMove(0, power, -power);
        break;
      case 3:
        CompassMove(-power * 0.5747, power, -power * 0.5747);
        break;
      case 4:
        CompassMove(-power, power, 0);
        break;
      case 5:
        CompassMove(-power, power * 0.5747, power * 0.5747);
        break;
      case 6:
        CompassMove(-power, 0, power);
        break;
      case 7:
        CompassMove(-power * 0.5747, -power * 0.5747, power);
        break;
      case 8:
        CompassMove(0, -power, power);
        break;
      case 9:
        CompassMove(power * 0.5747, - power, power * 0.5747);
        break;
      case 10:
        CompassMove(power, -power, 0);
        break;
      case 11:
        CompassMove(power, -power * 0.5747, -power * 0.5747);
        break;
    }
    delay(1);
  }
}

void LineMove()
{
  switch(Line)
  {
    case 1:
    if (UltraSensor[3] < 15){
      DirMove(3,60);
    }
    else if (UltraSensor[1] < 15){
      DirMove(9,60);
    }
    else{
      DirMove(6,60);
    }
    break;

    case 2:
    DirMove(9,60);
    break;

    case 3:
    DirMove(8,60);
    break;

    case 4:
    if (UltraSensor[3] < 15){
      DirMove(3,60);
    }
    else if (UltraSensor[1] < 15){
      DirMove(9,60);
    }
    else{
      DirMove(0,60);
    }
    break;

    case 6:
    DirMove(10,60);
    break;

    case 7:
    DirMove(9,60);
    break;

    case 8:
    DirMove(3,60);
    break;

    case 9:
    DirMove(4,60);
    break;
    
    case 11:
    DirMove(6,60);
    break;

    case 12:
    DirMove(2,60);
    break;

    case 13:
    DirMove(3,60);
    break;

    case 14:
    DirMove(0,60);
    break;
  }
  delay(1);
}