/*
 * ESCAPE.h
 *
 * History
 *
 * 2022-05-06
 * 
 *  외곽선에서 회피
 *  

 *  선감지
 *  LineDetected
 *  경계라인 감지 기억 값 (밟은적이 있다).
          //0 Bit : 0시방향 감지
          //1 Bit : 3시방향 감지
          //2 Bit : 6시방향 감지
          //3 Bit : 9시방향 감지
 *  선을 감지하고 그에따른 액션을 수행한후에는 값을 Clear 시켜주세요. (LineDetected = 0)
*/ 
int LowSpeed = 25;
int NormalSpeed = 85;
int HighSpeed = 95;

#define nearWALL 50
#define farWALL 80
#define lowPower 60
#define highPower 90

#define frontWALL (ultra[0] < nearWALL)
#define frontNotWALL (ultra[0] > farWALL && ultra[0] < 255)
#define rightWALL (ultra[1] < nearWALL)
#define rightNotWALL (ultra[1] > farWALL && ultra[1] < 255)
#define rearWALL (ultra[2] < nearWALL)
#define rearNotWALL (ultra[2] > farWALL && ultra[2] < 255)
#define leftWALL (ultra[3] < nearWALL)
#define leftNotWALL (ultra[3] > farWALL && ultra[3] < 255)

void SetToFront();
//void ReadInfo();
void ReadHeading();
void SR04Read();
void EscapeLine2();

void Escape(double dir, double power)
{
  double start = millis();
  
  while(millis() - start < 500)
  {
    dir_move(dir*30, power);
    delay(10);
  }
  motor_stop();
  
  Line = 0;
  LineDetected = false;

}

void atFrontLine()  //앞쪽선을 밟음 (앞쪽에 벽이 있음)
{
  if(leftWALL)
    if(rightWALL)
      if(rearWALL)          SetToFront();    //앞:벽, 좌:벽, 우:벽, 뒤:벽
      else if(rearNotWALL)  Escape(6, lowPower);  //앞:벽, 좌:벽, 우:벽, 뒤:벽아님
      else                  Escape(6, lowPower);  //앞:벽, 좌:벽, 우:벽, 뒤:모르겠음
    else if(rightNotWALL)
      if(rearWALL)          Escape(3, lowPower);  //앞:벽, 좌:벽, 우:벽아님, 뒤:벽
      else if(rearNotWALL)  Escape(4.5, highPower);  //앞:벽, 좌:벽, 우:벽아님, 뒤:벽아님
      else                  Escape(4, highPower);  //앞:벽, 좌:벽, 우:벽아님, 뒤:모르겠음
    else
      if(rearWALL)          Escape(3, lowPower);  //앞:벽, 좌:벽, 우:모르겠음, 뒤:벽
      else if(rearNotWALL)  Escape(5, highPower);  //앞:벽, 좌:벽, 우:모르겠음, 뒤:벽아님
      else                  Escape(4.5, highPower);  //앞:벽, 좌:벽, 우:모르겠음, 뒤:모르겠음
  else if(leftNotWALL)
    if(rightWALL)
      if(rearWALL)          Escape(9, lowPower);  //앞:벽, 좌:벽아님, 우:벽, 뒤:벽
      else if(rearNotWALL)  Escape(7.5, highPower);  //앞:벽, 좌:벽아님, 우:벽, 뒤:벽아님
      else                  Escape(8, highPower);  //앞:벽, 좌:벽아님, 우:벽, 뒤:모르겠음
    else if(rightNotWALL)
      if(rearWALL)          Escape(3, lowPower);    //앞:벽, 좌:벽아님, 우:벽아님, 뒤:벽
      else if(rearNotWALL)  Escape(6, highPower);  //앞:벽, 좌:벽아님, 우:벽아님, 뒤:벽아님
      else                  Escape(6, lowPower);  //앞:벽, 좌:벽아님, 우:벽아님, 뒤:모르겠음
    else
      if(rearWALL)          Escape(9, lowPower);  //앞:벽, 좌:벽아님, 우:모르겠음, 뒤:벽
      else if(rearNotWALL)  Escape(7, highPower);  //앞:벽, 좌:벽아님, 우:모르겠음, 뒤:벽아님
      else                  Escape(8, highPower);  //앞:벽, 좌:벽아님, 우:모르겠음, 뒤:모르겠음
  else
    if(rightWALL)
      if(rearWALL)          Escape(9, lowPower);  //앞:벽, 좌:모르겠음, 우:벽, 뒤:벽
      else if(rearNotWALL)  Escape(7, highPower);  //앞:벽, 좌:모르겠음, 우:벽, 뒤:벽아님
      else                  Escape(7.5, highPower);  //앞:벽, 좌:모르겠음, 우:벽, 뒤:모르겠음
    else if(rightNotWALL)
      if(rearWALL)          Escape(3, lowPower);  //앞:벽, 좌:모르겠음, 우:벽아님, 뒤:벽
      else if(rearNotWALL)  Escape(5, highPower);  //앞:벽, 좌:모르겠음, 우:벽아님, 뒤:벽아님
      else                  Escape(4, highPower);  //앞:벽, 좌:모르겠음, 우:벽아님, 뒤:모르겠음
    else
      if(rearWALL)          Escape(3, lowPower);      //앞:벽, 좌:모르겠음, 우:모르겠음, 뒤:벽
      else if(rearNotWALL)  Escape(6, highPower);  //앞:벽, 좌:모르겠음, 우:모르겠음, 뒤:벽아님
      else                  Escape(6, lowPower);      //앞:벽, 좌:모르겠음, 우:모르겠음, 뒤:모르겠음
}

