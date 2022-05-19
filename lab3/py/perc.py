import matplotlib.pyplot as plt
import numpy as np


data_sisd = np.genfromtxt("SISD_8192.csv", delimiter=',')
data_simd = np.genfromtxt("SIMD_8192.csv", delimiter=',')

data_simd = data_simd[:,1]
data_sisd = data_sisd[:,1]

print(data_simd)
print(data_sisd)

percentages = [(x-y)/x*100 for x, y in zip(data_sisd, data_simd)]
print(percentages)

plt.bar(['dodawanie', 'odejmowanie', 'mnożenie', 'dzielenie'], percentages)

# plt.scatter(['dodawanie', 'odejmowanie', 'mnożenie', 'dzielenie'], data_simd[:,1], color='b', label='SIMD')
# plt.plot(['dodawanie', 'odejmowanie', 'mnożenie', 'dzielenie'], data_simd[:,1], '--', color='b')

# plt.legend()

plt.ylabel('[%]')
plt.title('Zysk w czasie z zastosowania mechanizmów SIMD')
plt.savefig("perc.png")
