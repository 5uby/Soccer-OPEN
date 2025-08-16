/*
 * USERPROGRAM.h
 *
 * History
 *
 * 2024-04-30
 */

/*---------------------------------------------------------------------------
 * 프로 그램 작성에 필요한 정보들
 *---------------------------------------------------------------------------
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
 *    Line  : 현재 센서의 흰색선 감지 여부(지금 현재 상황)
 *    LineDetected : 흰색 선을 감지했었덕적이 있는지 (감지했던 이력)    
 *    LineDetected : LineLineDetected12의 값을 4비트로 변환 (Bit 0:정면, Bit 1: 우측, Bit 2: 후방, Bit 3:좌측)

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

/* Configuration for ball tracking */

#define FRAME_CENTER_X    320   // 해상도 가로 절반 (VGA 640×480 기준)
#define X_DEADZONE        40    // ±40px 이내면 중앙
#define STOP_DISTANCE     (NEARBALL + 5)  // 이 거리 이하로 오면 정지
#define ANGLE_DEADZONE     5    // ±5도 이내면 정면
#define DIST_DEADZONE     5           // ±5픽셀 이내면 거리 OK
#define integralMax      100
#define maxOutput         70

int ball_x, ball_y, ball_dir, ball_dist;

// 공 정보 초기화
void ball_clear() {
  ball_x    = OBJ[2].x;
  ball_y    = OBJ[2].y;
  ball_dir  = OBJ[2].dir;
  ball_dist = OBJ[2].dist;
}

// 공이 화면 중앙에 있는지
int isBallCentered() {
  return (OBJ[2].found && abs(ball_x - FRAME_CENTER_X) <= X_DEADZONE) ? 1 : 0;
}

// 공을 중앙에 맞추기 위해 회전
void CenterBall() {
    double angle = OBJ[2].angle;   // -CCW ~ +CW (도)
    double dist  = OBJ[2].dist;    // 거리

    // 1) 각도 정렬
    int dirIndex = (int)round(angle / 30.0);
    if (dirIndex < 0) dirIndex += 12;
    dirIndex %= 12;

    // 2) angle 이 데드존 벗어나면 해당 방향(시계방향 인덱스)으로 회전
    if (fabs(angle) > ANGLE_DEADZONE) {
        DirMove(dirIndex, 10);
    }
    // 3) 각도 정렬 완료 후, 거리 정렬: 12시 방향(0 인덱스)으로 전진/후진
    else {
        if (dist > NEARBALL + DIST_DEADZONE) {
            // 12시 방향으로 전진
            DirMove(0, 10);
        } else if (dist < NEARBALL - DIST_DEADZONE) {
            // 6시 방향으로 후진
            DirMove(6, 10);
        } else {
            //정지
            MOVE(0, 0, 0);
        }
    }
}

void MovingToXY(int x, int y, int Power) {
  double Theta = atan2(x, y) * 180.0 / PI;
  double dir   = Theta / 30.0;
  DirMove(dir, Power);
}

void AproachToBall() {
    double Xb    = OBJ[2].x;
    double Yb    = OBJ[2].y;
    double r     = NEARBALL;
    double d     = OBJ[2].dist;
    double angle = OBJ[2].angle;
    int power    = (int)((d - r) * 3);  
    if (power > 80) power = 80;        
    if (power < 30) power = 30;         

    if (d < r + 10) {
        if (abs(angle) < 10) {
            DirMove(angle / 30.0, 20);   
        } else if (abs(angle) < 40) {
            MovingToXY((int)Xb, (int)(Yb - 45), 30);  
        } else {
            double Alpha = atan2(-Yb, -Xb);
            double Beta  = (5.0 * PI / 180.0) * (Xb < 0 ? -1 : 1);
            double theta = Alpha + Beta;
            double Xt    = Xb + r * cos(theta);
            double Yt    = Yb + r * sin(theta);
            if (abs(angle) < 135) power = 80;
            MovingToXY((int)Xt, (int)Yt, power);
        }
    } else if (Yb >= 0) {
        MovingToXY((int)Xb, (int)(Yb - r), power);
    } else {
        double Alpha = atan2(Yb, Xb);
        double l     = sqrt(d * d - r * r);
        double Beta  = asin(r / d);
        double Theta = (Xb < 0) ? Alpha + Beta : Alpha - Beta;
        int Xt = (int)(l * cos(Theta));
        int Yt = (int)(l * sin(Theta));
        MovingToXY(Xt, Yt, 80);  
    }
}