void atRearLine()  //뒤쪽선을 밟음 (뒤쪽에 벽이 있음)
{
  if(leftWALL)
    if(rightWALL)
      if(frontWALL)         SetToFront();      //뒤:벽, 좌:벽, 우:벽, 앞:벽
      else if(frontNotWALL) Escape(0, lowPower);  //뒤:벽, 좌:벽, 우:벽, 앞:벽아님
      else                  Escape(0, lowPower);  //뒤:벽, 좌:벽, 우:벽, 앞:모르겠음
    else if(rightNotWALL)
      if(frontWALL)         Escape(3, lowPower);  //뒤:벽, 좌:벽, 우:벽아님, 앞:벽
      else if(frontNotWALL) Escape(1.5, highPower);  //뒤:벽, 좌:벽, 우:벽아님, 앞:벽아님
      else                  Escape(2, highPower);  //뒤:벽, 좌:벽, 우:벽아님, 앞:모르겠음
    else
      if(frontWALL)         Escape(3,lowPower);      //뒤:벽, 좌:벽, 우:모르겠음, 앞:벽
      else if(frontNotWALL) Escape(1, highPower);  //뒤:벽, 좌:벽, 우:모르겠음, 앞:벽아님
      else                  Escape(1.5,highPower);      //뒤:벽, 좌:벽, 우:모르겠음, 앞:모르겠음
  else if(leftNotWALL)
    if(rightWALL)
      if(frontWALL)         Escape(9, lowPower);  //뒤:벽, 좌:벽아님, 우:벽, 앞:벽
      else if(frontNotWALL) Escape(10.5, highPower);  //뒤:벽, 좌:벽아님, 우:벽, 앞:벽아님
      else                  Escape(10, highPower);  //뒤:벽, 좌:벽아님, 우:벽, 앞:모르겠음
    else if(rightNotWALL)
      if(frontWALL)         Escape(3,lowPower);      //뒤:벽, 좌:벽아님, 우:벽아님, 앞:벽
      else if(frontNotWALL) Escape(0, highPower);  //뒤:벽, 좌:벽아님, 우:벽아님, 앞:벽아님
      else                  Escape(0,lowPower);      //뒤:벽, 좌:벽아님, 우:벽아님, 앞:모르겠음
    else
      if(frontWALL)         Escape(9, lowPower);  //뒤:벽, 좌:벽아님, 우:모르겠음, 앞:벽
      else if(frontNotWALL) Escape(11, highPower);  //뒤:벽, 좌:벽아님, 우:모르겠음, 앞:벽아님
      else                  Escape(10, highPower);  //뒤:벽, 좌:벽아님, 우:모르겠음, 앞:모르겠음
  else
    if(rightWALL)
      if(frontWALL)         Escape(9,lowPower);      //뒤:벽, 좌:모르겠음, 우:벽, 앞:벽
      else if(frontNotWALL) Escape(11, highPower);  //뒤:벽, 좌:모르겠음, 우:벽, 앞:벽아님
      else                  Escape(10.5,highPower);      //뒤:벽, 좌:모르겠음, 우:벽, 앞:모르겠음
    else if(rightNotWALL)
      if(frontWALL)         Escape(3, lowPower);  //뒤:벽, 좌:모르겠음, 우:벽아님, 앞:벽
      else if(frontNotWALL) Escape(1, highPower);  //뒤:벽, 좌:모르겠음, 우:벽아님, 앞:벽아님
      else                  Escape(2, highPower);  //뒤:벽, 좌:모르겠음, 우:벽아님, 앞:모르겠음
    else
      if(frontWALL)         Escape(3,lowPower);      //뒤:벽, 좌:모르겠음, 우:모르겠음, 앞:벽
      else if(frontNotWALL) Escape(0,highPower);  //뒤:벽, 좌:모르겠음, 우:모르겠음, 앞:벽아님
      else                  Escape(0,lowPower);      //뒤:벽, 좌:모르겠음, 우:모르겠음, 앞:모르겠음
}

