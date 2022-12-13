#Plotter for Robot Communicator

from re import S
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time
import pandas as pd
# writing to file


'''
struct reportData {
    int32_t l_speed, r_speed = 0;
    int32_t l_ticks, r_ticks = 0;
'''
file_location = 0
line_count = 0

f = open('data.csv', 'r')

plt.style.use('fivethirtyeight')
plt.tight_layout()


def animate(i):

    window_size = 500
    data = pd.read_csv('data.csv') # [l_speed, r_speed, l_ticks, r_ticks]
    x = data['count']
    y1 = data['l_ticks']
    y2 = data['r_ticks']
    y3 = data['r_ticks']
    y4 = data['r_ticks']
    if len(x) > window_size:
        s = len(x) - window_size
        x = x[s:]
        y1 = y1[s:]
        y2 = y2[s:]
        y3 = y3[s:]
        y4 = y4[s:]

    plt.cla()

    plt.plot(x, y1, label='Left Ticks', linewidth=1)
    plt.plot(x, y2, label='Right Ticks', linewidth=1)
    plt.plot(x, y3, label='Left Speed', linewidth=1)
    plt.plot(x, y4, label='Right Speed', linewidth=1)

    plt.legend(loc='upper left')
    plt.tight_layout()

anim = animation.FuncAnimation(plt.gcf(), animate, interval=10)


plt.show()





