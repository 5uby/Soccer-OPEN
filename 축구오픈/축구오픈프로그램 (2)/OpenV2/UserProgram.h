/*
 * USERPROGRAM.h
 *
 * History
 *
 * 2024-04-30
 * 
 *
 */ 

/*---------------------------------------------------------------------------
 * 프로 그램 작성에 필요한 정보들
/*---------------------------------------------------------------------------
 * 1. 센서 데이터들의 수집
 *    자동으로 수집 : 바닥감지 센서, 정면 빛센서, 메인 전압, 모터 전원의 전압, 초음파 거리 측정값.
 *    호출에 의 한 수집 :
 *        로봇의 방향  : void ReadHeading(void)
 *        카메라 정보  : char ReadCAM(mode)
 *        
 * 2. 로봇의 방향     
 *    ReadHeading() 호출하여야 함. 
 *      Heading : -179 ~ 180도 (정면이 0도)
 *      Compass : 0 ~ 359도 (정면이 180도)
 *    
 * 3. 카메라 읽는 방법
 *    char Err = ReadCAM(0);        // 공, 노란색 골대, 파란색 골대를 모두 찾을때 (모드 0).
 *    char Err = ReadCAM(TgtGoal);  // 상대편 골대만 찾을때 (TgtGoal은 이미 정의된 변수임)
 *    char Err = ReadCAM(MyGoal);   // 우리편 골대만 찾을때 (MyGoal은 이미 정의된 변수임)
 *    char Err = ReadCAM(3);        // 공만 찾을때 (모드 3)
 *    
 *    리턴값 Err가 0 이면 데이터를 제대로 수신하였음.
 *    
 * 4. 공에 대한 정보   
 *    OBJ[2].found : true or false (공을 찾았는지 여부)
 *    OBJ[2].x     : X 좌표
 *    OBJ[2].y     : Y 좌표
 *    OBJ[2].angle : 공의 방향 (각도)
 *    OBJ[2].dir   : 공의 방향 (0시 ~ 11시 방향)
 *    OBJ[2].dist  : 공까지의 거리 상대값 (단순히 X제곱 + Y제곱 값의 제곱근임)
 * 5. 상대편 골대에 대한 정보   
 *    OBJ[TgtGoal].found : true or false (상대편 골대를 찾았는지 여부)
 *    OBJ[TgtGoal].x     : X 좌표
 *    OBJ[TgtGoal].y     : Y 좌표
 *    OBJ[TgtGoal].angle : 골대의 방향 (각도)
 *    OBJ[TgtGoal].dir   : 골대의 방향 (0시 ~ 11시 방향)
 *    OBJ[TgtGoal].dist  : 골대까지의 거리 상대값 (단순히 X제곱 + Y제곱 값의 제곱근임)
 * 6. 우리편 골대에 대한 정보   
 *    OBJ[MyGoal].found : true or false (우리편 골대를 찾았는지 여부)
 *    OBJ[MyGoal].x     : X 좌표
 *    OBJ[MyGoal].y     : Y 좌표
 *    OBJ[MyGoal].angle : 골대의 방향 (각도)
 *    OBJ[MyGoal].dir   : 골대의 방향 (0시 ~ 11시 방향)
 *    OBJ[MyGoal].dist  : 골대까지의 거리 상대값 (단순히 X제곱 + Y제곱 값의 제곱근임)
 *    
 * 7. 흰색 선에 대한 정보   (정면에서 시계방향으로 0번부터 11번까지 12개의 센서)
 *    Line12  : 현재 센서의 흰색선 감지 여부(지금 현재 상황)
 *    LineDetected12 : 흰색 선을 감지했었덕적이 있는지 (감지했던 이력)    
 *    LineLineDetected : LineLineDetected12의 값을 4비트로 변환 (Bit 0:정면, Bit 1: 우측, Bit 2: 후방, Bit 3:좌측)
 *    LineLineDetected : LineLineDetected12의 값을 4비트로 변환 (Bit 0:정면, Bit 1: 우측, Bit 2: 후방, Bit 3:좌측)
 *    
 * 8. 초음파 거리 값
 *    UltraSensor[0~3] : Bit 0:정면, Bit 1: 우측, Bit 2: 후방, Bit 3:좌측
 *    
 * 9. 캡쳐링존에 있는 전면을 향한 빛센서(공에 반사되는 값)
 *    LightSensor[12]
 *    
 * 10. 모터 구동 방법
 *    MOVE(A모터 POWER, B모터 POWER, C모터 POWER)   : POWER : -100 ~ 100
 *    
 *    개별동작
 *    MOTORA(POWER),MOTORB(POWER),MOTORC(POWER),MOTORD(POWER)
 *    
 * 11. 슈터 동작
 *    SHOOT(time)   : time ON 지속 타임 (최대 100ms)
 */


