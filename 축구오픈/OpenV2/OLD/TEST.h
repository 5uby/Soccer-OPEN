/*
 * TEST.h
 *
 * Created: 
 * Author: 
 *
 * History
 *
 * 테스트 프로그램들
 */ 

void DisplayStatus(void);
void drawText(uint16_t x, uint16_t y, String s, uint16_t color);
void ReadHeading(void);

void MotorTest(void)
{
  int delayTerm = 200;
  while(!KeyEnter)
  {
    
    for(int i = 0; i <= 100 ; i += 10)
    {
      move(i, 0, 0);
      delay(delayTerm);
    }
    for(int i = 100; i >= 0 ; i -= 10)
    {
      move(-i, 0, 0);
      delay(delayTerm);
    }
    
    for(int i = 0; i <= 100 ; i += 10)
    {
      move(0, i, 0);
      delay(delayTerm);
    }
    for(int i = 100; i >= 0 ; i -= 10)
    {
      move(0, -i, 0);
      delay(delayTerm);
    }
    /*
    for(int i = 0; i <= 100 ; i += 10)
    {
      move(0, 0, i);
      delay(delayTerm);
    }
    for(int i = 100; i >= 0 ; i -= 10)
    {
      move(0, 0, -i);
      delay(delayTerm);
    }
    move(0, 0, 0);
    for(int i = 0; i <= 100 ; i += 10)
    {
      MOTORD(i);
      delay(delayTerm);
    }
    for(int i = 100; i >= 0 ; i -= 10)
    {
      MOTORD(-i);
      delay(delayTerm);
    }
    MOTORD(0);
    */
  }
  while(KeyEnter) ;
}

void CircularMovingTest(void)
{
  while(!KeyEnter)  ;
  while(KeyEnter)  ;

  delay(1000);

  while(true)
  {
    for(double angle=0; angle<360;angle+=10)
    {
      for(int i = 0 ;i<2;i++)
      {
        ReadHeading();
        
        double ax=80*cos(angle* PI / 180);
        double ay=80*sin(angle* PI / 180);
  
        double wz = Heading*0.4;
    
        int f1=(int)(0.58*ax-0.33*ay + wz);
        int f2=(int)(-0.58*ax-0.33*ay + wz);
        int f3=(int)(0 + 0.67*ay + wz);
       
        String s = "  " + String((int)angle);
        drawText(0, 230, s.substring(s.length()-3,s.length()) , COLOR_WHITE);    
        s = "  " + String(f1);
        drawText(0, 260, s.substring(s.length()-3,s.length()) , COLOR_WHITE); 
    
        s = "  " + String(f2);
        drawText(0, 280, s.substring(s.length()-3,s.length()) , COLOR_WHITE); 
        s = "  " + String(f3);
        drawText(0, 300, s.substring(s.length()-3,s.length()) , COLOR_WHITE); 
    
        move(f1, f3, f2);
        
        delay(10);
      }
    }
  }
  
  //move(0, 0, 0);
  
  while(KeyEnter) ;
}

void CommunicationTest()
{
  while(!KeyEnter)
  {
    if(Serial1.availableForWrite())
    {
      char S[] = {(char)0xAA, (char)0x01};
      S[2] = (char)((0x1000 - (0xAA + 0x01)) & 0xFF);
      Serial1.write(S);   //1번 카메라에게 데이터 요청
      
      delay(100);
      
      unsigned char Ss[200];
      int l = 0;
      while(Serial1.available())    //카메라의 응답 데이터
      {
        Ss[l] = Serial1.read();
        l++;
      }
      int startpoint = 0;
      for(int i = 0; i < l-9; i ++)
      {
        if(Ss[i] == 0xAA)  startpoint = i;
      }
      
      if(l-startpoint >= 9 ) // 데이터가 들어왔음
      {
        int chksum = 0;
        for(int i = startpoint; i < startpoint + 9 ;i++)
        {
          chksum += Ss[i];
          Serial.print(Ss[i],HEX);
        }
        Serial.println("");
        
        if((chksum & 0xFF) == 0) //정상적인 데이터
        {
          
          String s = "     " + String((int)(Ss[startpoint + 2]));
          drawText(0, 60, s.substring(s.length()-6,s.length()) , COLOR_WHITE);   

          int x = Ss[startpoint + 2];
          if (x == 0)       move(0,0,0);
          else if(x < 65)   move(60,0,-10);
          else if (x > 94)  move(10,0,-60);
          else              move(100,0,-100);
        }
        else  //데이터는 들어왔지만 체크섬이 안맞음
        {
          move(0,0,0);
          drawText(0, 60, "CS ERR" , COLOR_WHITE);   
        }
      }
      else  //테이터가 안들어 옴
      {
        move(0,0,0);
        drawText(0, 60, "No Rep" , COLOR_WHITE);   
      }
    }
    delay(1);
  }
  while(KeyEnter)  ;
}

void SR04Test(void)
{
  while(true)
  {
    DisplayStatus();
    
    TRIGER2ON;
    delayMicroseconds(5);
    TRIGER2OFF;
    TRIGER2ON;
    delayMicroseconds(10);
    TRIGER2OFF;

    double duration = pulseIn (32, HIGH);
    int cm = int(duration / 58.8);

    String s = "    " + String(cm);
    drawText(0, 48, s.substring(s.length()-5,s.length()) , COLOR_WHITE);    

    //SHOOTERON;
    delay(100);
    //SHOOTEROFF;

    delay(1);
  }
  while(KeyEnter)  ;
}
