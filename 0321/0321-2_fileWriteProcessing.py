## 1단계: 파일 열기
rfp = open("C:/Windows/win.ini", "rt")
wfp = open("pythonWrite.txt", "wt")

## 2단계: 파일 읽기
while(True):
    line = rfp.readline()
    if(line == None or line == ""):         ## None == null
        break
    else:
        print(line, end = "")
        wfp.writelines(line)

## 3단계: 파일 닫기
rfp.close()
wfp.close()

