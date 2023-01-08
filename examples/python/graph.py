import pandas as pd
import matplotlib.pyplot as plt
import time

while(1):
    df = pd.read_csv("../output/data.csv", sep=';')
    data = df['Data']
    fig, (ax1, ax2) = plt.subplots(nrows=1, ncols=2, figsize=(10, 4))
    ax1.hist(data, bins=100)
    ax1.set_title("Histogram")
    ax2.plot(data)
    ax2.set_title("Graph")
    print("Standard deviation: " + str(data.std()))
    plt.show()