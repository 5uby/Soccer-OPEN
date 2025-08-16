/*
 * MENU.h
 *
 * History
 *
 * 2023-07-03
 * 
 *
 */ 

#include "PROGRAM.h"


//바닥감지 센서값을 보여줄 LCD 위치.
volatile int LCD_CX = 120;
volatile int LCD_CY = 170;

volatile int LCDpos[6][2] = {  {180,120}, {100,220}, {20,120}
                              , {140,140}, {100,180}, {60,140}};

volatile int LSpos[12][2] = {{15,34},{28,36},{45,-1},{45,-13},{36,-29},{22,-40}
                                ,{-22,-40},{-36,-29},{-45,-13},{-45,-1},{-28,36},{-15,34}};

//volatile int LCDpos[12][2] = {  {50,120}, {100,120}, {150,120}
//                              , {150, 140}, {150, 160}, {150, 180}  
//                              , {150,200}, {100,200}, {50,200}
//                              , {50, 180}, {50, 160}, {50, 140}  };
//
void displayVolt(void)
{
  tft.setTextSize(2);
  
  int color = Voltage < 110 ? COLOR_RED : COLOR_GREEN;  
  String s = "    " + String((int)Voltage/10)+"."+String((int)Voltage%10)+"V";
  drawText(90, 300, s.substring(s.length()-5,s.length()) , color);   

  tft.setTextSize(2);
  
  if(ADC_Value[14] > ADC_Value[13] >> 1)  drawText(150, 300, "   ", COLOR_GREEN);
  else  drawText(150, 300, "OFF", COLOR_RED);
  
}

void WarningDisplay()
{
  motor_stop();
  tft.setTextSize(2);

  drawText(50, 160, " LOW VOLTAGE", COLOR_RED);
  drawText(50, 200, "CHANGE BATTERY", COLOR_RED);
  
  while(1)  displayVolt();
}

void decreaseMenu(void)
{
  while(KeyLeft) ;
  menu--;
  if(menu < 0) menu = 5;
}

void increaseMenu(void)
{
  while(KeyRight) ;
  menu++;
  if(menu > 5) menu = 0;
}

void keyCheck(void)
{
  if(KeyLeft) decreaseMenu();
  else if(KeyRight) increaseMenu();
}

void displayBottom(void)
{
  tft.setTextSize(2);
  drawText(0, 300, "PREV", COLOR_BLUE);
  drawText(190, 300, "NEXT", COLOR_BLUE);
}

void GoalDisplay()
{
  if(AttackTo == toYellow)
  {
    tft.fillRect(59,0,120,25, COLOR_YELLOW);
    tft.fillRect(59,273,120,25, COLOR_BLUE);
  }
  else
  {
    tft.fillRect(59,0,120,25, COLOR_BLUE);
    tft.fillRect(59,273,120,25, COLOR_YELLOW);
  }

  drawText(80, 6, "TARGET", COLOR_BLACK);
  drawText(100, 279, "OUR", COLOR_BLACK);
}

void DisplayCamdata(void)
{
  String s;
  
  if(Ball_X == -60 + X_offset && Ball_Y == 0)
  {
    drawText(20, 28, "   X" , COLOR_RED);    
    drawText(20, 46, "   X" , COLOR_RED);      
  }
  else
  {
    s = "    " + String(Ball_X);
    drawText(20, 28, s.substring(s.length()-4,s.length()) , COLOR_RED);    
    s = "    " + String(Ball_Y);
    drawText(20, 46, s.substring(s.length()-4,s.length()) , COLOR_RED);  
  }

}

