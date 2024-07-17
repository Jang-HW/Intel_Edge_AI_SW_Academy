import sys
from tkinter import *
from tkinter.filedialog import *
from tkinter import messagebox
from tkinter.simpledialog import *
import os.path
import math


#####################################################
### 함수부
####################
# 공통 함수부
def malloc2D(h, w, init=0):
    memory = [[init for _ in range(w)] for _ in range(h)]
    return memory

def openImage():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    fullname = askopenfilename(parent=window, filetypes=(("RAW 파일", "*.raw"), ("모든 파일", "*.*")))

    # 1. 입력 이미지 크기
    fsize = os.path.getsize(fullname)  ## 파일 크기를 알려줌 (byte)
    inH = inW = int(math.sqrt(fsize))

    # 2. 메모리 할당
    inImage = malloc2D(inH, inW, 0)

    # 3. 파일 ---> 메모리
    rfp = open(fullname, "rb")
    for i in range(inH):
        for k in range(inW):
            inImage[i][k] = ord(rfp.read(1))

    equalImage()

def saveImage():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW
    global saved

    saved = True
    if (outImage == None or len(outImage) == 0):  ## 영상처리를 안했다면
        return

    ## 1단계: 파일 열기
    wfp = asksaveasfile(parent=window, mode="wb", defaultextension="*.raw",
                        filetypes=(("RAW 파일", "*.raw"), ("모든 파일", "*.*")))

    ## 2단계: 파일 쓰기
    import struct
    for i in range(outH):
        for k in range(outW):
            wfp.write(struct.pack("B", outImage[i][k]))

    ## 3단계: 파일 닫기
    wfp.close()
    messagebox.showinfo("저장 성공", wfp.name + " 파일이\n저장 완료 되었습니다.")

def accumulate():
    global statusbar, accumulation
    accumul = "중첩 "

    if (accumulation):
        accumulation = False
        accumul += "OFF"
    else:
        accumulation = True
        accumul += "ON"
    statusbar.config(text=accumul)

