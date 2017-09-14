"""
This module takes the top 1000 terms with highest frequency in the collection
for each of the indexes 1-4 and plots graph.
"""
import numpy as np
import matplotlib.pyplot as plt
from part1 import createindexes

def get_highestcf(invertedlist):
    """
    This function takes the top 1000 terms with highest frequency in the collection
    and plots a graph for each index i1-i4.
    x-axis: log(i) for ith most frequent term.
    y-axis: log of collection frequency of the term.

    Args:
        invertedlist (dict).

    Returns:
        None.
    """
    cflist = [invertedlist[word][1] for word in invertedlist.keys()]
    cflist.sort(reverse=True)
    plotlist = cflist[0:1000]
    xplot = []
    for i in range(1, 1001):
        xplot.append(np.log10(i))
    plt.plot(xplot, np.log10(plotlist))
    plt.xlabel('log(rank)')
    plt.title('log(rank) vs log(collection freq)')
    plt.ylabel('log(collection freq)')
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    INDEX = createindexes()
    for ind, item in enumerate(INDEX):
        print("Inverted Index " + str(ind+1) + "\n")
        get_highestcf(item)
