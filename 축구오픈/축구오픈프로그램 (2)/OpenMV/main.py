#--------------------------------------------------------------------
# 축구로봇 OpenMV 카메라 프로그램 (Open용)
# 경기장에서 공, 노랑색 골대, 파랑색 골대를 찾아서 그 중 가장 큰 물체를 감지 함.
# 로봇으로부터 2 Byte의 요청데이터가 오면 (0xAA, 모드)
# 수집한 데이터를 송신함 (0xAA, ....)
# 보내는 정보는 각 색상중 가장 큰 물체의 위치값만 보냄.
#--------------------------------------------------------------------

#--------------------------------------------------------------------
# PACKET 구조 (총 14 바이트)
#--------------------------------------------------------------------
# 1. PACKET HEADER      : unsigned char 1 BYTE (0xAA)
# 2. X of YELLOW GOAL   : signed int 2 BYTE (LITTLE ENDIAN 형식)
# 3. Y of YELLOW GOAL   : signed int 2 BYTE (LITTLE ENDIAN 형식)
# 4. X of BLUE GOAL     : signed int 2 BYTE (LITTLE ENDIAN 형식)
# 5. Y of BLUE GOAL     : signed int 2 BYTE (LITTLE ENDIAN 형식)
# 6. X of BALL          : signed int 2 BYTE (LITTLE ENDIAN 형식)
# 7. Y of BALL          : signed int 2 BYTE (LITTLE ENDIAN 형식)
# 8. CHECK SUM          : unsigned char 1 BYTE (EXCLUSIVE OR 방식)
#--------------------------------------------------------------------

#***************************************************************
# 각 색상에 대한 정의 (L min, L max, A min, A max, B min, B max)
# 여기 값들을 조정하여야 함.
# 우측 이미지 아래에 있는 HISTOGRAM color space를 "LAB Color Space"로 설정
# 우측 이미지에서 각 색상의 대표 영역을 설정하고
# 아래에 있는 스펙트럼 값중 Min - 5 정도를 min 값에 Max + 5 정도를 max에
#**************************************************************
BlueThreshold   = [(13, 20, -20, -5, -2, 15)]
YellowThreshold = [(24, 44, -10, 15, 22, 43)]
BallThreshold   = [(55, 70, 20, 30, 0, 15)]
#**************************************************************


import sensor, image, time, math, struct
from pyb import Pin, LED, UART

DisplayMark = True

#--------------------------------------------------------------------
# 반사경 중심의 좌표 (VGA 모드에서..)
#--------------------------------------------------------------------
CenterX = 325
CenterY = 247
#--------------------------------------------------------------------
# 경기장 벽을 넘지 않을 범위의 반지름( VGA 에서 )
#--------------------------------------------------------------------
Radius = 240

uart = UART(3, 115200)

red_led   = LED(1)
green_led = LED(2)
blue_led  = LED(3)
ir_led	= LED(4)
#--------------------------------------------------------------------
# 카메라 동작 모드
#   0 : 공, 노랑골대, 파랑골대를 전부 찾음.
#   1 : 노랑골대만 찾음
#   2 : 파랑골대만 찾음
#   3 : 공만 찾음.
#--------------------------------------------------------------------
Mode = 0    #1:YELLOW GOAL, 2:BLUE GOAL, 3:BALL, 0:ALL

#--------------------------------------------------------------------
# 카메라 초기화
#--------------------------------------------------------------------
clock = time.clock()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.VGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(True)
sensor.set_auto_whitebal(True)
sensor.set_hmirror(True)

PACKET = b''
length = {'Int8' : 1, 'uInt8' : 1, 'Int16' : 2, 'uInt16' : 2, 'Int32' : 4, 'uInt32' : 4, 'float' : 4}
format = {'Int8' : '<b', 'uInt8' : '<B', 'Int16' : '<h', 'uInt16' : '<H',
     'Int32' : '<l', 'uInt32' : '<L', 'float' : '<f'}

#--------------------------------------------------------------------
# 정보를 전송할 패켓에 추가함.
#--------------------------------------------------------------------
def addData(type, array):
    global PACKET
    if isinstance(array,list):
        array = array[:5]
        for element in array:
            PACKET += struct.pack(format[type], element)
    else:
        PACKET += struct.pack(format[type], array)

#--------------------------------------------------------------------
# 골대에 관한 정보를 전송할 패켓에 추가함.
#--------------------------------------------------------------------
def addGoal(goal, isGOAL):
    global img, PACKET, resolution, shift

    img.draw_line(CenterX, CenterY, goal.cx(),goal.cy(), color = (0,0,0),thickness= 2)
    x = goal.cx() - CenterX
    y = CenterY - goal.cy()
    if isGOAL:
        theta = math.atan2(y,x)
        x2 = goal.w()/2
        y2 = goal.h()/2
        if x < 0:   x += x2
        else:	   x -= x2
        if y < 0:   y += y2
        else:	   y -= y2
        l = math.sqrt(x*x + y*y)
        x = int(l * math.cos(theta))
        y = int(l * math.sin(theta))
    print("X:",x,"Y:",y)
    img.draw_line(CenterX, CenterY, CenterX + x, CenterY - y, color = (255,255,255),thickness= 2)
    PACKET += struct.pack('<h',x)
    PACKET += struct.pack('<h',y)