int ball_x,ball_y,ball_dir,ball_dist,move_power,oppgoal_dir,oppgoal_dist;
int shootornot=0;

void ball_clear() //공 정보 초기화
{
  ball_x = OBJ[2].x;
  ball_y = OBJ[2].y;
  ball_dir = OBJ[2].dir;
  ball_dist = OBJ[2].dist;
}

void ynb_clear() //골대 정보 초기화
{
  oppgoal_dir = OBJ[TgtGoal].dir;
  oppgoal_dist = OBJ[TgtGoal].dist;
}

void LCD_check() //LCD로 값 체크용 (defend모드 사용)
{
  LCD.print(oppgoal_dist);
  delay(100);
  LCD.setCursor(0,0);
  LCD.fillScreen(COLOR_BLACK);
}

void power_reset() //거리에 따른 파워값 조정
{
  if (ball_dist > 45 && ball_dist < 135 && ball_x > 45 && ball_x < 115 && ball_y > 45 && ball_y < 115){
    move_power = 30; //30
  }
  else{
    move_power = 60; //60
  }
}

void MovingToXY(int x, int y, int Power) {
  double Theta = atan2(x, y) * 180 / PI;  // 목표점과의 각도 (- CCW , CW +), RADIAN값을 각도로 변환.
  double dir = Theta / 30;

  DirMove(dir, Power);
}

void AproachToBall(void) {
  double Xb = OBJ[BALL].x;  // 공의 좌표
  double Yb = OBJ[BALL].y;
  double r = NEARBALL;             // 공을 끼고 접근할 원의 반지름
  double d = OBJ[BALL].dist;       // 공까지의 거리
  double angle = OBJ[BALL].angle;  // 공과의 각도(- CCW ~ CW +)

  int power = ((int)d - r) * 5;  // 거리에 따른 속도 조절(공근처에서 70까지 줄임)
  if (power > 100) power = 100;
  else if (power < 40) power = 40;

  if (d < r + 10)  // 공이 가까이 있으면 고을 끼고 돌기
  {
    if (abs(angle) < 10) DirMove(OBJ[BALL].angle / 30, 30);
    else if (abs(angle) < 40) MovingToXY(Xb, Yb - 45, 40);  //공이 앞쪽에 있으면 공의 뒤로 이동
                                                            //dir_move(angle/30, 50);
    else {
      double Alpha = atan2(-Yb, -Xb);
      double Beta = 5 * PI / 180;  // 공이 오른쪽에 있으면 반시계방향으로 돌기
      if (Xb < 0) Beta *= -1;      // 공이 왼쪽에 있으면 시계방향으로 돌기
      double theta = Alpha + Beta;

      double Xt = r * cos(theta);
      double Yt = r * sin(theta);

      Xt = Xb + Xt;
      Yt = Yb + Yt;

      if (abs(angle) < 135) power = 100;
      //else  power = abs(angle) - 160;

      MovingToXY(Xt, Yt, power);  //공이 앞쪽에 있으면 공의 뒤로 이동
    }
  } else if (Yb >= 0)  //공이 앞에 있을때
  {
    MovingToXY(Xb, Yb - r, power);  //공이 앞쪽에 있으면 공의 뒤로 이동
  } else                            // 공이 뒷쪽에 있을때
  {
    // 로봇을 중심 O를 원점 (0,0)으로 하고
    // 공의 좌표 B 는 (Xb, Yb)
    // 공을 중심으로 반지름이 r인 원을 그렸을 때
    // 공과의 각도 Alpha
    // 접근할 위치 T (Xt, Yt)
    // BT와 OT는 직각을 이루며
    // 공까지의 거리 d 는 루트(Xb*Xb + Yb*Yb)
    // T 점까지의 거리 l은 루트(d*d - r*r)
    // OB와 OT의 내각(90도보다 작은 양의 값)을 Beta라면
    // 목표로 하는 T점의 각도는 Theta는 Alpha + Beta (Alpha가 음의 값, Beta는 양의 값)
    // T점의 좌표는
    // Xt = l * cos(Theta)
    // Yt = l * sin(Theta)
    //
    double Alpha = atan2(Yb, Xb);  // 공까지의 각도(RADIAN)
    double l = sqrt(d * d - r * r);
    double Beta = asin(r / d);  // RT 선분과 OT선분이 직각을 이루는 한점(T) 공까지의 선분과 원상의 한점
    double Theta;
    if (Xb < 0) Theta = Alpha + Beta;
    else Theta = Alpha - Beta;

    int Xt = (int)(l * cos(Theta));
    int Yt = (int)(l * sin(Theta));
    MovingToXY(Xt, Yt, 100);
  }
}


