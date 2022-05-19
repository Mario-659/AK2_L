import matplotlib.pyplot as plt
import numpy as np


data_sisd = np.genfromtxt("SISD_8192.csv", delimiter=',')
data_simd = np.genfromtxt("SIMD_8192.csv", delimiter=',')

plt.scatter(['dodawanie', 'odejmowanie', 'mnożenie', 'dzielenie'], data_sisd[:,1], color='r', label='SISD')
plt.plot(['dodawanie', 'odejmowanie', 'mnożenie', 'dzielenie'], data_sisd[:,1], '--', color='r')

plt.scatter(['dodawanie', 'odejmowanie', 'mnożenie', 'dzielenie'], data_simd[:,1], color='b', label='SIMD')
plt.plot(['dodawanie', 'odejmowanie', 'mnożenie', 'dzielenie'], data_simd[:,1], '--', color='b')

plt.legend()

plt.ylabel('czas [s]')
plt.title('Średni czas operacji')
plt.savefig("types.png")