#--------------------------------------------------------------------
# 사진을 찍어서 설정된 모드에 맞는 물체를 찾는다.
#--------------------------------------------------------------------
def Find():
    global img, PACKET, resolution, shift, Mode

    clock.tick()

    # 사진을 찍는다.
    img = sensor.snapshot()

    YG = ''
    BG = ''
    BALL = ''

    if Mode in (1,0):   # 모드가 1 또는 0일때 노랑골대를 찾는다.
        for blob in img.find_blobs(YellowThreshold, pixels_threshold=200, area_threshold=200,merge = True):
            if DisplayMark:
                img.draw_circle(blob.cx(), blob.cy(), 4, color = (255, 0, 0), fill = False)
            if YG == '':
                if math.sqrt(math.pow(blob.cx()-CenterX,2)+math.pow(blob.cy()-CenterY,2)) < Radius:
                    YG = blob
            elif math.sqrt(math.pow(blob.cx()-CenterX,2)+math.pow(blob.cy()-CenterY,2)) < Radius and blob.pixels() > YG.pixels():
                YG = blob

    if Mode in (2,0):   # 모드가 2 또는 0일때 파랑골대를 찾는다.
        for blob in img.find_blobs(BlueThreshold, pixels_threshold=200, area_threshold=200,merge = True):
            if DisplayMark:
                img.draw_circle(blob.cx(), blob.cy(), 4, color = (255, 255, 255), fill = False)
            if BG == '':
                if math.sqrt(math.pow(blob.cx()-CenterX,2)+math.pow(blob.cy()-CenterY,2)) < Radius:
                    BG = blob
            elif math.sqrt(math.pow(blob.cx()-CenterX,2)+math.pow(blob.cy()-CenterY,2)) < Radius and blob.pixels() > BG.pixels():
                BG = blob

    #if Mode in (3,0):   # 모드가 3 또는 0일때 공을 찾는다.
    # 공은 항상 찾는다.
    for blob in img.find_blobs(BallThreshold, pixels_threshold=20, area_threshold=20,merge = True):
        if DisplayMark:
            img.draw_circle(blob.cx(), blob.cy(), 4, color = (0, 0, 0), fill = False)
        if BALL == '':
            if math.sqrt(math.pow(blob.cx()-CenterX,2)+math.pow(blob.cy()-CenterY,2)) < Radius:
                BALL = blob
        elif math.sqrt(math.pow(blob.cx()-CenterX,2)+math.pow(blob.cy()-CenterY,2)) < Radius and blob.pixels() > BALL.pixels():
            BALL = blob

    # 찍은 사진 이미지 위에 좌표선과 유효영역을 표시한다.
    img.draw_line(CenterX,0, CenterX, 479, color = (255,0,0))
    img.draw_line(0, CenterY, 639, CenterY, color = (255,0,0))
    img.draw_circle(CenterX, CenterY, Radius, color = (255,0,0))

    # 전송할 패켓을 생성한다.
    PACKET = b''
    # 노란색 골대 정보를 패켓에 추가한다.
    PACKET += struct.pack('<B', 0xAA)   # PACKET HEADER
    if YG == '':
        PACKET += struct.pack('<h', 0)  # 찾은 정보가 없으면 X,Y 정보를 0으로 만듬.
        PACKET += struct.pack('<h', 0)
    else:
        addGoal(YG, True)

    # 파란색 골대 정보를 패켓에 추가한다.
    if BG == '':
        PACKET += struct.pack('<h', 0)  # 찾은 정보가 없으면 X,Y 정보를 0으로 만듬.
        PACKET += struct.pack('<h', 0)
    else:
        addGoal(BG, True)

    # 공 정보를 패켓에 추가한다.
    if BALL == '':
        PACKET += struct.pack('<h', 0)  # 찾은 정보가 없으면 X,Y 정보를 0으로 만듬.
        PACKET += struct.pack('<h', 0)
    else:
        addGoal(BALL, False)

    # 체크섬을 계산한다.
    chksum = 0xFF
    for i in PACKET:
        chksum ^= i
    chksum &= 0xFF
    PACKET += struct.pack('<B', chksum)

    x1=0
    y1=0
    x2=0
    y2=0

    if YG:
        y1 = -(CenterY-YG.cy())
        x1 = YG.cx()-CenterX

    if BG:
        y2 = -(CenterY-BG.cy())
        x2 = BG.cx()-CenterX

    xt = 0
    yt = 0
    if YG and BG:
        yt = int((y1+y2) / 2)
        xt = int((x1+x2) /2)
    elif YG:
        yt = y1
        xt = int(x1 /2)
    elif BG:
        yt = y2
        xt = int(x2 /2)

    img.draw_line(CenterX, CenterY, (CenterX + xt), (CenterY + yt), color = (0,255,0),thickness= 1)
    A = int(math.degrees(math.atan2(xt, yt)))

    print("angle:",A)

    if DisplayMark:
        print("FPS:",clock.fps())

#--------------------------------------------------------------------
# 메인 루프
#--------------------------------------------------------------------
while(True):
    Find()  # 물체를 찾는다.
    if uart.any():
        print("COMMAND RECEIVED")
        time.sleep_ms(1)
        InBytes = bytearray()
        InBytes = uart.read()
        if len(InBytes) >= 2:
            if InBytes[0] == 0xAA:  # 로봇으로부터 명령을 수신하였음.
                green_led.on()
                #----------------------------------------------------
                # 만약 모드가 변경되었으면 물체를 다시 찾는다.
                #----------------------------------------------------
                if Mode != InBytes[1] and InBytes[1] < 4:
                    Mode = InBytes[1]
                    Find()
                uart.write(PACKET, 14)  # 패켓 전송
                print(PACKET)
                green_led.off()
    while uart.any():
        InBytes = uart.read()   # 수신 버퍼 비우기

    time.sleep_ms(5)
