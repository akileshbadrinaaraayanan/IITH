"""
This module demonstrates finding the top K,median K and least K terms
in the inverted index based on their document frequency
"""
import numpy as np
from part1 import createindexes
def get_topk(invertedindex):
    """
    This function outputs the top k terms with highest document frequency.
    It takes the inverted index as input and sorts the terms based on their document frequency.

    Args:
        invertedindex (dict): The dictionary storing the inverted index
    """

    invertedlist = list(invertedindex)
    freqlist = [len(invertedindex[word][0]) for word in invertedlist]
    sortedfreq = np.argsort(-1*np.array(freqlist))
    for i in range(k):
        word = invertedlist[sortedfreq[i]]
        gap = 0
        postlist = np.array(invertedindex[word][0])
        gap = np.mean(postlist[1:]-postlist[:-1]) if len(postlist) > 1 else 0
        print("Term : " + word + "\t")
        print("PostingSize: " + str(freqlist[sortedfreq[i]]) + "\tAverage Gap : " + str(gap) + "\n")

def get_medk(invertedindex):
    """
    This function outputs the median k terms based on the document frequency.
    It takes the inverted index as input and sorts the terms based on their document frequency.

    Args:
        invertedindex (dict): The dictionary storing the inverted index
    """

    invertedlist = list(invertedindex)
    freqlist = [len(invertedindex[word][0]) for word in invertedlist]
    sortedfreq = np.argsort(-1*np.array(freqlist))
    start = len(sortedfreq)//2 - k//2
    for i in range(start, start + k):
        word = invertedlist[sortedfreq[i]]
        gap = 0
        postlist = np.array(invertedindex[word][0])
        gap = np.mean(postlist[1:]-postlist[:-1]) if len(postlist) > 1 else 0
        print("Term : " + word + "\t")
        print("PostingSize: " + str(freqlist[sortedfreq[i]]) + "\tAverage Gap : " + str(gap) + "\n")

def get_leastk(invertedindex):
    """
    This function outputs the k terms with least document frequency
    It takes the inverted index as input and sorts the terms based on their document frequency.

    Args:
        invertedindex (dict): The dictionary storing the inverted index
    """
    invertedlist = list(invertedindex)
    freqlist = [len(invertedindex[word][0]) for word in invertedlist]
    sortedfreq = np.argsort(np.array(freqlist))
    for i in range(k):
        word = invertedlist[sortedfreq[i]]
        gap = 0
        postlist = np.array(invertedindex[word][0])
        gap = np.mean(postlist[1:]-postlist[:-1]) if len(postlist) > 1 else 0
        print("Term : " + word + "\t")
        print("Size: " + str(freqlist[sortedfreq[i]]) + "\tAverage Gap : " + str(gap) + "\n")

if __name__ == "__main__":
    k = 20
    INDEX = createindexes()
    for ind, item in enumerate(INDEX):
        print("Inverted Index " + str(ind+1) + "\n")
        print("TOP K\n")
        get_topk(item)
        print("MEDIAN K\n")
        get_medk(item)
        print("LEAST K\n")
        get_leastk(item)
        print(60*"-")
