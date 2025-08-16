/*
 * PROGRAM.h
 *
 * History
 *
 * 2023-07-04
 * 
 *
 */ 
#include "ESCAPE.c"

volatile bool oldDir = false;
volatile int Ball[2]={0,0};
volatile int OurGoal[2]={0,0};
volatile int TargetGoal[2]={0,0};
volatile int LeftDist = 0;
volatile int RightDist = 0;

volatile  int OurX = 0;
volatile  int OurY = 0;
volatile  int TargetX = 0;
volatile  int TargetY = 0;

void SetToFront()
{
  ReadHeading();  //컴파스 정보 읽기

  double starttime = millis();
  while(!(Heading > -5 && Heading < 5) && (millis() - starttime < 1000))
  {
    ReadHeading();  //컴파스 정보 읽기
    
    if(Heading > 0) move(15,15,15);
    else            move(-15,-15,-15);
  }
  move(0,0,0);
}

void FindBall(void)
{
  if(oldDir)  move(20,20,20);
  else        move(-20,-20,-20);
  
  while (Ball_X == -60 + X_offset && Ball_Y == 0)
  {
//    ReadInfo();
    if(PINK & 0x20) break;
  }
  motor_stop();
}


//----------------------------------------------------------------------------------------
void dir_move_max(double dir)
{  
  dir_move(dir, 100);
  return;

  ReadHeading();
  
  double W_gain = 0.2;
  
  double Alpha = radians(dir*30);
  
  double ax = 100 * cos(Alpha);
  double ay = 100 * sin(Alpha);


  double Wz = W_gain * Heading; //(Heading + glower);

  if(Wz < -5) Wz=-5;
  if(Wz > 5) Wz = 5;
  

  double F1 = 0.5784 * ax - 0.3333 * ay + Wz;
  double F2 = -0.5784 * ax - 0.3333 * ay + Wz;
  double F3 = 0 * ax + 0.6666* ay + Wz;

  double f1 = abs(F1);
  double f2 = abs(F2);
  double f3 = abs(F3);

  double Fmax = f1;
  if(f2 > Fmax) Fmax = f2;
  if(f3 > Fmax) Fmax = f3;

  if(Fmax != 0)
  {
    F1 = F1 * 80 / Fmax;
    F2 = F2 * 80 / Fmax;
    F3 = F3 * 80 / Fmax;
  }
 
  move(F1, F3, F2);
}

void dir_move(double dir, double Velocity)
{
  ReadHeading();
      
  Velocity = Velocity * 1.35;
  if(Velocity > 135)      Velocity = 135;
  if(dir == 0)
  {
    if(ADC_Value[6] < 20 && ultra[0] < 25) Velocity = Velocity * 0.5;
  }
  else if(dir == 1 || dir == 2)
  {
    if(ultra[0] < 20 || ultra[1] < 15) Velocity = 0;
    else if(ultra[0] < 25 || ultra[1] < 25) Velocity = Velocity * 0.6;
  }
  else if(dir == 3)
  {
    if(ultra[1] < 15) Velocity = 0;
    else if(ultra[1] < 25) Velocity = Velocity * 0.5;
  }
  else if(dir == 4 || dir == 5)
  {
    if(ultra[1] < 15 || ultra[2] < 20) Velocity = 0;
    else if(ultra[1] < 25 || ultra[2] < 25) Velocity = Velocity * 0.6;
  }
  else if(dir == 6)
  {
    if(ultra[2] < 20) Velocity = 0;
    else if(ultra[2] < 25) Velocity = Velocity * 0.5;
  }
  else if(dir == 7 || dir == 8)
  {
    if(ultra[2] < 20 || ultra[3] < 15) Velocity = 0;
    else if(ultra[2] < 25 || ultra[3] < 25) Velocity = Velocity * 0.6;
  }
  else if(dir == 9)
  {
    if(ultra[3] < 15) Velocity = 0;
    else if(ultra[3] < 25) Velocity = Velocity * 0.5;
  }
  else if(dir == 10 || dir == 11)
  {
    if(ultra[0] < 20 || ultra[3] < 15) Velocity = 0;
    else if(ultra[0] < 25 || ultra[3] < 25) Velocity = Velocity * 0.6;
  }
  
  if(Velocity == 0)
  {
    SetToFront();
  }
  else
  {
    double W_gain = 0.2;
    
    double Alpha = radians(dir*30);
    
    double ax = Velocity * cos(Alpha);
    double ay = Velocity * sin(Alpha);


/*
    if(ax>0)
    {
      if(OBJ[TgtGoal].y < 30) ax =0;
    }
    else
    {
      if(OBJ[MyGoal].y > -30) ax = 0;
    }
    if (ay>0)
    {
      if(OBJ[TgtGoal].x < -20) ay = 0;
    }
    else
    {
      if(OBJ[TgtGoal].x >  20) ay = 0;
    }
*/
    double Wz = W_gain * Heading; //(Heading + glower);

    if(Wz < -0) Wz=-10;
    if(Wz > 10) Wz = 10;
    
  
    double F1 = 0.5784 * ax - 0.3333 * ay + Wz;
    double F2 = -0.5784 * ax - 0.3333 * ay + Wz;
    double F3 = 0 * ax + 0.6666* ay + Wz;

    double f1 = abs(F1);
    double f2 = abs(F2);
    double f3 = abs(F3);

    double Fmax = f1;
    if(f2 > Fmax) Fmax = f2;
    if(f3 > Fmax) Fmax = f3;

    if(Fmax != 0 && Fmax > 100)
    {
      F1 = F1 * 100 / Fmax;
      F2 = F2 * 100 / Fmax;
      F3 = F3 * 100 / Fmax;
    }
   
    move(F1, F3, F2);
  }
}

