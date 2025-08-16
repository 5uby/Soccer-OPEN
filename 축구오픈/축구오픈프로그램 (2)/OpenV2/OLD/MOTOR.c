/*
 * Motor.h
 *
 * History
 *
 * 2019-07-01
 * 
 *  헤더 파일들 정리 작업.
 *  모터 구동 관련 함수들.
 *
 */ 
volatile unsigned int OldDirA =0;
volatile unsigned int OldDirB =0;
volatile unsigned int OldDirC =0;
volatile unsigned int OldDirD =0;

void drawText(uint16_t x, uint16_t y, String s, uint16_t color);
void ReadHeading(void);

void SHOOT(unsigned int ms)
{
  SHOOTERON;
  double start = millis();
  while((millis()-start < ms))
  {
    if(LineDetected)  dir_move(0,0);
    else dir_move(0,100);
    
    delay(1);
  }
  if(LineDetected) dir_move(0,0);
  else  dir_move(0,60);
  SHOOTEROFF;
}

int Sign(int value)
{
	if (value==0) return 0;
	else if (value<0) return -1;
	else return 1;
}

void MOTORA(int ma)
{
	int tmp = abs(ma);
	
	if(Voltage < 0)
	{
		//WarningDisplay();
	}
	else
	{
		if(Sign(ma) != OldDirA)
		{
      OCR3B = 0;
      //delay(20);
		}
		
		tmp=tmp*255/100;
		if(tmp>250)	tmp=250;
		
		if(ma<0)	madirB;
		else		madirF;

		OCR3B = tmp;
      
		OldDirA=Sign(ma);
	}
}

void MOTORB(int mb)
{
  int tmp = abs(mb);
  
  if(Voltage < 0)
  {
    //WarningDisplay();
  }
  else
  {
    if(Sign(mb) != OldDirB)
    {
      OCR3C = 0;
      //delay(20);
    }
    
    tmp=tmp*255/100;
    if(tmp>250) tmp=250;
    
    if(mb<0)  mbdirB;
    else    mbdirF;

    OCR3C = tmp;
      
    OldDirB=Sign(mb);
  }
}

void MOTORC(int mc)
{
  int tmp = abs(mc);
  
  if(Voltage < 0)
  {
    //WarningDisplay();
  }
  else
  {
    if(Sign(mc) != OldDirC)
    {
      OCR4B = 0;
      //delay(20);
    }
    
    tmp=tmp*255/100;
    if(tmp>250) tmp=250;
    
    if(mc<0)  mcdirB;
    else    mcdirF;

    OCR4B = tmp;
      
    OldDirC=Sign(mc);
  }
}

void MOTORD(int md)
{
  int tmp = abs(md);
  
  if(Voltage < 0)
  {
    //WarningDisplay();
  }
  else
  {
    if(Sign(md) != OldDirD)
    {
      OCR4C = 0;
      //delay(20);
    }
    
    tmp=tmp*255/100;
    if(tmp>250) tmp=250;
    
    if(md<0)  mddirB;
    else    mddirF;

    OCR4C = tmp;
      
    OldDirD=Sign(md);
  }
}

void motor_stop(void)
{
   MOTORA(0);
   MOTORB(0);
   MOTORC(0);
   MOTORD(0);
}

void move(int ma, int mb, int mc)
{
   MOTORA(ma);
   MOTORB(mb);
   MOTORC(mc);
}