int PIDByUltra_Ver(int target)
{
  // PID 게인 (초기값)
  //float Kp = 1;
  float Ki = 0.1;
  //float Kd = 1.2;0

  static float Ultra_error = 0;
  static float Ultra_lastError = 0;
  static float Ultra_integral = 0;

  int currentDis = UltraSensor[2];
  Ultra_error = target - currentDis;

  Ultra_integral += Ultra_error;
  if (Ultra_integral > integralMax) Ultra_integral = integralMax;
  if (Ultra_integral < -integralMax) Ultra_integral = -integralMax;

  float derivative = Ultra_error - Ultra_lastError; //
  if (Ultra_error * Ultra_lastError < 0) derivative = 0;

  float dynamicKp = 2.7 - 0.2 * exp(-0.1 * abs(Ultra_error)); // 거리에 따른 가변 kp
  float dynamicKd = 2.2 + 0.3 * exp(-0.1 * abs(Ultra_error)); // 거리에 따른 가변 kd
  if (dynamicKp > 5) dynamicKp = 5;
  if (dynamicKd > 5) dynamicKd = 5;

  // PID 계산
  float output = (dynamicKp * Ultra_error) + (Ki * Ultra_integral) + (dynamicKd * derivative);


  // 모터에 출력
  Ultra_lastError = Ultra_error;

  if (abs(Ultra_error) < 3 && abs(derivative) < 2) {
    Ultra_integral = 0; // 적분항 초기화
    return 0;
  }

  if (output > 0) output += 40; // 뒤로 밀렸을 때 바로 탈출

  if (output > maxOutput) output = maxOutput;
  if (output < -maxOutput) output = - maxOutput;

  return output;
}

int ultraTarget = 45;

int wall_left = UltraSensor[1];
int wall_right = UltraSensor[3];

void handlePos(void)
{
  if (abs(UltraSensor[2] - ultraTarget) >= 5) //수직으로 정렬해야 해
  {
    //    isPositioned = false;
    DirMove(0, PIDByUltra_Ver(ultraTarget));
  }
       //UltraSensor[0~3] : Bit 0:정면, Bit 1: 우측, Bit 2: 후방, Bit 3:좌측
  else
  {
    if (UltraSensor[1] < 73 ){
      DirMove(9, 60);
    }
    else if (UltraSensor[1] > 86 ){
      DirMove(3, 60);
    }
    /*if (OBJ[MyGoal].found == true)
    {
      if (abs(158 - goalPost_X) >= 20) // 수평으로 정렬해야 해
      {
        DirMove(3, PIDByGoalPost_X() );
      }
      else//정렬 끝
      {
        move(0, 0, 0);
      }
    }
    else//카메라 감지 안됨ㅜㅜ
    {
      if (UltraSensor[3] < 15) DirMove(3, 60);
      else if (UltraSensor[1] < 15) DirMove(9, 60);
      else
      {
        if (UltraSensor[1] > UltraSensor[3]) DirMove(3, 70);
        else DirMove(9, 70);
      }
    }
    */
  }
}

// 공격
void ATTACK() {
  ClearLineStatus();
  LCD.fillScreen(COLOR_BLACK);
  LCD.setTextSize(3);
  drawText(60,150,"ATTACK",COLOR_WHITE);

  while (1) {
    ball_clear();
    ReadHeading(); 
    ReadCAM(0);
    DirMove(OBJ[MyGoal].dir,80);
  }
}

int ut = UltraSensor[2];

int chk = 0;

int blst,balldist_save;

// 수비
void DEFENCE()
{
  ClearLineStatus();
  LCD.fillScreen(COLOR_BLACK);
  LCD.setTextSize(3);
  drawText(20, 20, "DEFENCE", COLOR_WHITE);
     //UltraSensor[0~3] : Bit 0:정면, Bit 1: 우측, Bit 2: 후방, Bit 3:좌측
  while (1) {
    if (UltraSensor[2] < 37){ //뒤
      DirMove(0, 90);
    }
    if (UltraSensor[1] < 19){ //오
      DirMove(9, 90);
    }
    else if (UltraSensor[3] < 23){ //왼
      DirMove(3, 90);
    }

    ball_clear();
    ReadCAM(0);  // 공 위치 업데이트
      if (OBJ[2].found == true) {
        /*
        if (blst = 0){
          balldist_save = OBJ[2].dist;
          for (int i = 0;i <= 3000;i++){
            ball_clear()
            delay(1);
            if (ball_dist < balldist_save -10 || ball_dist > balldist_save + 10){
              break;
              blst = 0;
            }
            blst = 1;
          }
          if (blst == 1){
            while (OBJ[2].dist > 90){
              DirMove(ball_dir,60);
            }
            DirMove(0,60);
            delay(100);
            SHOOT(100);
          }
        }
        
        /*if (OBJ[2].dist != 0 && OBJ[2].x < 25 && OBJ[2].x > -18){
          for (int i = 0;i <= 3000;i++){
            if (OBJ[2].x > 25 || OBJ[2].x < -18){
              break;
            }
          }
          if (OBJ[2].x > 25 || OBJ[2].x < -18){
            break;
          }
          for (int j = 0;j <= 100;j++){
            DirMove(OBJ[MyGoal].dir,80);
          }
          SHOOT(100);
        }*/
        
        if (OBJ[2].x > 50) {
          DirMove(3, 100);
        }
        else if (OBJ[2].x < -50) {
          DirMove(9, 100);
        }
        else {
          MOVE(0, 0, 0);
          SetToFront();
          /*
          for (int i = 0;i <= 3000;i++){
            if (OBJ[2].x > 50 || OBJ[2].x < -50){
              chk = 0;
              break;
            }
            chk = 1;
          }
          if (chk == 1){
            for (int j = 0;j <= 100;j++){
              DirMove(0,80);
            }
            SHOOT(100);
            chk = 0;
          }
          */
        }
      }
      else {
        handlePos();
      }
      ut = OBJ[2].x;
      LCD.setCursor(60, 180);
      LCD.print(ut);
      }
}