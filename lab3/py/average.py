import matplotlib.pyplot as plt
import numpy as np

data_sisd = np.genfromtxt("SISD.csv", delimiter=',')
data_simd = np.genfromtxt("SIMD.csv", delimiter=',')

plt.scatter(data_sisd[:,0], data_sisd[:,1], color='r', label='SISD')
plt.plot(data_sisd[:,0], data_sisd[:,1], '--', color='r')

plt.scatter(data_simd[:,0], data_simd[:,1], color='b', label='SIMD')
plt.plot(data_simd[:,0], data_simd[:,1], '--', color='b')

plt.legend()

plt.ylabel('czas [s]')
plt.xlabel('Liczba liczb')
plt.title('Średni czas operacji')
plt.savefig("avg.png")
