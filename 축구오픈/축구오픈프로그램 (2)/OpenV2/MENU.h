/*
 * MENU.h
 *
 * History
 *
 * 2024-04-30
 * 
 * 메뉴 관련 기능들
 * 
 */ 


//바닥감지 센서값을 보여줄 LCD내 위치.
volatile int LCD_CX = 120;
volatile int LCD_CY = 233;

volatile int LSpos[12][2] = {{9,23},{20,25},{32,-2},{28,-11},{25,-21},{17,-28}
                                ,{-17,-28},{-25,-21},{-28,-11},{-32,-2},{-20,25},{-9,23}};

void drawText(uint16_t x, uint16_t y, String s, uint16_t color)
{
  LCD.setCursor(x, y);
  LCD.setTextColor(color,  ILI9341_BLACK);
  LCD.print(s);
}

/*
 *  흰색선에 대한 정보 초기화
 */
void ClearLineStatus(void)
{
  Line12 = 0;
  LineDetected12 = 0;
  Line = 0;
  LineDetected = 0;
}
 
/*
 *  메뉴 선택 관련 기능들
 *  현재 메뉴 위치는 menu 변수에 저장되어 있음
 */
void decreaseMenu(void)
{
  while(KEY_LEFT) ; // 버튼이 눌렀다 놓을때까지 대기
  menu--;
  if(menu < 0) menu = 2;
}
void increaseMenu(void)
{
  while(KEY_RIGHT) ;  // 버튼이 눌렀다 놓을때까지 대기
  menu++;
  if(menu > 2) menu = 0;
}
void keyCheck(void)
{
  if(KEY_LEFT) decreaseMenu();
  else if(KEY_RIGHT) increaseMenu();
}

/*
 *  메인 전압 표시
 *  전압은 10으로 나눠야 함.
 */
void displayVolt(void)
{
  LCD.setTextSize(2);
  
  int color = MainVolt < 110 ? COLOR_RED : COLOR_GREEN;  
  String s = "    " + String((int)MainVolt/10)+"."+String((int)MainVolt%10)+"V";
  drawText(70, 300, s.substring(s.length()-5,s.length()) , color);   

  LCD.setTextSize(2);
  
  if(MotorVoltRAW > MainVoltRAW >> 1)  drawText(140, 300, "   ", COLOR_GREEN);
  else  drawText(140, 300, "OFF", COLOR_RED);
}

/*
 *  하단에 표시되는 문장
 */
void displayBottom(void)
{
  LCD.setTextSize(2);
  
  drawText(0, 300, "PREV", COLOR_BLUE);
  drawText(190, 300, "NEXT", COLOR_BLUE);

  drawText(180, 279, "ULTRA" , COLOR_WHITE);    
  drawText(0, 279, "CAM" , COLOR_WHITE);    
}

/*
 *  공격 방향에 맞게 골대 색상 보여주기
 */
void GoalDisplay()
{
  LCD.setTextSize(2);
  
  if(AttackTo == toYellow)
  {
    LCD.fillRect(59,0,120,25, COLOR_YELLOW);
    LCD.fillRect(59,273,120,25, COLOR_BLUE);
  }
  else
  {
    LCD.fillRect(59,0,120,25, COLOR_BLUE);
    LCD.fillRect(59,273,120,25, COLOR_YELLOW);
  }

  drawText(80, 6, "TARGET", COLOR_BLACK);
  drawText(100, 279, "OUR", COLOR_BLACK);
}

/*
 *  흰색선 감지 기준값 표시
 */
void ThresholdDisplay(void)
{
  String s;

  LCD.setTextSize(1);
  
  for (int i = 0; i < 6; i++) 
  {
    s = "   " + String(Threshold[i]);
    drawText(222, i*10+135, s.substring(s.length()-3,s.length()) , COLOR_GREEN);  
  }
  for (int i = 0; i < 6; i++) 
  {
    s = "   " + String(Threshold[11-i]);
    drawText(0, i*10+135, s.substring(s.length()-3,s.length()) , COLOR_GREEN);  
  }
}

/*
 *  카메라 정보 표시
 *  LCD내 특정 영역에 지속적으로 공과 골대에 대한 정보 표시
 */
