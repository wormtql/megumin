import matplotlib
import matplotlib.pyplot as plt

data_x = ["1", "2", "3", "4", "5"]
# data_x = [1, 2, 3, 4, 5, 60]
# data_openssl = [161, 172, 182, 188, 191, 232]
data_openssl = [161, 172, 182, 188, 191]

matplotlib.rcParams['font.family'] = ["Times New Roman", "simsun"]

fig, ax = plt.subplots()

ax.plot(data_x, data_openssl, "o-", color=(0.1, 0.2, 0.3))
plt.xlabel("时间预算（min）", fontsize=15)
plt.ylabel("优化指令数", fontsize=15)

# plt.plot(data_x, data_openssl, color=(0.1, 0.2, 0.3))
plt.show()