void atLeftLine()  //좌측선을 밟음 (좌측에 벽이 있음)
{
  if(rightWALL)
    if(frontWALL)
      if(rearWALL)          SetToFront();      //좌:벽, 우:벽, 앞:벽, 뒤:벽
      else if(rearNotWALL)  Escape(6, lowPower);  //좌:벽, 우:벽, 앞:벽, 뒤:벽아님
      else                  Escape(6,lowPower);      //좌:벽, 우:벽, 앞:벽, 뒤:모르겠음
    else if(frontNotWALL)
      if(rearWALL)          Escape(0, lowPower);  //좌:벽, 우:벽, 앞:벽아님, 뒤:벽
      else if(rearNotWALL)  Escape(6, lowPower);  //좌:벽, 우:벽, 앞:벽아님, 뒤:벽아님
      else                  Escape(0, lowPower);  //좌:벽, 우:벽, 앞:벽아님, 뒤:모르겠음
    else
      if(rearWALL)          Escape(0,lowPower);      //좌:벽, 우:벽, 앞:모르겠음, 뒤:벽
      else if(rearNotWALL)  Escape(6, lowPower);  //좌:벽, 우:벽, 앞:모르겠음, 뒤:벽아님
      else                  Escape(6,lowPower);      //좌:벽, 우:벽, 앞:모르겠음, 뒤:모르겠음
  else if(rightNotWALL)
    if(frontWALL)
      if(rearWALL)          Escape(3, lowPower);  //좌:벽, 우:벽아님, 앞:벽, 뒤:벽
      else if(rearNotWALL)  Escape(4.5, highPower);  //좌:벽, 우:벽아님, 앞:벽, 뒤:벽아님
      else                  Escape(4, highPower);  //좌:벽, 우:벽아님, 앞:벽, 뒤:모르겠음
    else if(frontNotWALL)
      if(rearWALL)          Escape(1.5,highPower);      //좌:벽, 우:벽아님, 앞:벽아님, 뒤:벽
      else if(rearNotWALL)  Escape(3, highPower);  //좌:벽, 우:벽아님, 앞:벽아님, 뒤:벽아님
      else                  Escape(2,highPower);      //좌:벽, 우:벽아님, 앞:벽아님, 뒤:모르겠음
    else
      if(rearWALL)          Escape(2, highPower);  //좌:벽, 우:벽아님, 앞:모르겠음, 뒤:벽
      else if(rearNotWALL)  Escape(4, highPower);  //좌:벽, 우:벽아님, 앞:모르겠음, 뒤:벽아님
      else                  Escape(3, highPower);  //좌:벽, 우:벽아님, 앞:모르겠음, 뒤:모르겠음
  else
    if(frontWALL)
      if(rearWALL)          Escape (3,lowPower);      //좌:벽, 우:모르겠음, 앞:벽, 뒤:벽
      else if(rearNotWALL)  Escape(5, highPower);  //좌:벽, 우:모르겠음, 앞:벽, 뒤:벽아님
      else                  Escape(4.5,highPower);      //좌:벽, 우:모르겠음, 앞:벽, 뒤:모르겠음
    else if(frontNotWALL)
      if(rearWALL)          Escape(1, highPower);  //좌:벽, 우:모르겠음, 앞:벽아님, 뒤:벽
      else if(rearNotWALL)  Escape(3, lowPower);  //좌:벽, 우:모르겠음, 앞:벽아님, 뒤:벽아님
      else                  Escape(1, highPower);  //좌:벽, 우:모르겠음, 앞:벽아님, 뒤:모르겠음
    else
      if(rearWALL)          Escape(1.5,highPower);      //좌:벽, 우:모르겠음, 앞:모르겠음, 뒤:벽
      else if(rearNotWALL)  Escape(5, highPower);  //좌:벽, 우:모르겠음, 앞:모르겠음, 뒤:벽아님
      else                  Escape(3,lowPower);      //좌:벽, 우:모르겠음, 앞:모르겠음, 뒤:모르겠음
}

