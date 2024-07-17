# 이 스크립트는 http://futures-plus.com/ai4y/co2-mm-mlo.csv에서 월별 평균 이산화탄소 값을 다운로드하고 표시합니다.

import os
import urllib.request
import pandas as pd
import matplotlib.pyplot as plt

# 현재 디렉토리 확인 및 새 디렉토리 생성
cwd = os.getcwd()
print("Current working directory: %s" % cwd)
path = os.path.join(cwd,"Magic_Data")
if not os.path.isdir(path):
    os.mkdir(path)
    print("Newly created directory to store files: %s" % path)

# 다운로드를 위한 파일 경로를 제공합니다. 파일 경로에는 폴더 경로와 파일 이름이 모두 포함됩니다.
full_path = os.path.join(path,"co2-mm-mlo.csv")


# 파일 다운로드
print("Downloading file..........")

urllib.request.urlretrieve("http://sl2files.sustainablelivinglab.org/co2-mm-mlo.csv", full_path)


print("\n")
print("File has been downloaded!")

# 기본 표시
print("Performing super difficult data analysis..........")
fn = "co2-mm-mlo.csv"
carbon_dioxide = pd.read_csv(os.path.join(path,fn))
plt.plot(carbon_dioxide['Date'],carbon_dioxide['Average'])
plt.xlabel('Time')
plt.ylabel('CO2 concentration in ppm')
plt.show()