void attack_near_ball(int BallDirecrion)
{
  if(BallDirecrion == 0)        dir_move(0,70);
  else if (BallDirecrion == 1)  dir_move(3, 70);
  else if (BallDirecrion == 2)  dir_move(4, NormalSpeed);
  else if (BallDirecrion == 3)  dir_move(5, NormalSpeed);
  else if (BallDirecrion == 4)  dir_move(6, NormalSpeed);
  else if (BallDirecrion == 5)  dir_move(7, NormalSpeed);
  else if (BallDirecrion == 6)
    if (ultra[1] > ultra[3])    dir_move(5, NormalSpeed);
    else                        dir_move(7, NormalSpeed);
  else if (BallDirecrion == 7)  dir_move(5, NormalSpeed);
  else if (BallDirecrion == 8)  dir_move(6, NormalSpeed);
  else if (BallDirecrion == 9)  dir_move(7, NormalSpeed);
  else if (BallDirecrion == 10) dir_move(8, NormalSpeed);
  else if (BallDirecrion == 11) dir_move(9, 70);
//  else                          dir_move(0,0);
}

void attack_far_ball(int BallDirecrion)
{

  if (BallDirecrion == 0)       dir_move_max(0);
  else if (BallDirecrion == 1)  dir_move_max(2);
  else if (BallDirecrion == 2)  dir_move_max(3);
  else if (BallDirecrion == 3)  dir_move_max(5);
  else if (BallDirecrion == 4)  dir_move_max(5);
  else if (BallDirecrion == 5)  dir_move_max(6);
  else if (BallDirecrion == 6)
    if (ultra[1] > ultra[3])    dir_move_max(5);
    else                        dir_move_max(7);
  else if (BallDirecrion == 7)  dir_move_max(6);
  else if (BallDirecrion == 8)  dir_move_max(7);
  else if (BallDirecrion == 9)  dir_move_max(7);
  else if (BallDirecrion == 10) dir_move_max(9);
  else if (BallDirecrion == 11) dir_move_max(10);
//  else                          dir_move(0,0);

}

void delay200(void)
{
  dir_move(0, 0);
  delay(200);
}

void BallFinding()
{
  int cnt = 0;

  dir_move(0,0);
  return;
  
  if(ultra[1] > ultra[3]) dir_move(3,70);
  else                    dir_move(9,70);

  while(ball_dir > 12 && cnt < 25 && !LineDetected)
  {
      ReadHeading();
    cnt++;
  }
}

void Shooting()
{
  int cnt = 0;
  double ang = OBJ[TgtGoal].angle;
  if(ang > 180) ang -= 360;
  glower = ang;

  //MOTORD(100);
  double start = millis();
  while(!LineDetected && (millis()-start < 1000))
  {
    ReadHeading();
    dir_move(0,100);
    delay(1);
  }
  SHOOT(400);
  if(LineDetected)  delay200();
  else              dir_move(0,20);
  MOTORD(0);
  
  glower = 0;
}

void Attack(void) // 공격 //##############################
{
  move(100, 0, -100);
  Line = 0;
  LineDetected = false;
  
  delay(400);
  delay200();
  
  while (1)
  {
    ReadHeading();
    
    if(ReadCAM(3)==0)
    {
      if(OBJ[2].found)
      {
        //if(LineDetected)            EscapeLine(); // 외곽선을 밟았을때
        if(ADC_Value[12] >= 60) Shooting(); // 슈팅
        else if (OBJ[2].dist < 100) attack_near_ball(ball_dir); //공이 가까이 있을때
        else                        attack_far_ball(ball_dir);  //공이 멀리 있을때
      }
      else
      {
        move(0,0,0);
        delay(200);
        //else  BallFinding();    // 이도 저도 아니면 공을 찾는다.
      }
      delay(1);
    }
    else
    {
      move(0,0,0);
      delay(200);
    }
    //String s = "   " + String(Heading);
    //drawText(100, 60, s.substring(s.length()-4,s.length()) , COLOR_YELLOW);    
    //s = "   " + String(Line);
    //drawText(100, 80, s.substring(s.length()-4,s.length()) , COLOR_YELLOW);    

  }
}

void Defence(void)
{
  Attack();
}
