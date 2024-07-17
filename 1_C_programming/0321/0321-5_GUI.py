from tkinter import *
from tkinter.filedialog import *
from tkinter import messagebox
import math



### 함수부
def malloc2D(h, w):
    memory = [[0 for _ in range(w)] for _ in range(h)]
    return memory

def openImage():
    # 열기를 클릭하면 실행할 함수
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW
    #fullname = 'D:/RAW/Pet_RAW(squre)/Pet_RAW(256x256)/cat11_256.raw'
    fullname = askopenfilename(parent=window, filetypes=(("RAW 파일", "*.raw"), ("모든 파일", "*.*")))


    # 1. 입력영상 크기 설정
    inH = inW = 256

    #2. 메모리 확보
    inImage = malloc2D(inH, inW)

    #3. 파일 ---> 메모리
    rfp = open(fullname, "rb")
    for i in range(inH):
        for k in range(inW):
            inImage[i][k] = ord(rfp.read(1))
            # read(1)는 한 바이트를 글자로 읽어오는데 그거를 숫자로 변경해야됨
                         ## 그래서 ord라는 타입으로 변경 / 반대는
    rfp.close()

def displayImage():
    # 열기를 클릭하면 실행할 함수
    global window, canvas, paper
    global inImage, outImage, inH, inW, outH, outW

    for i in range(outH):
        for k in range(outW):
            r = g = b = outImage[i][k]
            paper.put('#%02x%02x%02x' % (r, g, b), (k, i))

############
# 영상 처리 함수

# 동일 영상 알고리즘
def equalImage():
    global window, canvas, paper
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

    displayImage()

