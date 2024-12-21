import pandas as pd
import matplotlib.pyplot as plt

data1 = pd.read_csv('data1.csv')
data2 = pd.read_csv('data2.csv')

plt.title('Regular Vs Concurrent Queue')
plt.xlabel('Enqueues and Dequeues')
plt.ylabel('Time Taken')
plt.plot(data1['x'] , data1['y'] , label='Regular')
plt.plot(data2['x'] , data2['y'] , label='Concurrent')
plt.legend()
plt.grid()

plt.show()