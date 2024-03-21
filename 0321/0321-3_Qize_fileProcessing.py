## 1단계: 파일 열기
rfp = open("C:/Windows/win.ini", "rt")
wfp = open("win2.txt", "wt")

## 2단계: 파일 읽기
while(True):
    line = rfp.readline()
    if(line == None or line == ""):         ## None == null
        break
    else:
        for i in range(len(line)-1, -1, -1):
            print(line[i], end = "")
            wfp.write(line[i])

## 3단계: 파일 닫기
rfp.close()
wfp.close()

######################
print("\n")
######################

## 1단계: 파일 열기
rfp = open("C:/Windows/win.ini", "rt")
wfp = open("win3.txt", "wt")

## 2단계: 파일 읽기
lineNum = 0
line = ""
txt = []
while(True):
    line = rfp.readline()
    txt.append(line)
    if (line == None or line == ""):  ## None == null
        break

for i in range(len(txt) - 1, -1, -1):
    for k in range(len(txt[i]) - 1, -1, -1):
        print(txt[i][k], end="")
        wfp.write(txt[i][k])


## 3단계: 파일 닫기
rfp.close()
wfp.close()