# 밝히기 어둡게 하기
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
    displayImage()

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
    displayImage()

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
    displayImage()

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
    global inImage, outImage, inH, inW, outH, outW

    ## (중요!) 출력이미지의 크기를 알고리즘에 따라 결정
    scale = int(input("줌아웃 배수: "))
    outH = inH // scale
    outW = inW // scale

    ## 출력이미지 메모리 확보
    outImage = malloc2D(outH, outW)

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
    global inImage, outImage, inH, inW, outH, outW

    scale = int(input("회전 각도: "))
    scale %= 360
    radian = scale * 3.1415925386 // 180.0

    ## (중요!) 출력이미지의 크기를 알고리즘에 따라 결정
    outH = (int)(inW * math.sin(radian) + inH * math.cos(radian))
    outW = (int)(inW * math.cos(radian) + inH * math.sin(radian))

    ## 출력이미지 메모리 확보
    outImage = malloc2D(outH, outW)

    ## xd = cos * xs - sin * ys
    ## yd = sin * xs + cos * ys
    cx = inH // 2
    cy = inW // 2

    for i in range(outH):
        for k in range(outW):
            xd = i
            yd = k
            xs = math.cos(radian) * (xd - (outH // 2)) + math.sin(radian) * (yd - (outW // 2))
            ys = -math.sin(radian) * (xd - (outH // 2)) + math.cos(radian) * (yd - (outW // 2))
            xs = int(xs + cx)
            ys = int(ys + cy)

            if ((0 <= xs) & (xs < inH) & (0 <= ys) & (ys < inW)):
                outImage[xd][yd] = inImage[xs][ys]
    displayImage()

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
            outImage[i + scale][k] = inImage[i][k]
    displayImage()

# 미러링
def mirrorHorizon():
    global inImage, outImage, inH, inW, outH, outW

    ## (중요!) 출력이미지의 크기를 알고리즘에 따라 결정
    outH = inH * 2
    outW = inW * 2

    ## 출력이미지 메모리 확보
    outImage = malloc2D(outH, outW)

    for i in range(inH):
        for k in range(inW):
            outImage[i][inW - k] = inImage[i][k]
    print("가로 미러링")
    displayImage()
def mirrorVertical():
    global inImage, outImage, inH, inW, outH, outW

    ## (중요!) 출력이미지의 크기를 알고리즘에 따라 결정
    outH = inH
    outW = inW

    ## 출력이미지 메모리 확보
    outImage = malloc2D(outH, outW)

    for i in range(inH):
        for k in range(inW):
            outImage[inH - i][k] = inImage[i][k]
    print("세로 미러링")
    displayImage()

######

# 히스토 스트레칭
def histoStractch():
    global inImage, outImage, inH, inW, outH, outW

    ## (중요!) 출력이미지의 크기를 알고리즘에 따라 결정
    outH = inH
    outW = inW

    ## 출력이미지 메모리 확보
    outImage = malloc2D(outH, outW)

    ## new = (old - low) / (high-low) * 255.0
    ## high, low 찾기
    high = inImage[0][0]
    low = inImage[0][0]
    for i in range(inH):  ## for (int i = 1; i < inH; i++)
        for k in range(inW):  ## for (int k = 1; k < inW; k++)
            if (inImage[i][k] < low):    low = inImage[i][k]
            if (inImage[i][k] > high):    high = inImage[i][k]

    ## 입력 배열 --> 출력 배열
    new, old = [0] * 2
    for i in range(inH):  ## for (int i = 0; i < inH; i++)
        for k in range(inW):  ## for (int k = 0; k < inW; k++)
            old = inImage[i][k]
            new = int((float(old - low)) // (high - low) * 255.0)
            if (new > 255):
                new = 255
            if (new < 0):
                new = 0
            outImage[i][k] = new
    displayImage()

# 엔드인
def endIn():
    global inImage, outImage, inH, inW, outH, outW

    ## (중요!) 출력이미지의 크기를 알고리즘에 따라 결정
    outH = inH
    outW = inW

    ## 출력이미지 메모리 확보
    outImage = malloc2D(outH, outW)

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
            new = int((float(old - low)) // (high - low) * 255.0)
            if (new > 255):
                new = 255
            if (new < 0):
                new = 0
            outImage[i][k] = new
    displayImage()

# 히스토그램 평활화
def histoFlatten():
    global inImage, outImage, inH, inW, outH, outW

    ## (중요!) 출력이미지의 크기를 알고리즘에 따라 결정
    outH = inH
    outW = inW

    ## 출력이미지 메모리 확보
    outImage = malloc2D(outH, outW)

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
        sumHisto.append(1.0)
    for i in range(256):  ## for (int i = 1; i < 256; i++)
        normalHisto[i] = sumHisto[i] * (1.0 // (inH * inW)) * 255.0

    ## 4) 입력 배열을 치환 --> 출력 배열
    for i in range(inH):  ## for (int i = 0; i < inH; i++) 
        for k in range(inW):  ## for (int k = 0; k < inW; k++) 
            outImage[i][k] = int(normalHisto[inImage[i][k]])
    displayImage()


### 전역변수부
window, canvas, paper = None, None, None
inImage, outImage = [], []
inH, inW, outH, outW = [0] * 4


### 메인부
window = Tk()
window.geometry("512x512")
window.resizable(width=False, height=False)
window.title("영상 처리 Beta 1")

canvas = Canvas(window, height=512, width=512, bg="yellow")     ## 칠판 준비
paper = PhotoImage(height=512, width=512)                       ## 종이 준비
canvas.create_image((512//2, 512//2), image=paper, state="normal")


## 메뉴 만들기
mainMenu = Menu(window)         ## 메뉴 바 공간만 만듦
window.config(menu=mainMenu)

fileMenu = Menu(mainMenu, tearoff=False)
mainMenu.add_cascade(labe='파일', menu=fileMenu)           ## 상위메뉴 (파일) / 메뉴바에 요소추가
fileMenu.add_command(label='열기', command=openImage)      ## 파일 / 열기
fileMenu.add_command(label='저장')                         ## 파일 / 저장
fileMenu.add_separator()
fileMenu.add_command(label='종료')                         ## 파일 / 종료

pixelMenu = Menu(mainMenu, tearoff=False)
mainMenu.add_cascade(labe='화소점', menu=pixelMenu)                    ## 상위메뉴 (화소점)
pixelMenu.add_command(label='동일 영상', command=equalImage)
pixelMenu.add_command(label='밝기 변경', command=changeLightImage)
pixelMenu.add_command(label='반전', command=reverseImage)
pixelMenu.add_command(label='중앙값 흑백', command=monoMidImage)

geoMenu = Menu(mainMenu, tearoff=False)
rotateMenu = Menu(geoMenu, tearoff=False)
mirrorMenu = Menu(geoMenu, tearoff=False)
mainMenu.add_cascade(labe='기하학', menu=geoMenu)                     ## 상위메뉴 (기하학)
geoMenu.add_command(label='줌인', command=zoomIn)
geoMenu.add_command(label='줌아웃', command=zoomOut)
geoMenu.add_cascade(labe='회전', menu=rotateMenu)
rotateMenu.add_command(label='회전1', command=rotate)
rotateMenu.add_command(label='회전2', command=rotate2)
geoMenu.add_command(label='세로 이동', command=moveVertical)
geoMenu.add_cascade(labe='미러링', menu=mirrorMenu)
mirrorMenu.add_command(label='가로', command=mirrorHorizon)
mirrorMenu.add_command(label='세로', command=mirrorVertical)


## 컨트롤 == 위젯
canvas.pack()
window.mainloop()
