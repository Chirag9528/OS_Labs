import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

data1 = pd.read_csv('data1.csv')
data2 = pd.read_csv('data2.csv')
data3 = pd.read_csv('data3.csv')

plt.title('Regular Vs Concurrent LinkedList Vs Hand Over Linked List')
plt.xlabel('Inserts and LookUps')
plt.ylabel('Time Taken')
plt.plot(data1['x'] , data1['y'] , label='Regular')
plt.plot(data2['x'] , data2['y'] , marker='x' , label='Concurrent')
plt.plot(data3['x'] , data3['y'] , marker='+' , label='Hand Over Method')
plt.legend()
plt.grid()

plt.show()