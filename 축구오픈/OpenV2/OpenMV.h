/*
 * OpenMV.h
 *
 * History
 *
 * 2024-04-30
 * 
 * 카메라 정보 수집
 *
 */ 

//--------------------------------------------------------------------
// PACKET 구조 (총 14 바이트)
//--------------------------------------------------------------------
// 1. PACKET HEADER      : unsigned char 1 BYTE (0xAA)
// 2. X of YELLOW GOAL   : signed int 2 BYTE (LITTLE ENDIAN 형식)
// 3. Y of YELLOW GOAL   : signed int 2 BYTE (LITTLE ENDIAN 형식)
// 4. X of BLUE GOAL     : signed int 2 BYTE (LITTLE ENDIAN 형식)
// 5. Y of BLUE GOAL     : signed int 2 BYTE (LITTLE ENDIAN 형식)
// 6. X of BALL          : signed int 2 BYTE (LITTLE ENDIAN 형식)
// 7. Y of BALL          : signed int 2 BYTE (LITTLE ENDIAN 형식)
// 8. CHECK SUM          : unsigned char 1 BYTE (EXCLUSIVE OR 방식)
//--------------------------------------------------------------------

#define X_offset 18
//volatile int ball_dir = 14;
//volatile int Ball_X = 0;
//volatile int Ball_Y = 0;
//volatile  unsigned char command[2] = {0xAA, 0x55};  // = {0,0,0,0};
//volatile  double startms;
//volatile  double timeout;

#define MaxLostCount 5

struct ObjectInfo
{
  bool      found = false;
  
  int       x = 0;
  int       y = 0;
  int angle = 0;
  uint16_t  dir = 14;  
  uint16_t  dist = 0;
  int       LostCount = 0;
};

volatile ObjectInfo OBJ[3]; //0:Yellow Goal, 1:Blue Goal, 2:BALL

volatile unsigned char MyGoal = 0;
volatile unsigned char TgtGoal = 1;
volatile unsigned char BALL = 2;

void find_success(char i)
{
  OBJ[i].found = true;
  OBJ[i].LostCount = 0;
  
  OBJ[i].dist = 
      (int)sqrt((double)((double)(OBJ[i].x * OBJ[i].x) + (double)(OBJ[i].y * OBJ[i].y)));

  //double ang = 2*PI  - atan2(-OBJ[i].x, OBJ[i].y);
  double ang = PI/2  - atan2(OBJ[i].y, OBJ[i].x);

  ang = ang * 180 / PI; 
  
  //ang = ang + compass - 180;
  //if(ang < 0)     ang += 360;          
  //if(ang >= 360)  ang -= 360;

  
  OBJ[i].angle = (int)(ang); 

  ang -= 15;  // 방향 보정 [ 0시 방향 : -15도 ~ +15도 ]
  if(ang < 0)     ang += 360;
  if(ang >= 360)  ang -= 360;
  OBJ[i].dir = int(ang/30) & 0x0F;             
}

void find_fail(char i)
{
  OBJ[i].LostCount++;
  if(OBJ[i].LostCount > MaxLostCount)
  {
    OBJ[i].found = false;
    OBJ[i].dir = 14;
    OBJ[i].dist = 0;
    OBJ[i].angle = 0;
    OBJ[i].LostCount = MaxLostCount;
  }
}

char ReadCAM(unsigned char mode)
{
  unsigned char rcv[200];
  char ErrCode = 0;
  
  OBJ[0].x = 0;
  OBJ[0].y = 0;
  OBJ[0].found = false;
  OBJ[1].x = 0;
  OBJ[1].y = 0;
  OBJ[1].found = false;
  OBJ[2].x = 0;
  OBJ[2].y = 0;
  OBJ[2].found = false;
  
  if(Serial3.availableForWrite())
  {
    Serial3.write(0xAA);   //카메라에게 데이터 요청
    Serial3.write(mode);   //찾기 모드 
        
    double startms = millis();
    double timeout = 100;
    
    while(Serial3.available()<14 && millis() - startms < timeout)  ;

    if(millis() - startms >= timeout)  return 5; //TIME OUT
    
    int l = 0;
    while(Serial3.available())    //카메라의 응답 데이터
    {
      rcv[l++] = Serial3.read();
      //l++;
    }
    
    if(l >= 14)
    {
      if(rcv[0] == 0xAA)
      {
        unsigned char chksum = 0xFF;
        for(int i = 0 ; i < 14; i++) chksum ^= rcv[i];
     
        if(chksum == 0) //정상적인 데이터
        {
          // OpenMV에서 촬영한 이미지를 반시계방향으로 90도 회전시키고
          // 좌우 대칭시켜야 함. (X좌표를 Y좌표로, -Y좌표를 X좌표로 변환)
          OBJ[0].y = rcv[2]<<8 | rcv[1];
          OBJ[0].x = (rcv[4]<<8 | rcv[3]) * -1;
          OBJ[1].y = rcv[6]<<8 | rcv[5];
          OBJ[1].x = (rcv[8]<<8 | rcv[7]) * -1;
          OBJ[2].y = rcv[10]<<8 | rcv[9];
          OBJ[2].x = (rcv[12]<<8 | rcv[11]) * -1;
          
          if(OBJ[BALL].x == 0 && OBJ[0].y == 0)  find_fail(0);
          else  find_success(0);
                    
          if(OBJ[1].x == 0 && OBJ[1].y == 0)  find_fail(1);
          else  find_success(1);
          
          if(OBJ[2].x == 0 && OBJ[2].y == 0)  find_fail(2);
          else  find_success(2);

//          ball_dir = OBJ[BALL].dir;
          
          return 0; // OKAY.
        }
        else  //데이터는 들어왔지만 체크섬이 안맞음
        {
          ErrCode = 1; //CHECKSUM Error
        }
      }
      else  //구조가 안맞음
      {
        ErrCode = 2;   // No Start Of Packet
      }
    }
    else  //테이터가 안들어 옴
    {      
      ErrCode = 3;   //NO Response
    }
  }
  
  ErrCode = 4; // UART Access Error
  
  return ErrCode;
}
