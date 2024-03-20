            ## 함수부

            ## 변수부

            ## 메인부
## 조건문
num = 100
if (num > 100):
    print("100보다 크다.")
else:
    if (num < 100):
        print("100보다 작다.")
    else:
        print("100이다.")

if(num > 100):
    print("100보다 크다.")
elif(num < 100):
    print("100보다 작다.")
else:
    print("100이다.")


## 반복문
## 1부터 100까지 합계
hap = 0
for i in range(1, 101, 1):      ## for(int i = 0; i < 101; i++)
    hap = hap + i
print("1부터 100까지 합계:", hap)


## 퀴즈: 1부터 100까지 4의 배수의 합계
hap = 0
for i in range(0, 101, 4):
    hap = hap + i
print("1부터 100까지 4의 배수의 합계:", hap)


## 퀴즈: 1부터 12345까지 987의 배수의 합계
hap = 0
for i in range(1, 12346, 1):
    if (i % 987 == 0):
        hap = hap + i
print("1부터 12345까지 987의 배수의 합계:", hap)


## 퀴즈: 12345부터 67890까지 8282의 배수의 합계
hap = 0
for i in range(12345, 67891, 1):
    if (i % 8282 == 0):
        hap = hap + i
print("1부터 12345까지 8282의 배수의 합계:", hap)


## 심화: 2부터 1000까지의 소수의 합계
hap = 0
for i in range(2, 1001, 1):
    for k in range(2, i+1, 1):
        if (i % k == 0):
            if (i == k):
                hap = hap + k
                print(i, end=" ")
            break
print("2부터 1000까지의 소수의 합계:", hap)