void CamDataDisplay(char c)
{
  int CX = 120;
  int CY = 109;

  LCD.fillRect(59,30, 121, 161, COLOR_BLACK);

  LCD.setTextSize(2);

  if(c == 0)
  {
    drawText(0, 259, "OK!" , COLOR_YELLOW);
    LCD.drawFastHLine(60, 109, 121, COLOR_WHITE);
    LCD.drawFastVLine(120, 31, 161, COLOR_WHITE);
  
    if(OBJ[0].found)
    {
      int x = (int)(OBJ[0].x/4 + CX);
      int y = (int)(CY - OBJ[0].y/4);
      LCD.fillCircle(x,y,8,COLOR_YELLOW);
      LCD.drawLine(CX,CY ,x,y, COLOR_YELLOW);
    }
    
    if(OBJ[1].found)
    {
      int x = (int)(OBJ[1].x/4 + CX);
      int y = (int)(CY - OBJ[1].y/4);
      LCD.fillCircle(x,y,8,COLOR_BLUE);
      LCD.drawLine(CX,CY ,x,y, COLOR_BLUE);   
    }  
    
    if(OBJ[2].found)
    {
      int x = (int)(OBJ[2].x/4 + CX);
      int y = (int)(CY - OBJ[2].y/4);
      LCD.fillCircle(x,y,6,COLOR_RED);
      LCD.drawLine(CX,CY ,x,y, COLOR_RED);   
    }
  }
  else
  {
    drawText(0, 259, "FAIL" , COLOR_RED);
    
    LCD.setTextSize(2);
    drawText(65, 90, "CAM FAIL", COLOR_YELLOW);
    
    LCD.setTextSize(1);

    if(c==1)      drawText(65, 120, "CHECKSUM ERROR", COLOR_WHITE);
    else if(c==2) drawText(65, 120, "NO S.O.P", COLOR_WHITE);
    else if(c==3) drawText(65, 120, "NO Response", COLOR_WHITE);
    else if(c==4) drawText(65, 120, "UART Access Err.", COLOR_WHITE);
    else if(c==5) drawText(65, 120, "TIME OUT", COLOR_WHITE);
    else          drawText(65, 120, "Unknown Error", COLOR_WHITE);
  }
}

/*
 *  센서값들을 표시 함.
 *  방향, 초음파, 빛센서
 */
void SensorValueDisplay(void)
{
  
  LCD.setTextSize(2);

  //Robot Heading
  String s = "   " + String(Heading);
  drawText(LCD_CX-27, LCD_CY - 8, s.substring(s.length()-4,s.length()) , COLOR_WHITE);    

  //ULTRA SENSOR (FRONT, RIGHT, REAR, LEFT)
  for(char sel = 0; sel < 4; sel++)
  {
    s = "   " + String(UltraSensor[sel]);
    drawText(202, 200 + sel * 20, s.substring(s.length()-3,s.length()) , COLOR_YELLOW);    
  }

  LCD.setTextSize(1);

  //LIGHT SENSOR VALUE (0 -> 11h)
  for (int i = 0; i < 6; i++) 
  {
    s = "   " + String(LightSensor[i]);
    drawText(202, i*10+135, s.substring(s.length()-3,s.length()) , COLOR_WHITE);  
  }
  for (int i = 0; i < 6; i++) 
  {
    s = "   " + String(LightSensor[11-i]);
    drawText(20, i*10+135, s.substring(s.length()-3,s.length()) , COLOR_WHITE);  
  }

  //BALL DETECTION SENSOR VALUE IN CAPTURING ZONE
  LCD.setTextSize(1);
  s = "   " + String(LightSensor[12]);
  drawText(LCD_CX-9, LCD_CY - 35, s.substring(s.length()-3,s.length()) , COLOR_WHITE);    

  //Display Line Sensor Status
  //Green : 선을 감지한적이 없음
  //White : 현재 선을 감지 하였음.
  //RED   : 선을 감지 했던 적이 있음.
  for(int i = 0; i < 12; i++)
  {
    if(KEY_IN) break;
  
    if(Line12 & (1<<i)) LCD.fillCircle(LCD_CX + LSpos[i][0], LCD_CY - LSpos[i][1], 3, COLOR_WHITE);
    else if(LineDetected12 & (1<<i)) LCD.fillCircle(LCD_CX + LSpos[i][0], LCD_CY - LSpos[i][1], 3, COLOR_RED);
    else LCD.fillCircle(LCD_CX + LSpos[i][0], LCD_CY - LSpos[i][1], 3, COLOR_GREEN);
  }
}

/*
 *  공수 방향 전환 
 */
void ChangeDirection(void)
{
  if(AttackTo == toYellow)  AttackTo = toBlue;
  else                      AttackTo = toYellow;
  EEPROM.write(12,AttackTo);
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

  GoalDisplay();
  while(KEY_ENTER) ;
}    