void DisplayStatus(void)
{
  String s;

  //tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);

  GoalDisplay();  
  displayBottom();  
  //tft.drawCircle(120, 150, 110, COLOR_GRAY);
  
  drawText(180, 180, "ULTRA" , COLOR_WHITE);    
  drawText(0, 180, "CAMERA" , COLOR_WHITE);    
        
  while(!KeyLeft && !KeyRight)
  {
    displayVolt();
    ReadHeading();
    
    if(ReadCAM(0)==0) drawText(0, 200, "OKAY" , COLOR_YELLOW);
    else drawText(0, 200, "FAIL" , COLOR_RED);
       
    s = "   " + String(Heading);
    drawText(100, 60, s.substring(s.length()-4,s.length()) , COLOR_YELLOW);    
  
    for(char sel = 0; sel < 4; sel++)
    {
      s = "   " + String(ultra[sel]);
      drawText(190, 200 + sel * 20, s.substring(s.length()-4,s.length()) , COLOR_YELLOW);    
    }

    tft.drawCircle(LCD_CX,LCD_CY, 50, COLOR_WHITE);
  
    for(int i = 0; i < 12; i++)
    {
      if(Line & (1<<i)) tft.fillCircle(LCD_CX + LSpos[i][0], LCD_CY - LSpos[i][1], 3, COLOR_WHITE);
      else if(LineDetected & (1<<i)) tft.fillCircle(LCD_CX + LSpos[i][0], LCD_CY - LSpos[i][1], 3, COLOR_RED);
      else tft.drawCircle(LCD_CX + LSpos[i][0], LCD_CY - LSpos[i][1], 3, COLOR_GREEN);
    }

    tft.setTextSize(2);

    for (int i = 0; i < 6; i++) 
    {
      s = "   " + String(ADC_Value[i]);
      drawText(190, i*20+40, s.substring(s.length()-4,s.length()) , COLOR_WHITE);  
    }
    for (int i = 0; i < 6; i++) 
    {
      s = "   " + String(ADC_Value[11-i]);
      drawText(0, i*20 + 40, s.substring(s.length()-4,s.length()) , COLOR_WHITE);  
    }
    
   s = "   " + String(ADC_Value[12]);
    drawText(100, 100, s.substring(s.length()-3,s.length()) , COLOR_BLUE);    
    
  }
  keyCheck();
}

void shootingTest(void)
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);

  drawText(40, 200, "SHOOTING TEST", COLOR_YELLOW);
  drawText(40, 230, " PRESS ENTER", COLOR_YELLOW);

  displayBottom();

  while(!KeyLeft && !KeyRight)
  {
    displayVolt();
    
    if(KeyEnter)
    {
      SHOOTERON;
      delay(500);
      SHOOTEROFF;
      while(KeyEnter) ;
    }
    delay(200);
  }
  keyCheck();
}

void defenceMenu(void)
{

}

void attackMenu(void)
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);

  GoalDisplay();
  
  tft.setTextSize(3);

  drawText(60, 180, "ATTACK", COLOR_BLUE);
  tft.setTextSize(2);
  drawText(50, 230, "PRESS ENTER", COLOR_YELLOW);

  displayBottom();

  while(!KeyLeft && !KeyRight)
  {
    displayVolt();
    
    if(KeyEnter)
    {
      tft.fillScreen(ILI9341_BLACK);
      tft.setTextSize(3);
      drawText(30, 180, " ATTACK  ", COLOR_WHITE);
      
      while(true)
      {
        //drawText(60, 100, "STOP", COLOR_WHITE);
        
        //while(PINK & 0x20)  ;
        
        //drawText(60, 100, " GO ", COLOR_WHITE);

        Attack();
        
        //while(!(PINK & 0x20))  ;
        
      }
    }
    
    delay(200);
  }
  keyCheck();
}

void SetCalibration(void)
{
  unsigned char minV[12];
  unsigned char maxV[12];
  unsigned char diff[12];

  for(int i = 0; i < 12; i++)
  {
    minV[i] = 255;
    maxV[i] = 0;
  }

  move(20,20,20);

  double starttime = millis();
  while (millis()-starttime < 3000)
  {
    for ( int i = 0 ; i < 12 ; i++)
    {
      if (ADC_Value[i] < minV[i]) minV[i] = ADC_Value[i];
      else if (ADC_Value[i] > maxV[i]) maxV[i] = ADC_Value[i];
      diff[i] = maxV[i] - minV[i];
    }
    //delay(1);
  }

  move(0,0,0);

  
  for(int i = 0; i <12; i++)
  {
//    unsigned char thr  = minV[i] + (char)(diff[i] * 0.9);
    unsigned char thr  = (char)(maxV[i] * 0.9);
    EEPROM.write(i,thr);
    Threshold[i]= thr;
    
    Line = 0;
    LineDetected = 0;

  }
  
}