def displayImage():
    global window, canvas, paper, fullName, accumulation
    global inImage, outImage, inH, inW, outH, outW
    global saved

    ## 기존에 이미지가 있으면
    # 캔버스를 밑으로 내리면서 계속 그리는 것과 같아서 여러번 로딩시 이상해짐
    ## 기존에 이미지가 있으면 캔버스 뜯어내기
    if (canvas != None):
        canvas.destroy()

    ## 벽, 캔버스, 종이 설정
    window.geometry(str(outH) + "x" + str(outW))
    canvas = Canvas(window, height=outH, width=outW, bg="yellow")  ## 칠판 준비
    paper = PhotoImage(height=outH, width=outW)  ## 종이 준비
    canvas.create_image((outH // 2, outW // 2), image=paper, state="normal")

    ## 메모리 ---> 화면
    ## 기존 출력은 느림
    # Why? 한 점씩 여러번 (화면으로) 변형 후 옮겨서 느린것
    # 기존 ver.
    # for i in range(outH):
    #     for k in range(outW):
    #         r = g = b = outImage[i][k]
    #         paper.put("#%02x%02x%02x" % (r, g, b), (k, i))

    ## 더블 버퍼링 변형 ver
    # 모두 메모리 상에 변형해서 저장한 후에 빠르게 띄우기
    # Why? 여러 점을 한 번에 변형 해서
    #               한 번에 화면으로 옮겨서 빠른 것
    rgbString = ""  ## 전체 이미지를 16진수 문자열 저장
    for i in range(outH):
        oneString = ""  ## 픽셀 한 줄을 16진수 문자열 저장
        for k in range(outW):
            # 각 픽셀을 16진수 문자열로 저장
            r = g = b = outImage[i][k]
            oneString += "#%02x%02x%02x " % (r, g, b)  ## 띄어쓰기는 픽셀 간 구분을 위해
        ## 픽셀 한 줄을 변형한 string을 전체 이미지 string에 추가
        rgbString += '{' + oneString + '} '  ## 띄어쓰기는 마찬가지로 행 간 구분을 위해
    paper.put(rgbString)
    canvas.pack()

    ## 저장 안 되었을때 프로그램 종료 전에 물어보기 위한 변수 설정
    saved = False

    ## 계속 편집을 위한 inImage의 변경
    if (accumulation):
        inH = outH
        inW = outW
        inImage = malloc2D(inH, inW)

        for i in range(outH):  # for (int i = 0; i < outH; i++):
            for k in range(outW):  # for (int k = 0; k < outW; k++):
                inImage[i][k] = outImage[i][k]

def exitImage():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW
    global saved

    if (saved):
        window.destroy()
    else:
        unsaveExit = messagebox.askquestion("저장하지 않고 종료", "저장되지 않았습니다.\n종료하시겠습니까?")
        if (unsaveExit == "yes"):
            window.destroy()


####################
## 영상 처리부

# 동일 영상 알고리즘
def equalImage():
    # 동일 이미지 알고리즘
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    for i in range(inH):
        for k in range(inW):
            outImage[i][k] = inImage[i][k]

    displayImage()
# 밝히기 어둡게 하기
def changeLightImage():
    ## 밝게 어둡게
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    scale = askinteger("밝기 변경", "밝기를 얼마나 바꿀까요?\n-255~255", maxvalue=255, minvalue=-255)

    for i in range(inH):
        for k in range(inW):
            px = inImage[i][k] + scale
            if (px > 255):
                px = 255
            if (px < 0):
                px = 0
            outImage[i][k] = px
    displayImage()
# 이미지 반전 시키기
def reverseImage():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    for i in range(inH):
        for k in range(inW):
            outImage[i][k] = 255 - inImage[i][k]
    print("반전")
    displayImage()
# 감마 변환
def gammaImage():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    scale = askinteger("감마 변경", "감마 값을 얼마로 할까요?\n0~100", maxvalue=100, minvalue=0)
    for i in range(inH):
        for k in range(inW):
            outImage[i][k] = int(255 * (inImage[i][k] / 255) ** (30 / scale))
    displayImage()

# 파라볼라 변환
def paraCAPImage():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    for i in range(inH):
        for k in range(inW):
            outImage[i][k] = int(255. * ((inImage[i][k] / 127.) - 1) ** 2)
    displayImage()
def paraCUPImage():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    for i in range(inH):
        for k in range(inW):
            val = int(255 - 255 * ((inImage[i][k] / 127) - 1) ** 2)
            if (val > 255):
                outImage[i][k] = 255
            elif (val < 0):
                outImage[i][k] = 0
            else:
                outImage[i][k] = val
    displayImage()

# 흑백
def monoImage():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    mid, count = 0, 0
    valCount = [0 for _ in range(256)]

    for i in range(inH):
        for k in range(inW):
            if (inImage[i][k] < 128):
                outImage[i][k] = 0
            else:
                outImage[i][k] = 255
    displayImage()
def monoAvgImage():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    mid = 0
    for i in range(inH):
        for k in range(inW):
            mid += inImage[i][k]
    mid = mid / (inH * inW)

    ## 평균으로 출력배열 변경
    messagebox.showinfo("평균", "평균 : " + str(mid))
    for i in range(inH):
        for k in range(inW):
            if (inImage[i][k] < mid):
                outImage[i][k] = 0
            else:
                outImage[i][k] = 255
    displayImage()
def monoMidImage():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    mid, count = 0, 0
    valCount = []

    for _ in range(256):
        valCount.append(0)

    for i in range(inH):
        for k in range(inW):
            valCount[inImage[i][k]] += 1

    for i in range(256):
        count += valCount[i]
        if (count > (inW * inH) // 2):
            mid = i
            break

    ## 중앙값으로 출력배열 변경
    messagebox.showinfo("중앙값", "중앙값 : " + str(mid))
    for i in range(inH):
        for k in range(inW):
            if (inImage[i][k] < mid):
                outImage[i][k] = 0
            else:
                outImage[i][k] = 255
    displayImage()

# 마스크 알고리즘
def maskAndImage():
    # And 연산으로 적용
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    maskH = 0
    maskW = 0
    maskImage = []

    maskName = askopenfilename(parent=window, filetypes=(("RAW 파일", "*.raw"), ("모든 파일", "*.*")))
    # 1. 입력 이미지 크기
    fsize = os.path.getsize(maskName)  ## 파일 크기를 알려줌 (byte)
    maskH = maskW = int(math.sqrt(fsize))
    # 2. 메모리 할당
    maskImage = malloc2D(maskH, maskW, 0)
    # 3. 파일 ---> 메모리
    rfp = open(maskName, "rb")
    for i in range(maskH):
        for k in range(maskW):
            maskImage[i][k] = ord(rfp.read(1))

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    for i in range(inH):
        for k in range(inW):
            if ((i >= maskH) or (k >= maskW)):
                outImage[i][k] = inImage[i][k]
            else:
                outImage[i][k] = inImage[i][k] & maskImage[i][k]

    displayImage()
def maskOrImage():
    # And 연산으로 적용
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    maskH = 0
    maskW = 0
    maskImage = []

    maskName = askopenfilename(parent=window, filetypes=(("RAW 파일", "*.raw"), ("모든 파일", "*.*")))
    # 1. 입력 이미지 크기
    fsize = os.path.getsize(maskName)  ## 파일 크기를 알려줌 (byte)
    maskH = maskW = int(math.sqrt(fsize))
    # 2. 메모리 할당
    maskImage = malloc2D(maskH, maskW, 0)
    # 3. 파일 ---> 메모리
    rfp = open(maskName, "rb")
    for i in range(maskH):
        for k in range(maskW):
            maskImage[i][k] = ord(rfp.read(1))

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    for i in range(inH):
        for k in range(inW):
            if ((i >= maskH) or (k >= maskW)):
                outImage[i][k] = inImage[i][k]
            else:
                outImage[i][k] = inImage[i][k] | maskImage[i][k]

    displayImage()
def maskXorImage():
    # And 연산으로 적용
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    maskH = 0
    maskW = 0
    maskImage = []

    maskName = askopenfilename(parent=window, filetypes=(("RAW 파일", "*.raw"), ("모든 파일", "*.*")))
    # 1. 입력 이미지 크기
    fsize = os.path.getsize(maskName)  ## 파일 크기를 알려줌 (byte)
    maskH = maskW = int(math.sqrt(fsize))
    # 2. 메모리 할당
    maskImage = malloc2D(maskH, maskW, 0)
    # 3. 파일 ---> 메모리
    rfp = open(maskName, "rb")
    for i in range(maskH):
        for k in range(maskW):
            maskImage[i][k] = ord(rfp.read(1))

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    for i in range(inH):
        for k in range(inW):
            if ((i >= maskH) or (k >= maskW)):
                outImage[i][k] = inImage[i][k]
            else:
                outImage[i][k] = inImage[i][k] ^ maskImage[i][k]

    displayImage()

# not 변환
def notImage():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    for i in range(inH):
        for k in range(inW):
            val = ~inImage[i][k]
            if (val < 0):
                val = abs(val % 255)
            outImage[i][k] = val
    displayImage()

# 명암 대비
def constrastStratchImage():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    for i in range(inH):
        for k in range(inW):
            if (inImage[i][k] < 50):
                outImage[i][k] = 0
            elif (inImage[i][k] > 200):
                outImage[i][k] = 255
            else:
                outImage[i][k] = int(inImage[i][k] * 51 // 30 - 85)
    displayImage()
def constrastZipImage():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    for i in range(inH):
        for k in range(inW):
            outImage[i][k] = int(inImage[i][k] * 30 // 51 + 50)
    displayImage()

# 포스터라이징
def posterizeImage():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    for i in range(inH):
        for k in range(inW):
            if (inImage[i][k] // 50 == 0):
                outImage[i][k] = 0
            elif (inImage[i][k] // 50 == 1):
                outImage[i][k] = 50
            elif (inImage[i][k] // 50 == 2):
                outImage[i][k] = 100
            elif (inImage[i][k] // 50 == 3):
                outImage[i][k] = 150
            elif (inImage[i][k] // 50 == 4):
                outImage[i][k] = 200
            else:
                outImage[i][k] = 255
    displayImage()

# 범위 강조
def areaStressImage():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    for i in range(inH):
        for k in range(inW):
            if (75 < inImage[i][k] < 175):
                outImage[i][k] = 255
            else:
                outImage[i][k] = inImage[i][k]
    displayImage()


######

# 줌인 - 생기는 픽셀 양선형 보간법으로 채우기
def zoomIn():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    scale = askinteger("확대 배율", "얼마나 확대할까요?\n0~100", maxvalue=100, minvalue=0)
    outH = inH * scale
    outW = inW * scale
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    ## 입력 이미지를 출력 이미지로 복사하기
    blockH, blockW = [0] * 2  ## 블록의 칸
    rateH, rateW = [0.0] * 2  ## 블록 내에서 좌우 와의 비율
    inLocateH, inLocateW = [0.0] * 2  ## 블록 내에서 좌우 와의 비율
    c1, c2, c3, c4 = [0.0] * 4

    for i in range(outH):
        for k in range(outW):
            rateH = i // scale
            rateW = k // scale
            blockH = round(rateH)
            blockW = round(rateW)
            inLocateH = rateH - blockH
            inLocateW = rateW - blockW

            if ((blockH < 0) or (blockH >= (inH - 1)) or (blockW < 0) or (blockW >= (inW - 1))):
                outImage[i][k] = 255
            else:
                # 각 점들과 블록 내의 위치 비율을 반영해서 점의 색 지정
                c1 = float(inImage[blockH][blockW])  # 좌 상단점 지정
                c2 = float(inImage[blockH][blockW + 1])  # 우 상단점 지정
                c3 = float(inImage[blockH + 1][blockW + 1])  # 우 하단점 지정
                c4 = float(inImage[blockH + 1][blockW])  # 좌 하단점 지정
                outImage[i][k] = int(c1 * (1 - inLocateH) * (1 - inLocateW)
                                     + c2 * inLocateW * (1 - inLocateH)
                                     + c3 * inLocateW * inLocateH
                                     + c4 * (1 - inLocateW) * inLocateH)
    displayImage()

# 줌아웃 - 없어지는 픽셀 중앙 값으로 줄이기
def zoomOut():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    scale = askinteger("축소 배율", "얼마나 축소할까요?\n0~100", maxvalue=100, minvalue=0)
    outH = inH // scale
    outW = inW // scale
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    tmp = []  # 줌 아웃 계산을 위한 1차원 배열
    num = 0  # tmp에서 몇 칸째 인지를 추적할 변수

    # tmp 배열 생성
    for _ in range(scale ** 2):
        tmp.append(0)

    # 입력배열 --> 임시배열
    for i in range(inH // scale):
        for k in range(inW // scale):
            for m in range(scale):
                for n in range(scale):
                    # 이 안의 칸은 즉 inImage[scale * i + m][scale * k + n]
                    tmp[num] = inImage[scale * i + m][scale * k + n]
                    num += 1
            num = 0
            sorted(tmp)
            outImage[i][k] = tmp[scale ** 2 // 2]
    displayImage()

# 회전
def rotate():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    scale = askinteger("돌릴 각도", "얼마나 돌릴까요?")
    scale %= 360
    radian = scale * 3.1415925386 / 180.0
    ## xd = cos * xs - sin * ys
    ## yd = sin * xs + cos * ys

    cx = inH // 2
    cy = inW // 2

    for i in range(outH):
        for k in range(outW):
            xd = i
            yd = k
            xs = math.cos(radian) * (xd - cx) + math.sin(radian) * (yd - cy)
            ys = -math.sin(radian) * (xd - cx) + math.cos(radian) * (yd - cy)
            xs = int(xs + cx)
            ys = int(ys + cy)

            if ((0 <= xs) & (xs < inH) & (0 <= ys) & (ys < inW)):
                outImage[xd][yd] = inImage[xs][ys]
    displayImage()

# 회전 - 안 깨지고 안 짤리게
def rotate2():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    scale = askinteger("돌릴 각도", "얼마나 돌릴까요?\n-360~360", minvalue=-360, maxvalue=360)
    if(scale < 0):
        scale += 360
    radian = scale * 3.1415925386 / 180.0

    ## (중요!) 출력이미지의 크기를 알고리즘에 따라 결정
    outH = int(abs(inW * math.sin(radian)) + abs(inH * math.cos(radian)))
    outW = int(abs(inW * math.cos(radian)) + abs(inH * math.sin(radian)))
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
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

            if ((0 <= xs) & (xs < inH) & (0 <= ys) & (ys < inW)):
                outImage[xd][yd] = inImage[xs][ys]
    displayImage()

# 이동
def moveVertical():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    scale = askinteger("세로 이동", "세로로 얼마나 이동할까요?\n" + str(-outH) + "~" + str(outH),
                       minvalue=-outH, maxvalue=outH)

    for i in range(inH):
        for k in range(inW):
            if (i + scale >= outH):
                continue
            if (i + scale < 0):
                continue
            outImage[i + scale][k] = inImage[i][k]
    displayImage()
def moveHorizonal():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    scale = askinteger("가로 이동", "가로로 얼마나 이동할까요?\n" + str(-outW) + "~" + str(outW),
                       minvalue=-outW, maxvalue=outW)

    for i in range(inH):
        for k in range(inW):
            if (k + scale >= outW):
                continue
            if (k + scale < 0):
                continue
            outImage[i][k + scale] = inImage[i][k]
    displayImage()

# 미러링
def mirrorHorizon():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    for i in range(inH):
        for k in range(inW):
            outImage[i][inW - k - 1] = inImage[i][k]
    print("가로 미러링")
    displayImage()
def mirrorVertical():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    for i in range(inH):
        for k in range(inW):
            outImage[inH - i - 1][k] = inImage[i][k]
    print("세로 미러링")
    displayImage()

# 모핑
def morph():
    global window, canvas, paper, fullName, accumulation
    global inImage, outImage, inH, inW, outH, outW

    maskH = 0
    maskW = 0
    maskImage = []

    maskName = askopenfilename(parent=window, filetypes=(("RAW 파일", "*.raw"), ("모든 파일", "*.*")))
    # 1. 입력 이미지 크기
    fsize = os.path.getsize(maskName)  ## 파일 크기를 알려줌 (byte)
    maskH = maskW = int(math.sqrt(fsize))
    # 2. 메모리 할당
    maskImage = malloc2D(maskH, maskW, 0)
    # 3. 파일 ---> 메모리
    rfp = open(maskName, "rb")
    for i in range(maskH):
        for k in range(maskW):
            maskImage[i][k] = ord(rfp.read(1))

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    scale = askinteger("모핑 단계", "몇 단계가 필요한가요?\n0~100", maxvalue=100, minvalue=0)

    tmpAccumulation = accumulation
    accumulation = False
    for morph in range(scale):
        for i in range(inH):
            for k in range(inW):
                if ((i >= maskH) or (k >= maskW)):
                    outImage[i][k] = inImage[i][k]
                else:
                    outImage[i][k] = int(
                        ((scale - 1 - morph) / (scale - 1)) * inImage[i][k] + (morph / (scale - 1)) * maskImage[i][k])
        displayImage()
        messagebox.showinfo("다음 단계", str(morph + 1) + "/" + str(scale))
        accumulation = tmpAccumulation


######

# 히스토스트레칭
def histoStractch():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    ## new = (old - low) / (high - low) * 255.0
    ## high, low 찾기
    high = inImage[0][0]
    low = inImage[0][0]
    for i in range(inH):
        for k in range(inW):
            if (inImage[i][k] < low):
                low = inImage[i][k]
            if (inImage[i][k] > high):
                high = inImage[i][k]

    ## 입력 배열 --> 출력 배열
    new, old = [0] * 2
    for i in range(inH):
        for k in range(inW):
            old = inImage[i][k];
            new = (int)((old - low) / (high - low) * 255.0)
            if (new > 255):
                new = 255
            if (new < 0):
                new = 0
            outImage[i][k] = new
    displayImage()

# 엔드인
def endIn():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    ## new = (old - low) / (high - low) * 255.0
    ## high, low 찾기
    high = inImage[0][0]
    low = inImage[0][0]
    for i in range(inH):  ## for (int i = 1; i < inH; i++)
        for k in range(inW):  ## for (int k = 1; k < inW; k++)
            if (inImage[i][k] < low):
                low = inImage[i][k]
            if (inImage[i][k] > high):
                high = inImage[i][k]

    ##### 여기가 다름 ####
    high -= 50
    low += 50

    ## 입력 배열 --> 출력 배열
    new, old = [0] * 2
    for i in range(inH):  ## for (int i = 0; i < inH; i++)
        for k in range(inW):  ## for (int k = 0; k < inW; k++)
            old = inImage[i][k]
            new = int((float(old - low)) / (high - low) * 255.0)
            if (new > 255):
                new = 255
            if (new < 0):
                new = 0
            outImage[i][k] = new
    displayImage()

# 히스토그램 평활화
def histoFlatten():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    ## 1) 빈도수 세기
    histo = []
    for i in range(256):
        histo.append(0)

    for i in range(inH):  ## for (int i = 0; i < inH; i++)
        for k in range(inW):  ## for (int k = 0; k < inW; k++)
            histo[inImage[i][k]] += 1

    ## 2) 누적 히스토그램 생성
    sumHisto = []
    for i in range(256):
        sumHisto.append(0)

    sumHisto[0] = histo[0]
    for i in range(256):  ## for (int i = 1; i < 256; i++)
        sumHisto[i] = sumHisto[i - 1] + histo[i]

    ## 3) 정규화 히스토그램 생성
    ## normalHisto = sumHisto * (1.0 / (inH * inW)) * 255.0
    normalHisto = []
    for i in range(256):
        normalHisto.append(1.0)
    for i in range(256):  ## for (int i = 1; i < 256; i++)
        normalHisto[i] = sumHisto[i] * (1.0 / (inH * inW)) * 255.0

    ## 4) 입력 배열을 치환 --> 출력 배열
    for i in range(inH):  ## for (int i = 0; i < inH; i++)
        for k in range(inW):  ## for (int k = 0; k < inW; k++)
            outImage[i][k] = int(normalHisto[inImage[i][k]])
    displayImage()


######
# 엠보싱
def emboss():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    mask = [[-1.0, 0.0, 0.0],
            [0.0, 0.0, 0.0],
            [0.0, 0.0, 1.0]]

    ## 임시메모리 할당(실수형으로)
    tmpInImage = malloc2D(inH + 2, inW + 2, 127)
    tmpOutImage = malloc2D(outH, outW)

    ## 입력 이미지 --> 임시 입력 이미지
    for i in range(inH):  # for (int i = 0; i < inH; i++)
        for k in range(inW):  # for (int k = 0; k < inW; k++)
            tmpInImage[i + 1][k + 1] = inImage[i][k]

    ##  *****mask*****     회선 연산
    S = 0.0
    for i in range(inH):  # for (int i = 0; i < inH; i++)
        for k in range(inW):  # for (int k = 0; k < inW; k++)
            ## 마스크와 한 점을 중심으로한 3*3 곱하기
            S = 0.0  ## 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

            for m in range(3):  # for (int m = 0; m < 3; m++)
                for n in range(3):  # for (int n = 0; n < 3; n++)
                    S += tmpInImage[i + m][k + n] * mask[m][n]

            tmpOutImage[i][k] = S

    ## 후처리 : 마스크의 합계에 따라
    for i in range(outH):  # for (int i = 0; i < outH; i++)
        for k in range(outW):  # for (int k = 0; k < outW; k++)
            tmpOutImage[i][k] += 127.0

    ## 임시출력영상 --> 출력영상
    for i in range(outH):  # for (int i = 0; i < outH; i++) {
        for k in range(outW):  # for (int k = 0; k < outW; k++) {
            if (tmpOutImage[i][k] > 255.0):
                outImage[i][k] = 255
            elif (tmpOutImage[i][k] < 0.0):
                outImage[i][k] = 0
            else:
                outImage[i][k] = int(tmpOutImage[i][k])
    displayImage()

# 원하는 대로 블러링
def blurr():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    scale = askinteger("블러링 크기", "블러 효과의 강도는 얼마로 할까요?\n0~20", maxvalue=20, minvalue=0)
    lenScale = scale * 2 + 1
    mask = malloc2D(lenScale, lenScale, (1 / float(lenScale ** 2)))

    ## 임시메모리 할당(실수형으로)
    tmpInImage = malloc2D(inH + scale * 2, inW + scale * 2, 127)
    tmpOutImage = malloc2D(outH, outW)

    ## 입력 이미지 --> 임시 입력 이미지
    for i in range(inH):  # for (int i = 0; i < inH; i++)
        for k in range(inW):  # for (int k = 0; k < inW; k++)
            tmpInImage[i + scale][k + scale] = inImage[i][k]

    ##  *****mask*****     회선 연산
    S = 0.0
    for i in range(inH):  # for (int i = 0; i < inH; i++)
        for k in range(inW):  # for (int k = 0; k < inW; k++)
            ## 마스크와 한 점을 중심으로한 곱하기
            S = 0.0  ## 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

            for m in range(lenScale):  # for (int m = 0; m < 3; m++)
                for n in range(lenScale):  # for (int n = 0; n < 3; n++)
                    S += tmpInImage[i + m][k + n] * mask[m][n]

            tmpOutImage[i][k] = S

    # ## 후처리 : 마스크의 합계에 따라
    # for i in range(outH):                   # for (int i = 0; i < outH; i++)
    #     for k in range(outW):               # for (int k = 0; k < outW; k++)
    #         tmpOutImage[i][k] += 127.0

    ## 임시출력영상 --> 출력영상
    for i in range(outH):  # for (int i = 0; i < outH; i++) {
        for k in range(outW):  # for (int k = 0; k < outW; k++) {
            if (tmpOutImage[i][k] > 255.0):
                outImage[i][k] = 255
            elif (tmpOutImage[i][k] < 0.0):
                outImage[i][k] = 0
            else:
                outImage[i][k] = int(tmpOutImage[i][k])
    displayImage()

# 가우시안 스무딩
def gausinaSmooth():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    mask = [[1. / 16, 1. / 8, 1. / 16],
            [1. / 8, 1. / 4, 1. / 8],
            [1. / 16, 1. / 8, 1. / 16]]

    ## 임시메모리 할당(실수형으로)
    tmpInImage = malloc2D(inH + 2, inW + 2, 127)
    tmpOutImage = malloc2D(outH, outW)

    ## 입력 이미지 --> 임시 입력 이미지
    for i in range(inH):  # for (int i = 0; i < inH; i++)
        for k in range(inW):  # for (int k = 0; k < inW; k++)
            tmpInImage[i + 1][k + 1] = inImage[i][k]

    ##  *****mask*****     회선 연산
    S = 0.0
    for i in range(inH):  # for (int i = 0; i < inH; i++)
        for k in range(inW):  # for (int k = 0; k < inW; k++)
            ## 마스크와 한 점을 중심으로한 3*3 곱하기
            S = 0.0  ## 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

            for m in range(3):  # for (int m = 0; m < 3; m++)
                for n in range(3):  # for (int n = 0; n < 3; n++)
                    S += tmpInImage[i + m][k + n] * mask[m][n]

            tmpOutImage[i][k] = S

    ## 후처리 : 마스크의 합계에 따라
    for i in range(outH):  # for (int i = 0; i < outH; i++)
        for k in range(outW):  # for (int k = 0; k < outW; k++)
            tmpOutImage[i][k] += 127.0

    ## 임시출력영상 --> 출력영상
    for i in range(outH):  # for (int i = 0; i < outH; i++) {
        for k in range(outW):  # for (int k = 0; k < outW; k++) {
            if (tmpOutImage[i][k] > 255.0):
                outImage[i][k] = 255
            elif (tmpOutImage[i][k] < 0.0):
                outImage[i][k] = 0
            else:
                outImage[i][k] = int(tmpOutImage[i][k])
    displayImage()

# 샤프닝
def sharf1():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    mask = [[-1., -1., -1.],
            [-1., 9., -1.],
            [-1., -1., -1.]]

    ## 임시메모리 할당(실수형으로)
    tmpInImage = malloc2D(inH + 2, inW + 2, 127)
    tmpOutImage = malloc2D(outH, outW)

    ## 입력 이미지 --> 임시 입력 이미지
    for i in range(inH):  # for (int i = 0; i < inH; i++)
        for k in range(inW):  # for (int k = 0; k < inW; k++)
            tmpInImage[i + 1][k + 1] = inImage[i][k]

    ##  *****mask*****     회선 연산
    S = 0.0
    for i in range(inH):  # for (int i = 0; i < inH; i++)
        for k in range(inW):  # for (int k = 0; k < inW; k++)
            ## 마스크와 한 점을 중심으로한 3*3 곱하기
            S = 0.0  ## 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

            for m in range(3):  # for (int m = 0; m < 3; m++)
                for n in range(3):  # for (int n = 0; n < 3; n++)
                    S += tmpInImage[i + m][k + n] * mask[m][n]

            tmpOutImage[i][k] = S

    ## 후처리 : 마스크의 합계에 따라
    for i in range(outH):  # for (int i = 0; i < outH; i++)
        for k in range(outW):  # for (int k = 0; k < outW; k++)
            tmpOutImage[i][k] += 127.0

    ## 임시출력영상 --> 출력영상
    for i in range(outH):  # for (int i = 0; i < outH; i++) {
        for k in range(outW):  # for (int k = 0; k < outW; k++) {
            if (tmpOutImage[i][k] > 255.0):
                outImage[i][k] = 255
            elif (tmpOutImage[i][k] < 0.0):
                outImage[i][k] = 0
            else:
                outImage[i][k] = int(tmpOutImage[i][k])
    displayImage()
def sharf2():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    mask = [[0., -1., 0.],
            [-1., 5, -1.],
            [0., -1., 0.]]

    ## 임시메모리 할당(실수형으로)
    tmpInImage = malloc2D(inH + 2, inW + 2, 127)
    tmpOutImage = malloc2D(outH, outW)

    ## 입력 이미지 --> 임시 입력 이미지
    for i in range(inH):  # for (int i = 0; i < inH; i++)
        for k in range(inW):  # for (int k = 0; k < inW; k++)
            tmpInImage[i + 1][k + 1] = inImage[i][k]

    ##  *****mask*****     회선 연산
    S = 0.0
    for i in range(inH):  # for (int i = 0; i < inH; i++)
        for k in range(inW):  # for (int k = 0; k < inW; k++)
            ## 마스크와 한 점을 중심으로한 3*3 곱하기
            S = 0.0  ## 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.

            for m in range(3):  # for (int m = 0; m < 3; m++)
                for n in range(3):  # for (int n = 0; n < 3; n++)
                    S += tmpInImage[i + m][k + n] * mask[m][n]

            tmpOutImage[i][k] = S

    ## 후처리 : 마스크의 합계에 따라
    for i in range(outH):  # for (int i = 0; i < outH; i++)
        for k in range(outW):  # for (int k = 0; k < outW; k++)
            tmpOutImage[i][k] += 127.0

    ## 임시출력영상 --> 출력영상
    for i in range(outH):  # for (int i = 0; i < outH; i++) {
        for k in range(outW):  # for (int k = 0; k < outW; k++) {
            if (tmpOutImage[i][k] > 255.0):
                outImage[i][k] = 255
            elif (tmpOutImage[i][k] < 0.0):
                outImage[i][k] = 0
            else:
                outImage[i][k] = int(tmpOutImage[i][k])
    displayImage()

# 유사 연산자
def simular():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    ## 임시메모리 할당(실수형으로)
    tmpInImage = malloc2D(inH + 2, inW + 2, 127)
    tmpOutImage = malloc2D(outH, outW)

    ## 입력 이미지 --> 임시 입력 이미지
    for i in range(inH):  # for (int i = 0; i < inH; i++)
        for k in range(inW):  # for (int k = 0; k < inW; k++)
            tmpInImage[i + 1][k + 1] = inImage[i][k]

    ##  *****mask*****     회선 연산
    S = 0.0
    for i in range(inH):  # for (int i = 0; i < inH; i++)
        for k in range(inW):  # for (int k = 0; k < inW; k++)
            S = abs(tmpInImage[i + 1][k + 1] - tmpInImage[i][k])
            for m in range(3):  # for (int m = 0; m < 3; m++)
                for n in range(3):  # for (int n = 0; n < 3; n++)
                    if ((m != 1) & (n != 1)):
                        S = max(abs(tmpInImage[i + 1][k + 1] - tmpInImage[i + m][k + n]), S)
            tmpOutImage[i][k] = S

    # ## 후처리 : 마스크의 합계에 따라
    # for i in range(outH):                   # for (int i = 0; i < outH; i++)
    #     for k in range(outW):               # for (int k = 0; k < outW; k++)
    #         tmpOutImage[i][k] += 127.0

    ## 임시출력영상 --> 출력영상
    for i in range(outH):  # for (int i = 0; i < outH; i++) {
        for k in range(outW):  # for (int k = 0; k < outW; k++) {
            if (tmpOutImage[i][k] > 255.0):
                outImage[i][k] = 255
            elif (tmpOutImage[i][k] < 0.0):
                outImage[i][k] = 0
            else:
                outImage[i][k] = int(tmpOutImage[i][k])
    displayImage()

# 원하는 필터 직접 입력
def wantFilter():
    global window, canvas, paper, fullName
    global inImage, outImage, inH, inW, outH, outW

    ### 주의 ### 출력 영상 크기 결정 및 할당 (알고리즘에 의존)
    outH = inH
    outW = inW
    outImage = malloc2D(outH, outW)

    ########## 영상처리 ##########
    mask = []
    # 필터 크기 입력
    scale = askinteger("필터 크기", "필터의 크기는 몇인가요?\n3, 5, 7, 9", minvalue=3, maxvalue=9)
    if (scale != 3 and scale != 5 and scale != 7 and scale != 9):
        messagebox.showerror("필터 크기 오류","필터 크기는 3, 5, 7, 9로 입력 해주세요!")
        return

    # 필터 입력
    for i in range(1, scale + 1):
        tmp = askstring("필터의 내용", "필터의 %d번째 줄을 알려주세요.\n(예시) 1 2 3 2 1" % i)
        tmpList = []
        tmpMask = []
        tmpList = tmp.split(" ")
        for i in range(scale):
            if (i < len(tmpList) and not (tmpList[i].isalpha())):
                tmpMask.append(int(tmpList[i]))
            else:
                tmpMask.append(0)
        mask.append(tmpMask)
    ## 필터의 총합을 1으로 맞추기
    sum = 0
    for i in range(scale):
        for k in range(scale):
            sum += mask[i][k]
    if (sum != 0):
        for i in range(scale):
            for k in range(scale):
                mask[i][k] /= sum

    ## 임시메모리 할당(실수형으로)
    tmpInImage = malloc2D(inH + scale - 1, inW + scale - 1, 127)
    tmpOutImage = malloc2D(outH, outW)

    ## 입력 이미지 --> 임시 입력 이미지
    for i in range(inH):  # for (int i = 0; i < inH; i++)
        for k in range(inW):  # for (int k = 0; k < inW; k++)
            tmpInImage[i + (scale - 1) // 2][k + (scale - 1) // 2] = inImage[i][k]

    ##  *****mask*****     회선 연산
    S = 0.0
    for i in range(inH):  # for (int i = 0; i < inH; i++)
        for k in range(inW):  # for (int k = 0; k < inW; k++)
            ## 마스크와 한 점을 중심으로한 곱하기
            S = 0.0  ## 마스크 9개와 입력값 9개를 각각 곱해서 합한 값.
            for m in range(scale):  # for (int m = 0; m < 3; m++)
                for n in range(scale):  # for (int n = 0; n < 3; n++)
                    S += tmpInImage[i + m][k + n] * mask[m][n]

            tmpOutImage[i][k] = S

    ## 후처리 : 마스크의 합계에 따라
    if (sum != 0):
        for i in range(outH):  # for (int i = 0; i < outH; i++)
            for k in range(outW):  # for (int k = 0; k < outW; k++)
                tmpOutImage[i][k] += 127.0

    ## 임시출력영상 --> 출력영상
    for i in range(outH):  # for (int i = 0; i < outH; i++) {
        for k in range(outW):  # for (int k = 0; k < outW; k++) {
            if (tmpOutImage[i][k] > 255.0):
                outImage[i][k] = 255
            elif (tmpOutImage[i][k] < 0.0):
                outImage[i][k] = 0
            else:
                outImage[i][k] = int(tmpOutImage[i][k])
    displayImage()


#####################################################
### 전역변수부

window, statusbar, canvas, paper = None, None, None, None
inImage, outImage = [], []
inH, inW, outH, outW = [0] * 4
accumulation = False
saved = True
fullName = ""

#####################################################
### 메인 코드부

window = Tk()
window.geometry("500x500")
window.resizable(width=False, height=False)
window.title("영상처리 RC 1")

## 메뉴 만들기
mainMenu = Menu(window)  ## 메뉴 바 공간만 만듦
window.config(menu=mainMenu)

fileMenu = Menu(mainMenu, tearoff=0)
mainMenu.add_cascade(label='파일', menu=fileMenu)  ## 상위메뉴 (파일) / 메뉴바에 요소추가
fileMenu.add_command(label='열기', command=openImage)  ## 파일 / 열기
fileMenu.add_command(label='저장', command=saveImage)  ## 파일 / 저장
fileMenu.add_separator()
fileMenu.add_command(label="중첩", command=accumulate)  ## 파일 / 중첩
fileMenu.add_separator()
fileMenu.add_command(label='종료', command=exitImage)  ## 파일 / 종료

pixelMenu = Menu(mainMenu, tearoff=0)
paraMenu = Menu(pixelMenu, tearoff=False)
monoMenu = Menu(pixelMenu, tearoff=False)
maskMenu = Menu(pixelMenu, tearoff=False)
constrastMenu = Menu(pixelMenu, tearoff=False)
mainMenu.add_cascade(label='화소점처리', menu=pixelMenu)  ## 상위메뉴 (화소점처리)
pixelMenu.add_command(label='동일 이미지', command=equalImage)
pixelMenu.add_command(label='밝기 변경', command=changeLightImage)
pixelMenu.add_command(label='반전', command=reverseImage)
pixelMenu.add_command(label='감마 변환', command=gammaImage)
pixelMenu.add_cascade(label='파라볼라 변환', menu=paraMenu)
paraMenu.add_command(label='CAP', command=paraCAPImage)
paraMenu.add_command(label='CUP', command=paraCUPImage)
pixelMenu.add_cascade(label='흑백', menu=monoMenu)
monoMenu.add_command(label='127', command=monoImage)
monoMenu.add_command(label='평균값', command=monoAvgImage)
monoMenu.add_command(label='중앙값', command=monoMidImage)
pixelMenu.add_cascade(label='마스크', menu=maskMenu)
maskMenu.add_command(label='AND', command=maskAndImage)
maskMenu.add_command(label='OR', command=maskOrImage)
maskMenu.add_command(label='XOR', command=maskXorImage)
pixelMenu.add_command(label='NOT', command=notImage)
pixelMenu.add_cascade(label='명암대비', menu=constrastMenu)
constrastMenu.add_command(label='스트레칭', command=constrastStratchImage)
constrastMenu.add_command(label='압축', command=constrastZipImage)
pixelMenu.add_command(label='포스터라이징', command=posterizeImage)
pixelMenu.add_command(label='범위 강조', command=areaStressImage)

geoMenu = Menu(mainMenu, tearoff=False)
rotateMenu = Menu(geoMenu, tearoff=False)
moveMenu = Menu(geoMenu, tearoff=False)
mirrorMenu = Menu(geoMenu, tearoff=False)
mainMenu.add_cascade(label='기하학', menu=geoMenu)  ## 상위메뉴 (기하학)
geoMenu.add_command(label='줌인', command=zoomIn)
geoMenu.add_command(label='줌아웃', command=zoomOut)
geoMenu.add_cascade(label='회전', menu=rotateMenu)
rotateMenu.add_command(label='원본 크기', command=rotate)
rotateMenu.add_command(label='안 짤리게', command=rotate2)
geoMenu.add_cascade(label='이동', menu=moveMenu)
moveMenu.add_command(label='가로', command=moveHorizonal)
moveMenu.add_command(label='세로', command=moveVertical)
geoMenu.add_cascade(label='미러링', menu=mirrorMenu)
mirrorMenu.add_command(label='가로', command=mirrorHorizon)
mirrorMenu.add_command(label='세로', command=mirrorVertical)
geoMenu.add_command(label='모핑', command=morph)

histoMenu = Menu(mainMenu, tearoff=False)
mainMenu.add_cascade(label='히스토그램', menu=histoMenu)  ## 상위메뉴 (히스토그램)
histoMenu.add_command(label='히스토스트레칭', command=histoStractch)
histoMenu.add_command(label='엔드인', command=endIn)
histoMenu.add_command(label='평활화', command=histoFlatten)

areaMenu = Menu(mainMenu, tearoff=False)
sharfMenu = Menu(geoMenu, tearoff=False)
mainMenu.add_cascade(label='화소 영역처리', menu=areaMenu)  ## 상위메뉴 (화소 영역처리)
areaMenu.add_command(label='엠보싱', command=emboss)
areaMenu.add_command(label='블러링', command=blurr)
areaMenu.add_command(label='가우시안 스무딩', command=gausinaSmooth)
areaMenu.add_cascade(label='샤프닝', menu=sharfMenu)
sharfMenu.add_command(label='1', command=sharf1)
sharfMenu.add_command(label='2', command=sharf2)
areaMenu.add_command(label='유사 연산자', command=simular)
areaMenu.add_command(label='원하는 필터', command=wantFilter)

statusbar = Label(window, text="중첩 OFF", relief=SUNKEN, bd=1, anchor=E)
statusbar.pack(side=BOTTOM, fill=X)
window.mainloop()
