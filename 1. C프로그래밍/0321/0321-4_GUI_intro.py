from tkinter import *
from tkinter import messagebox

### 함수부
def openImage():
    # 열기를 클릭하면 실행할 함수
    messagebox.showinfo("여기 제목", "여기 내용")

def copyImage():
    # 열기를 클릭하면 실행할 함수
    messagebox.showinfo("복사복사복사", "복사를 누르셨군요")


### 전역변수부



### 메인부
window = Tk()
window.geometry("400x200")
window.resizable(width=False, height=False)
window.title("영상 처리 Beta 1")

## 메뉴 만들기
mainMenu = Menu(window)         ## 메뉴 바 공간만 만듦
window.config(menu=mainMenu)

fileMenu = Menu(mainMenu, tearoff=False)
mainMenu.add_cascade(labe='파일', menu=fileMenu)           ## 상위메뉴 (파일) / 메뉴바에 요소추가
fileMenu.add_command(label='열기', command=openImage)      ## 파일 / 열기
fileMenu.add_command(label='저장')                         ## 파일 / 저장
fileMenu.add_separator()
fileMenu.add_command(label='종료')                         ## 파일 / 종료

editMenu = Menu(mainMenu, tearoff=False)
mainMenu.add_cascade(labe='편집', menu=editMenu)          ## 상위메뉴 (편집) / 메뉴바에 요소 추가
editMenu.add_command(label='복사', command=copyImage)     ## 편집 / 복사
editMenu.add_command(label='잘라내기')                     ## 편집 / 잘라내기
editMenu.add_separator()
editMenu.add_command(label='붙여넣기')                     ## 편집 / 붙여넣기

# 컨트롤 == 위젯
label1 = Label(window, text = "나는 글자 (label)", font=("궁서체", 20), fg="red", bg="yellow")
button1 = Button(window, text="클릭해줘잉")

label1.pack()
button1.pack(side=BOTTOM)
window.mainloop()
