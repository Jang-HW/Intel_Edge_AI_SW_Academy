## 배열(Array) --> 리스트(List)
## 리스트는 속성 상관은 사실 없는데
    ## array라 생각하는게 편해서

import random

ary1 = [0, 0, 0]        ## int ary1[3] = {0, 0, 0};

## 10개짜리 빈 배열을 생성하자
ary2 = []
for i in range(0, 10, 1):   # => (0, 10) == (10)
    # => for _ in range(10):
    ary2.append(0)
print(ary2)

ary3 = [0 for _ in range(10)]
print(ary3)


## 100개의 배열에 0~10000까지의 숫자를 랜덤하게 채워서
    ## 최대, 최소, 합계, 평균 구하기
ary4 = []
for _ in range(100):
    ary4.append(random.randint(0, 10000))
print(ary4)

maxVal, minVal, sumVal, avgVal = [0] * 4
maxVal, minVal = ary4[0]

for i in range(len(ary4)):
    if (ary4[i] > maxVal):
        maxVal = ary4[i]
    if (ary4[i] < minVal):
        minVal = ary4[i]
    sumVal += ary4[i]

avgVal = sumVal / len(ary4)
print("최대:", maxVal)
print("최소:", minVal)
print("합계:", sumVal)
print("평균:", avgVal)
