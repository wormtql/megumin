import matplotlib
import matplotlib.pyplot as plt
import math

def load_data():
    ans = []
    with open(r"E:\CLionProjects\megumin\cmake-build-release\bin\cost_over_iterations.txt", "r") as f:
        for line in f.readlines():
            ans.append(float(line))
    return ans

data = load_data()
print(len(data))


iterations = []
data2 = []
step = 2000
i = step - 1
iterations.append(1)
data2.append(data[0])
while i < len(data):
    iterations.append(i + 1)
    data2.append(data[i])
    i += step
if i != len(data) - 1:
    iterations.append(len(data) - 1)
    data2.append(data[-1])

for i in range(len(data2)):
    data2[i] = math.log(data2[i], 10)

matplotlib.rcParams['font.family'] = ["Times New Roman", "simsun"]

fig, ax = plt.subplots()
ax.plot(iterations, data2, color=(0.1, 0.2, 0.3))
plt.xlabel("迭代次数", fontsize=15)
plt.ylabel("损失函数（log(y)）", fontsize=15)

plt.show()