void ATTACK(void)
{
  ClearLineStatus();
  LCD.fillScreen(COLOR_BLACK);
  LCD.setTextSize(3);
  drawText(60, 150, "ATTACK", COLOR_WHITE);

  /*
  <기본 구동>
  공을 찾으면 공을 공쪽으로 이동후 (슈터에 공이 향하도록) 그런뒤, 공대 방향으로 공을 몰다가 슛
  공을 찾지 못하면 점점 원 크기를 넓혀가면서 돌아서 공을 찾는다. 그런 뒤에 그 행동을 계속 반복한다.*/

  /*
  <슛>
  공위치는 x,y값 사용 그리고 공을 찾을때는 dir값 사용해서 공의 위치로 이동.
  골대 위치는 dist 사용 그리고 공을 모는 방향은 dir값 사용.
  그 다음 슛.*/

  /*
  <득점 후 동작>
  

  */

  while(1)
  {
    if (Line != 0){
      LineMove();
      delay(600);
    }
    else{
      if (OBJ[2].found == true){
        ReadCAM(0);
        LineMove();
        ball_clear();
        ynb_clear();
        power_reset();
        if (shootornot == 0){ //슛 전 후 구분
          ReadCAM(0);
          LineMove();
          ball_clear();
          ynb_clear();
          power_reset();
          if (ball_dist > 45 && ball_dist < 95 && ball_x > 45 && ball_x < 75 && ball_y > 45 && ball_y < 75){   //공이 로봇 앞에 있을떄
            if (oppgoal_dist > 115){ //상대 골대 쪽 공 몰기
              if (Line == 0){
                SHOOT(100);
                if (oppgoal_dir == 3){
                  DirMove(oppgoal_dir+2,100);
                }
                else if (oppgoal_dir == 9){
                  DirMove(oppgoal_dir-2,100);
                }
                else if (oppgoal_dir > 0 && oppgoal_dir < 6){
                  DirMove(oppgoal_dir+1,100);
                }
                else if (oppgoal_dir > 6 && oppgoal_dir < 12){
                  DirMove(oppgoal_dir-1,100);
                }
                else{
                  DirMove(ball_dir,move_power);
                }
              }
            }
            else if(ball_dist > 45 && ball_dist < 135 && ball_x > 45 && ball_x < 115 && ball_y > 45 && ball_y < 115){ //슛
              if (Line == 0){
                SHOOT(100);
                if (oppgoal_dir == 3){
                  DirMove(oppgoal_dir+2,100);
                }
                else if (oppgoal_dir == 9){
                  DirMove(oppgoal_dir-2,100);
                }
                else if (oppgoal_dir > 0 && oppgoal_dir < 6 && oppgoal_dir != 3 && oppgoal_dir != 9){
                  DirMove(oppgoal_dir+1,100);
                }
                else if (oppgoal_dir > 6 && oppgoal_dir < 12 && oppgoal_dir != 3 && oppgoal_dir != 9){
                  DirMove(oppgoal_dir-1,100);
                }
                else{
                  DirMove(ball_dir,move_power);
                }
              }
            }
            else{ //공쪽으로 이동
              AproachToBall();
            }
          }
        }
        else if (shootornot == 1){
          shootornot = 0;
        }
        }
        else if (OBJ[2].found == false){
          while(true){
            ReadCAM(0);
            LineMove();
            ball_clear();
            ynb_clear();
            power_reset();
            if (Line == 0){
              DirMove(ball_dir,60);
              delay(1);
            }
            if (OBJ[2].found == true){
              break;
            }
          }
        }
    }
  }
}



/*
 *  수비 프로그램
 */
void DEFENCE(void)
{

}