void DisplayKeyDESCRIPTION(void)
{
  LCD.setTextSize(2);
  
  drawText(0, 30, "ENT", COLOR_YELLOW);
  drawText(0, 85, "F2", COLOR_YELLOW);
  drawText(190, 85, "F1", COLOR_YELLOW); 
   
  LCD.setTextSize(1);
  
  drawText(0, 50, "CHANGE", COLOR_WHITE);
  drawText(0, 60, "DIRECTION", COLOR_WHITE);

  drawText(0, 105, "DEFENCE", COLOR_WHITE);
  drawText(190, 105, "ATTACK", COLOR_WHITE); 
}

/* 
 *  메인 화면
 *  로봇 모든 상태를 표시 함.
 */
void DisplayStatus(void)
{
  String s;

  LCD.fillScreen(ILI9341_BLACK);
  
  ThresholdDisplay();
  GoalDisplay();  
  displayBottom();  
  DisplayKeyDESCRIPTION();
  
  LCD.drawRect(58,29, 123, 163, COLOR_GREEN); //SOCCER FIELD
  LCD.drawCircle(LCD_CX,LCD_CY, 40, COLOR_WHITE);

  while(!KEY_IN)
  {
    char Err = ReadCAM(0);
    ReadHeading();
 
    displayVolt();
    SensorValueDisplay();
    CamDataDisplay(Err);

    if(KEY_ENTER) ChangeDirection();
    if(KEY_IN) break;
  }
  //keyCheck();
}

/* 
 *  모터, 솔레노이드 테스트
 */
void ActuatorTest(void)
{
  LCD.fillScreen(ILI9341_BLACK);
  
  LCD.setTextSize(3);
  drawText(0, 40, "ACTUATOR TEST", COLOR_GREEN);
  
  LCD.setTextSize(2);
  drawText(10, 90, "IF WANT MOTOR TEST", COLOR_WHITE);
  drawText(40, 110, "LIFT UP ROBOT", COLOR_WHITE);
  
  drawText(10, 140, "F1 : MOTOR CW(30)", COLOR_YELLOW);
  drawText(10, 170, "F2 : MOTOR CCW(-30)", COLOR_YELLOW);
  drawText(10, 200, "ENTER : SHOOTER", COLOR_YELLOW);

  drawText(25, 240, "CHECK ERROR LEDS", COLOR_WHITE);
  
  displayBottom();

  while(!KEY_LEFT && !KEY_RIGHT)
  {
    displayVolt();
    
    if(KEY_ENTER)
    {
      SHOOT(100);
      while(KEY_ENTER) ;
    }
    if(KEY_F1)
    {
      MOVE(30,30,30);
      MOTORD(30);
      while(KEY_F1) ;
      MOVE(0,0,0);
      MOTORD(0);
    }
    else if(KEY_F2)
    {
      MOVE(-30,-30,-30);
      MOTORD(-30);
      while(KEY_F2) ;
      MOVE(0,0,0);
      MOTORD(0);
    }
    delay(200);
  }
//  keyCheck();
}

/* 
 *  흰색 선 기준값 계산 실행
 */
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

  MOVE(20,20,20);

  double starttime = millis();
  while (millis()-starttime < 3000)
  {
    for ( int i = 0 ; i < 12 ; i++)
    {
      if (LightSensor[i] < minV[i]) minV[i] = LightSensor[i];
      else if (LightSensor[i] > maxV[i]) maxV[i] = LightSensor[i];
      diff[i] = maxV[i] - minV[i];
    }
    //delay(1);
  }

  MOVE(0,0,0);

  /* 
   *  흰색선의 기준값 계산
   *  곱하는 값을 조정하여 최적화
  */
  for(int i = 0; i <12; i++)
  {
    //unsigned char thr  = minV[i] + (char)(diff[i] * 0.9); // 최대 최소값 차이의 90%
    unsigned char thr  = (char)(maxV[i] * 0.9);             // 최대치의 90%
    EEPROM.write(i,thr);
    Threshold[i]= thr;
    
    ClearLineStatus();
  }
}

/* 
 *  바닥감지 빛센서 칼리브레이션
 */
void Calibration(void)
{
  LCD.fillScreen(ILI9341_BLACK);
  LCD.setTextSize(2);

  drawText(60, 170, "CALIBRATION", COLOR_YELLOW);
  drawText(30, 200, "PUT ON WHITE LINE", COLOR_WHITE);
  drawText(60, 230, "PRESS ENTER", COLOR_YELLOW);

  displayBottom();

  while(!KEY_IN)
  {
    displayVolt();
    
    if(KEY_ENTER)
    {
      while(KEY_ENTER)  ; // 버튼을 놓을때까지 기다림.
      SetCalibration();
      LCD.fillScreen(ILI9341_BLACK);
      LCD.setTextSize(2);
    
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
  //keyCheck();  
}