void Calibration(void)
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);

  drawText(60, 170, "CALIBRATION", COLOR_YELLOW);
  drawText(30, 200, "PUT ON WHITE LINE", COLOR_WHITE);
  drawText(60, 230, "PRESS ENTER", COLOR_YELLOW);

  displayBottom();

  while(!KeyLeft && !KeyRight)
  {
    displayVolt();
    
    if(KeyEnter)
    {
      SetCalibration();
      tft.fillScreen(ILI9341_BLACK);
      tft.setTextSize(2);
    
      drawText(60, 170, "CALIBRATION", COLOR_YELLOW);
      drawText(80, 200, "COMPLETE", COLOR_WHITE);
      drawText(40, 230, "WAIT 1 SECOND", COLOR_YELLOW);
    
      displayBottom();
      delay(1000);
      menu=2;
      return;

    }
    delay(200);
  }
  keyCheck();  
}

void ChangeDirInfo(void)
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);

  GoalDisplay();
  
  drawText(70, 130, "ATTACK TO", COLOR_WHITE);
  if(AttackTo == toYellow) drawText(90, 150, "YELLOW", COLOR_YELLOW);
  else                     drawText(100, 150, "BLUE", COLOR_BLUE);
  drawText(0, 200, "IF CHANGE DIRECTION", COLOR_RED);
  drawText(60, 230, "PRESS ENTER", COLOR_WHITE);

  displayBottom();
}

void ChangeDir(void)
{
  ChangeDirInfo();
  
  while(!KeyLeft && !KeyRight)
  {
    displayVolt();
    
    if(KeyEnter)
    {
      if(AttackTo == toYellow)  AttackTo = toBlue;
      else                      AttackTo = toYellow;
      EEPROM.write(40,AttackTo);
      ChangeDirInfo();
      while(KeyEnter) ;
    }
    delay(200);
  }
  keyCheck();  

  if (AttackTo == toBlue)
  {
    MyGoal = 0;
    TgtGoal = 1;
  }
  else
  {
    MyGoal = 1;
    TgtGoal = 0;          
  }
}

void ViewCamData(void)
{
  int x,y;
  
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);

  drawText(80, 6, "CAMERA", COLOR_WHITE);
  
  displayBottom();

  while(!KeyLeft && !KeyRight)
  {
    displayVolt();
    tft.fillRect(0,25,239,275, COLOR_BLACK);
    tft.drawLine(120,25,120,275, COLOR_GRAY);  
    tft.drawLine(0,150,239,150, COLOR_GRAY);  
    
    char c = ReadCAM(0);
    if(c == 0)
    {
      if(OBJ[0].found)
      {
        x = (int)(OBJ[0].x/2 + 120);
        y = (int)(150 - OBJ[0].y/2);
        tft.fillCircle(x,y,8,COLOR_YELLOW);
        tft.drawLine(120,150,x,y, COLOR_YELLOW);      
      }
      
      if(OBJ[1].found)
      {
        x = (int)(OBJ[1].x/2 + 120);
        y = (int)(150 - OBJ[1].y/2);
        tft.fillCircle(x,y,8,COLOR_BLUE);
        tft.drawLine(120,150,x,y, COLOR_BLUE);    
      }  
      
      if(OBJ[2].found)
      {
        x = (int)(OBJ[2].x/2 + 120);
        y = (int)(150 - OBJ[2].y/2);
        tft.fillCircle(x,y,8,COLOR_RED);
        tft.drawLine(120,150,x,y, COLOR_RED);      
      }
      drawText(10, 279, "CAMERA OKAY", COLOR_WHITE);
    }
    else
    {
      if(c==1)      drawText(10, 279, "CHECKSUM ERROR", COLOR_RED);
      else if(c==2) drawText(10, 279, "NO S.O.P", COLOR_RED);
      else if(c==3) drawText(10, 279, "NO Response", COLOR_RED);
      else if(c==4) drawText(10, 279, "UART Access Err.", COLOR_RED);
      else if(c==5) drawText(10, 279, "TIME OUT", COLOR_RED);
      else          drawText(10, 279, "Unknown Error", COLOR_RED);
    }
    if(KeyEnter)
    {
      tft.fillScreen(ILI9341_BLACK);
      tft.setTextSize(3);
      drawText(30, 180, "SUPER TEAM", COLOR_WHITE);      
    }
    delay(200);
  }
  keyCheck();  
}
