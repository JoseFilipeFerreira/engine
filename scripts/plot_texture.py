import sys
import matplotlib.pyplot as plt

with open(sys.argv[1], 'rt') as f:
    x_arr = []
    y_arr = []
    for row in f:
        l = row.split(' ')
        x_arr.append(float(l[6]))
        y_arr.append(float(l[7]))
    plt.plot(x_arr, y_arr)
    #plt.scatter(x_arr, y_arr, color= "green", marker= "*",s=10)
    plt.show()

