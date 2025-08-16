/*
 * TimerInterrupt.h
 *
 * History
 *
 * 2024-04-30
 * 
 *  타이머 인터럽트 루틴
 *  초음파 거리 측정
 *  아나로그 센서(빛센서, 전압)값 변환
 *
 */ 

void LedShift(void)
{
  if(AdcIndex == 0 || AdcIndex > 15 - TurnOnLeds)
  {
    PORTA |= 0x01;  //IR_SERIN HIGH
  }
  else
  {
    PORTA &= 0xFE;  //IR_SERIN LOW
  }
  PORTA |= 0x02;  //IR_SRCK HIGH
  PORTA &= 0xFD;  //IR_SRCK LOW
  PORTA |= 0x04;  //IR_RCK HIGH
  PORTA &= 0xFB;  //IR RCK LOW
}

void Read_ADC(void)
{
  /*
   * ADC Read Here
   */
  if(AdcIndex < 13)
  {
    LightSensor[AdcIndex]=ADCH;
    if(LightSensor[AdcIndex] > Threshold[AdcIndex]) 
    {
      Line12 |= (1 << AdcIndex);
      LineDetected12 |= (1 << AdcIndex);
    }
    else
    {
      Line12 &= ~(1 << AdcIndex);
    }
  }
  else if(AdcIndex == 13) MainVoltRAW = ADCH;
  else if(AdcIndex == 14) MotorVoltRAW = ADCH;

  Line = 0;
  if(Line12 & 0x0C03) Line |= 0x01;
  if(Line12 & 0x001C) Line |= 0x02;
  if(Line12 & 0x00F0) Line |= 0x04;
  if(Line12 & 0x0380) Line |= 0x08;

  if(LineDetected12 & 0x0C03) LineDetected |= 0x01;
  if(LineDetected12 & 0x001C) LineDetected12 |= 0x02;
  if(LineDetected12 & 0x00F0) LineDetected12 |= 0x04;
  if(LineDetected12 & 0x0380) LineDetected12 |= 0x08;
  
  AdcIndex++;
  if(AdcIndex > 14) AdcIndex = 0;
  
  LedShift();

  if(AdcIndex < 8)  ADCSRB &= 0xF7;
  else ADCSRB |= 0x08; 
  
  ADMUX = 0x60 | (AdcIndex & 0x07);
  ADCSRA = ADCSRA | 0xC0; //ADC START & ADC Interrupt disable
}

void US_Check(void)
{
  if (Pulse_Width_Count==0)
  {
    switch(US_SEQ){
      case 0 :  TRIGER1ON;
                delayMicroseconds(2);
                TRIGER1OFF;
                delayMicroseconds(2);
                TRIGER1ON;
                delayMicroseconds(10);
                TRIGER1OFF;
                break;
      case 1 :  TRIGER2ON;  
                delayMicroseconds(2);
                TRIGER2OFF;
                delayMicroseconds(2);
                TRIGER2ON;  
                delayMicroseconds(10);
                TRIGER2OFF;
                break;
      case 2 :  TRIGER3ON;  
                delayMicroseconds(2);
                TRIGER3OFF;
                delayMicroseconds(2);
                TRIGER3ON;  
                delayMicroseconds(10);
                TRIGER3OFF;
                break;
      case 3 :  TRIGER4ON;  
                delayMicroseconds(2);
                TRIGER4OFF;
                delayMicroseconds(2);
                TRIGER4ON;  
                delayMicroseconds(10);
                TRIGER4OFF;
                break;  
    }
  }
  else if (Pulse_Width_Count < 300)
    switch(US_SEQ){
      case 0 :
          if (ECHO_CNT[US_SEQ] && !ECHO_REPLY[US_SEQ] && !ECHO1)
          {
            ECHO_CNT[US_SEQ]=Pulse_Width_Count-ECHO_CNT[US_SEQ];
            ECHO_REPLY[US_SEQ]=1;
          }
          if (!ECHO_CNT[US_SEQ] && ECHO1) ECHO_CNT[US_SEQ]=Pulse_Width_Count;
          break;
      case 1 :
          if (ECHO_CNT[US_SEQ] && !ECHO_REPLY[US_SEQ] && !ECHO2)
          {
            ECHO_CNT[US_SEQ]=Pulse_Width_Count-ECHO_CNT[US_SEQ];
            ECHO_REPLY[US_SEQ]=1;
          }
          if (!ECHO_CNT[US_SEQ] && ECHO2) ECHO_CNT[US_SEQ]=Pulse_Width_Count;
          break;

      case 2 :
          if (ECHO_CNT[US_SEQ] && !ECHO_REPLY[US_SEQ] && !ECHO3)
          {
            ECHO_CNT[US_SEQ]=Pulse_Width_Count-ECHO_CNT[US_SEQ];
            ECHO_REPLY[US_SEQ]=1;
          }
          if (!ECHO_CNT[US_SEQ] && ECHO3) ECHO_CNT[US_SEQ]=Pulse_Width_Count;
          break;
           
      case 3 :
          if (ECHO_CNT[US_SEQ] && !ECHO_REPLY[US_SEQ] && !ECHO4)
          {
            ECHO_CNT[US_SEQ]=Pulse_Width_Count-ECHO_CNT[US_SEQ];
            ECHO_REPLY[US_SEQ]=1;
          }
          if (!ECHO_CNT[US_SEQ] && ECHO4) ECHO_CNT[US_SEQ]=Pulse_Width_Count;
          break;
    }
  else if (Pulse_Width_Count == 300)  // Stop Measurement after 16ms (Distance : 272cm)
  {
    if (!ECHO_REPLY[US_SEQ]) ECHO_CNT[US_SEQ]=Pulse_Width_Count;
    UltraSensor[US_SEQ]=ECHO_CNT[US_SEQ];
  }
}

void ScanULTRA(void)
{
  US_Check();

  Pulse_Width_Count++;
  if (Pulse_Width_Count > 300) // Wait till 15ms, Restart Ultra-Sonic Measurement
  {
    Pulse_Width_Count=0;

    ECHO_CNT[US_SEQ]=0;
    ECHO_REPLY[US_SEQ]=0;
    
    US_SEQ++;
    if (US_SEQ > 3) US_SEQ = 0;
  }
}

void TimerInterrupt(void)
{
  if(ADCSRA & 0x10)   Read_ADC();
  ScanULTRA();
}