void atRightLine()  //우측선을 밟음 (우측에 벽이 있음)
{
  if(leftWALL)
    if(frontWALL)
      if(rearWALL)          SetToFront();      //우:벽, 좌:벽, 앞:벽, 뒤:벽
      else if(rearNotWALL)  Escape(6, lowPower);  //우:벽, 좌:벽, 앞:벽, 뒤:벽아님
      else                  Escape(6,lowPower);      //우:벽, 좌:벽, 앞:벽, 뒤:모르겠음
    else if(frontNotWALL)
      if(rearWALL)          Escape(0, lowPower);  //우:벽, 좌:벽, 앞:벽아님, 뒤:벽
      else if(rearNotWALL)  Escape(6, lowPower);  //우:벽, 좌:벽, 앞:벽아님, 뒤:벽아님
      else                  Escape(0, lowPower);  //우:벽, 좌:벽, 앞:벽아님, 뒤:모르겠음
    else
      if(rearWALL)          Escape(0,lowPower);      //우:벽, 좌:벽, 앞:모르겠음, 뒤:벽
      else if(rearNotWALL)  Escape(6, lowPower);  //우:벽, 좌:벽, 앞:모르겠음, 뒤:벽아님
      else                  Escape(6,lowPower);      //우:벽, 좌:벽, 앞:모르겠음, 뒤:모르겠음
  else if(leftNotWALL)
    if(frontWALL)
      if(rearWALL)          Escape(9, lowPower);  //우:벽, 좌:벽아님, 앞:벽, 뒤:벽
      else if(rearNotWALL)  Escape(7.5, highPower);  //우:벽, 좌:벽아님, 앞:벽, 뒤:벽아님
      else                  Escape(8, highPower);  //우:벽, 좌:벽아님, 앞:벽, 뒤:모르겠음
    else if(frontNotWALL)
      if(rearWALL)          Escape(10.5,highPower);      //우:벽, 좌:벽아님, 앞:벽아님, 뒤:벽
      else if(rearNotWALL)  Escape(9, highPower);  //우:벽, 좌:벽아님, 앞:벽아님, 뒤:벽아님
      else                  Escape(10,highPower);      //우:벽, 좌:벽아님, 앞:벽아님, 뒤:모르겠음
    else
      if(rearWALL)          Escape(10, highPower);  //우:벽, 좌:벽아님, 앞:모르겠음, 뒤:벽
      else if(rearNotWALL)  Escape(8, highPower);  //우:벽, 좌:벽아님, 앞:모르겠음, 뒤:벽아님
      else                  Escape(9, highPower);  //우:벽, 좌:벽아님, 앞:모르겠음, 뒤:모르겠음
  else
    if(frontWALL)
      if(rearWALL)          Escape(9,lowPower);      //우:벽, 좌:모르겠음, 앞:벽, 뒤:벽
      else if(rearNotWALL)  Escape(7, highPower);  //우:벽, 좌:모르겠음, 앞:벽, 뒤:벽아님
      else                  Escape(7.5,highPower);      //우:벽, 좌:모르겠음, 앞:벽, 뒤:모르겠음
    else if(frontNotWALL)
      if(rearWALL)          Escape(11, highPower);  //우:벽, 좌:모르겠음, 앞:벽아님, 뒤:벽
      else if(rearNotWALL)  Escape(9, lowPower);  //우:벽, 좌:모르겠음, 앞:벽아님, 뒤:벽아님
      else                  Escape(11, highPower);  //우:벽, 좌:모르겠음, 앞:벽아님, 뒤:모르겠음
    else
      if(rearWALL)          Escape(10.5,highPower);      //우:벽, 좌:모르겠음, 앞:모르겠음, 뒤:벽
      else if(rearNotWALL)  Escape(7, highPower);  //우:벽, 좌:모르겠음, 앞:모르겠음, 뒤:벽아님
      else                  Escape(9,lowPower);      //우:벽, 좌:모르겠음, 앞:모르겠음, 뒤:모르겠음
}

