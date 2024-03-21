## 1단계: 파일 열기
rfp = open("C:/Windows/win.ini", "rt")

## 2단계: 파일 읽기
while(True):
    line = rfp.readline()
    if(line == None or line == ""):         ## None == null
        break
    else:
        for ch in line:
            if ("0" <= ch <= "9"):
                print("*", end="")
            else:
                print(ch, end="")

## 3단계: 파일 닫기
rfp.close()