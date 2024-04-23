            ## 함수부
def add_func(n1, n2):
    return_val = n1 + n2
    return return_val


def minus_func(n1, n2):
    return_val = n1 - n2
    return return_val


def multi_func(n1, n2):
    return_val = n1 * n2
    return return_val


def divide_func(n1, n2):
    return_val = n1 / n2
    return return_val


def share_func(n1, n2):
    return_val = n1 // n2
    return return_val


def remain_func(n1, n2):
    return_val = n1 % n2
    return return_val


def square_func(n1, n2):
    return_val = n1 ** n2
    return return_val



            ## 전역 변수부
## 선언은 없으니까 초기화로
## [0] * 3  ==  0, 0, 0
num1, num2, res = 100, 23, 0



            ## main 코드부
res = add_func(num1, num2)
print(num1, "+", num2, "=", res)

res = minus_func(num1, num2)
print(num1, "-", num2, "=", res)

res = multi_func(num1, num2)
print(num1, "*", num2, "=", res)

res = divide_func(num1, num2)
print(num1, "/", num2, "=", res)

res = share_func(num1, num2)
print(num1, "//", num2, "=", res)

res = remain_func(num1, num2)
print(num1, "%", num2, "=", res)

res = square_func(num1, num2)
print(num1, "**", num2, "=", res)
