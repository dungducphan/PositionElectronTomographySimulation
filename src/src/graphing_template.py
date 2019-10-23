import matplotlib.pyplot as plt
plt.plot([3,3.5,4,4.5,5,5.5,6,6.5,7], [11700,8929,10340,10120,9690,11580,10890,11600,11810])
plt.axis([0, 8, 8000, 12000])
plt.xlabel('Crystal Width')
plt.ylabel('Average Number of Photons')
plt.show()