void EscapeWithCam()
{
  double xt, yt;
  
  if(OBJ[MyGoal].found && OBJ[TgtGoal].found)
  {
    xt = (OBJ[MyGoal].x + OBJ[TgtGoal].x)/2;
    yt = (OBJ[MyGoal].y + OBJ[TgtGoal].y)/2;
  }
  else if(OBJ[MyGoal].found)
  {
    xt = OBJ[MyGoal].x;
    yt = int(OBJ[MyGoal].y / 2);
    if(abs(xt) > abs(yt)) yt = abs(xt);
  }
  else if(OBJ[TgtGoal].found)
  {
    xt = OBJ[TgtGoal].x;
    yt = int(OBJ[TgtGoal].y / 2);
    if(abs(xt) > abs(yt)) yt = -abs(xt);    
  }
  else  return;
  
  double ang = PI/2 - atan2(yt, xt);
  //double ang = 2*PI - atan2(yt, xt);

  // Convert radians to degrees for readability.
  ang = ang * 180 / PI; 
  if(ang < 0) ang += 360;
  if(ang > 360) ang -= 360;

  double dir = ang /12;
  
  double startms = millis();
  while(millis() - startms < 600)
  {
    dir_move_max(dir);
    delay(10);
  }
  dir_move(0,0);
  delay(100);
  Line = 0;
  LineDetected = false;
}

void SetToFront();
void EscapeLine()
{
  motor_stop();
  SetToFront();
  SR04Read();
  ReadCAM(3);
  
  //공의 방향이 바뀔때까지 대기
  int OldBallDir = ball_dir;
  while(OldBallDir == ball_dir && ball_dir < 12)
  {
    ReadCAM(3);
    delay(10);
  }
  
  while(LineDetected)
  {
    if(ReadCAM(0) == 0)
    {
      EscapeWithCam();
    }
    else
    {
      //초음차센서를 이용해서 선빠져나오기
      if(frontWALL)       atFrontLine();  //앞쪽에 벽이 있을때
      else if(rearWALL)   atRearLine();   //뒤쪽에 벽이 있을때
      else if(leftWALL)   atLeftLine();   //좌측에 벽이 있을때
      else if(rightWALL)  atRightLine();   //우측에 벽이 있을때
      else dir_move(0,0);
//      {
//        int angle = MovingDir -180;
//        if(angle < 0) angle + 360;
//        if (angle > 360) angle -=360;
//        Escape(angle, lowPower);
//      }
        motor_stop();
    }
  }
  motor_stop();
  SetToFront();
//  ReadInfo();
  
  Line = 0;
  LineDetected = false;
}
