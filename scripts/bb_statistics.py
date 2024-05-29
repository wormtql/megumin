import seaborn as sns
import matplotlib.pyplot as plt
import matplotlib as mpl
import math
from matplotlib.font_manager import FontProperties

data_string = '''
5: 2624
6: 1316
7: 561
8: 407
9: 352
10: 118
11: 176
12: 62
13: 103
14: 31
15: 25
16: 24
17: 16
18: 14
19: 32
20: 15
21: 23
22: 6
23: 4
24: 10
25: 8
26: 6
27: 9
28: 4
29: 7
30: 1
31: 13
32: 3
33: 8
34: 1
35: 2
36: 4
37: 3
38: 1
39: 5
40: 4
41: 10
43: 3
44: 2
45: 2
46: 2
47: 5
51: 1
52: 1
53: 2
62: 1
64: 2
66: 1
67: 2
69: 2
71: 1
72: 1
74: 1
75: 4
77: 1
78: 2
79: 7
80: 1
81: 1
82: 1
83: 1
84: 2
85: 1
90: 1
91: 2
93: 1
97: 1
98: 1
100: 1
101: 1
103: 1
104: 1
106: 1
108: 1
109: 1
112: 1
138: 1
166: 1
195: 1
200: 1
228: 1
229: 1
256: 1
307: 1
404: 1
'''

datax = []
datay = []
for line in data_string.splitlines():
    line = line.strip()
    if line == "":
        continue
    # print(line)
    s = line.split(":")
    # print(s)
    basic_block_size = int(s[0])
    basic_block_count = int(s[1])
    datax.append(basic_block_size)
    # datay.append(basic_block_count)
    datay.append(math.log(basic_block_count) + 1)
    # data[basic_block_size] = basic_block_count

# font_songti = FontProperties(fname=r"c:\windows\fonts\simsun.ttc")

mpl.rcParams['font.family'] = ["Times New Roman", "simsun"]
# mpl.rcParams["font.family"] = "Times New Roman"
# mpl.rcParams["font.family"] = u"宋体"
fig, ax = plt.subplots()
# ax.spines['top'].set_visible(False)
# ax.spines['right'].set_visible(False)
ax.bar(datax, datay, color=(0.1, 0.2, 0.3), width=1.1)
tick = [0, 50, 100, 150, 200, 250, 300, 350, 400]
plt.xticks(ticks=tick, fontsize=15)
plt.xlabel("基本块长度", fontsize=15)
plt.ylabel("数量（log(y)+1）", fontsize=15)
# ax.set(xticks=range(100))
plt.show()

# ax = sns.

# mpl.rcParams["font.family"] = "Times New Roman"
# # sns.set(font="宋体", rc=mpl.rcParams)
# p = sns.barplot({"x": datax, "y": datay}, x="x", y="y", native_scale=True)
# p.set(xlabel=u"基本块长度", ylabel=u"数量（y'）")
# # p.set_xticks(range(100))
# # tick = [5, 10, 15, 20, 50]
# # plt.xticks(tick, tick)
# plt.show()
