## 2차원 배열

height = 5
width = 5

# c에서는 안됨 < int image[height][width]
image = [[0, 0, 0, 0],
         [0, 0, 0, 0],
         [0, 0, 0, 0]]      # 2차원 배열 (3, 4)
image[0][0] = 100
image[2][3] = 100
print(image)


## 가변 배열
## 파이썬은 2차원 배열 정의, 사용 모두 반복문임...
## 그대신 free 같은거는 신경 안써도 됨 ㅇㅇ
height, width = 3, 4
image = []             # 2차원 배열
tmp = []               # 1차원 임시 배열
for i in range(height):
    tmp = []
    for k in range(width):
        tmp.append(0)
    image.append(tmp)
print(image)

## 가변 배열
height, width = 3, 4
image = [[0 for _ in range(width)] for _ in range(height)]
print(image)