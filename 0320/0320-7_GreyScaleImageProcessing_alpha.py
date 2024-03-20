import random
import math


        ## 함수부
    ## 공통
# 메모리 할당
def malloc2D(h, w):
    memory = [[0 for _ in range(w)] for _ in range(h)]
    return memory

# 이미지 출력
def printImage():
    global inImage, outImage, inH, inW, outH, outW
    for i in range(outH//2-3, outH//2+3, 1):
        for k in range(outW//2-3, outH//2+3, 1):
            print("%3d " %outImage[i][k], end=" ")
        print()
    print()

        ##영상처리
    ## 화소영역처리

## 동일 영상 알고리즘
def equalImage():
    global inImage, outImage, inH, inW, outH, outW
    ## (중요!) 출력이미지의 크기를 알고리즘에 따라 결정
    outH = inH
    outW = inW
    ## 출력이미지 메모리 확보
    outImage = malloc2D(outH, outW)

    ## 진짜 동일영상 알고리즘
    for i in range(inH):
       for k in range(inW):
           outImage[i][k] = inImage[i][k]

    printImage()

## 밝히기 어둡게 하기
def changeLightImage():
    global inImage, outImage, inH, inW, outH, outW
    ## (중요!) 출력이미지의 크기를 알고리즘에 따라 결정
    outH = inH
    outW = inW
    ## 출력이미지 메모리 확보
    outImage = malloc2D(outH, outW)

    scale = int(input("밝기 변화(-255 ~ +255): "))

    for i in range(inH):
        for k in range(inW):
            if (inImage[i][k] + scale > 255):
                outImage[i][k] = 255
            elif(inImage[i][k] + scale < 0):
                outImage[i][k] = 0
            else:
                outImage[i][k] = inImage[i][k] + scale
    printImage()

# 이미지 반전 시키기
def reverseImage():
    global inImage, outImage, inH, inW, outH, outW
    ## (중요!) 출력이미지의 크기를 알고리즘에 따라 결정
    outH = inH
    outW = inW
    ## 출력이미지 메모리 확보
    outImage = malloc2D(outH, outW)

    for i in range(inH):
        for k in range(inW):
           outImage[i][k] = 255 - inImage[i][k]
    print("반전")
    printImage()

# 픽셀의 중앙 값으로 흑백
def monoMidImage():
    global inImage, outImage, inH, inW, outH, outW
    ## (중요!) 출력이미지의 크기를 알고리즘에 따라 결정
    outH = inH
    outW = inW
    ## 출력이미지 메모리 확보
    outImage = malloc2D(outH, outW)

    mid, count = 0, 0
    valCount = []

    for _ in range(256):
        valCount.append(0)

    for i in range(inH):
        for k in range(inW):
            valCount[inImage[i][k]] += 1

    for i in range(256):
        count += valCount[i]
        if (count > (inW * inH) // 2) :
            mid = i
            break

    ## 평균으로 출력배열 변경
    print("중앙값: ", mid)
    for i in range(inH):
        for k in range(inW):
            if (inImage[i][k] < mid):
                outImage[i][k] = 0
            else:
                outImage[i][k] = 255
    printImage()

######

# 줌인 - 생기는 픽셀 양선형 보간법으로 채우기
def zoomIn():
    global inImage, outImage, inH, inW, outH, outW

    ## (중요!) 출력 이미지의 크기를 알고리즘에 따라 결정
    scale = int(input("줌인 배수: "))
    outH = inH * scale
    outW = inW * scale

    ## 출력 이미지 메모리 확보
    outImage = malloc2D(outH, outW)

    ## 입력 이미지를 출력 이미지로 복사하기
    blockH, blockW, rateH, rateW, inLocateH, inLocateW = [0] * 6
    c1, c2, c3, c4 = [0.0] * 4

    for i in range(outH):
        for k in range(outW):
            rateH = i / scale
            rateW = k / scale
            blockH = round(rateH)
            blockW = round(rateW)
            inLocateH = rateH - blockH
            inLocateW = rateW - blockW

            if ((blockH < 0) | (blockH >= (inH-1)) | (blockW < 0) | (blockW >= (inW-1))):
                outImage[i][k] = 255
            else:
                # 각 점들과 블록 내의 위치 비율을 반영해서 점의 색 지정
                C1 = float(inImage[blockH][blockW])          # 좌 상단점 지정
                C2 = float(inImage[blockH][blockW + 1])       # 우 상단점 지정
                C3 = float(inImage[blockH + 1][blockW + 1])   # 우 하단점 지정
                C4 = float(inImage[blockH + 1][blockW]);      # 좌 하단점 지정
                outImage[i][k] = (C1 * (1 - inLocateH) * (1 - inLocateW)
                                  + C2 * inLocateW * (1 - inLocateH)
                                  + C3 * inLocateW * inLocateH
                                  + C4 * (1 - inLocateW) * inLocateH)
    printImage()

# 줌아웃 - 없어지는 픽셀 중앙 값으로 줄이기
def zoomOut():
    global inImage, outImage, inH, inW, outH, outW

    ## (중요!) 출력이미지의 크기를 알고리즘에 따라 결정
    scale = int(input("줌아웃 배수: "))
    outH = inH // scale
    outW = inW // scale

    ## 출력이미지 메모리 확보
    outImage = malloc2D(outH, outW)

    tmp = []            # 줌 아웃 계산을 위한 1차원 배열
    num = 0             # tmp에서 몇 칸째 인지를 추적할 변수

    # tmp 배열 생성
    for _ in range(scale**2):
        tmp.append(0)

    # 입력배열 --> 임시배열
    for i in range(inH//scale):
        for k in range(inW//scale):
            for m in range(scale):
                for n in range(scale):
                    # 이 안의 칸은 즉 inImage[scale * i + m][scale * k + n]
                    tmp[num] = inImage[scale * i + m][scale * k + n]
                    num += 1
            num = 0
            sorted(tmp)
            outImage[i][k] = tmp[scale**2 // 2]
    printImage()

## 회전
def rotate():
    global inImage, outImage, inH, inW, outH, outW

    ## (중요!) 출력이미지의 크기를 알고리즘에 따라 결정
    outH = inH
    outW = inW

    ## 출력이미지 메모리 확보
    outImage = malloc2D(outH, outW)

    scale = int(input("회전 각도: "))
    scale %= 360
    radian = scale * 3.1415925386 / 180.0
    ## xd = cos * xs - sin * ys
    ## yd = sin * xs + cos * ys

    cx = inH / 2
    cy = inW / 2

    for i in range(outH):
        for k in range(outW):
            xd = i
            yd = k
            xs = math.cos(radian) * (xd - cx) + math.sin(radian) * (yd - cy)
            ys = -math.sin(radian) * (xd - cx) + math.cos(radian) * (yd - cy)
            xs = int(xs + cx)
            ys = int(ys + cy)

            if ((0 <= xs) & (xs < inH) & (0 <= ys) & (ys < inW)) :
                outImage[xd][yd] = inImage[xs][ys]
    printImage()

## 회전 - 안 깨지고 안 짤리게
def rotate2():
    global inImage, outImage, inH, inW, outH, outW

    ## (중요!) 출력이미지의 크기를 알고리즘에 따라 결정
    outH = inH
    outW = inW

    ## 출력이미지 메모리 확보
    outImage = malloc2D(outH, outW)

    scale = int(input("회전 각도: "))
    scale %= 360
    radian = scale * 3.1415925386 / 180.0
    ## xd = cos * xs - sin * ys
    ## yd = sin * xs + cos * ys

    cx = inH / 2
    cy = inW / 2

    for i in range(outH):
        for k in range(outW):
            xd = i
            yd = k
            xs = math.cos(radian) * (xd - (outH / 2)) + math.sin(radian) * (yd - (outW / 2))
            ys = -math.sin(radian) * (xd - (outH / 2)) + math.cos(radian) * (yd - (outW / 2))
            xs = int(xs + cx)
            ys = int(ys + cy)

            if ((0 <= xs) & (xs < inH) & (0 <= ys) & (ys < inW)) :
                outImage[xd][yd] = inImage[xs][ys]
    printImage()

# 세로 이동
def moveVertical():
    global inImage, outImage, inH, inW, outH, outW

    ## (중요!) 출력이미지의 크기를 알고리즘에 따라 결정
    outH = inH
    outW = inW

    ## 출력이미지 메모리 확보
    outImage = malloc2D(outH, outW)

    scale = int(input("세로 이동: "))

    for i in range(inH):
        if (i + scale >= outH):
            break
        if (i + scale < 0):
            break
        for k in range(inW):
            outImage[i+scale][k] = inImage[i][k]
    printImage()

# 미러링
def mirror():
    global inImage, outImage, inH, inW, outH, outW

    ## (중요!) 출력이미지의 크기를 알고리즘에 따라 결정
    outH = inH * 2
    outW = inW * 2

    ## 출력이미지 메모리 확보
    outImage = malloc2D(outH, outW)

    for i in range(inH):
        for k in range(inW):
            outImage[i][k] = inImage[i][k]
            outImage[2 * inH - i - 1][k] = inImage[i][k]
            outImage[i][2 * inW - k - 1] = inImage[i][k]
            outImage[2 * inH - i - 1][2 * inW - k - 1] = inImage[i][k]
    print("미러링")
    printImage()

######

# 히스토 스트레칭
# 엔드인
# 히스토그램 평활화

######

# 엠보싱
# 블러링
# 가우시안 스무딩
# 유사 연산자


        ##  전역 변수부
    ## 사진 정보
inImage, outImage = [], []
inH, inW, outH, outW = [0] * 4



        ## 메인부
#1. 파일 > 메모리
#1-1. 이미지 크기 계산
inH = int(input("입력 이미지 높이(128, 256, 512, ...): "))
inW = int(input("입력 이미지 폭(128, 256, 512, ...): "))

#1-2. 메모리 할당
inImage = malloc2D(inH, inW)

#1-3. 메모리로 사진 로딩
for i in range(inH):
    for k in range(inW):
        px = random.randint(0, 255)
        inImage[i][k] = px

equalImage()
changeLightImage()
reverseImage()
monoMidImage()
zoomIn()
zoomOut()
rotate()
rotate2()
moveVertical()
mirror()
