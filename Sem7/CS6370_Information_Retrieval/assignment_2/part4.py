"""
This module demonstrates a relation between the number of tokens processed
and the vocabulary size after processing of each document.
A graph of LOG(number of tokens) VS LOG(Vocab size) is plotted
"""
from nltk.stem import PorterStemmer
from matplotlib import pyplot as plt
import numpy as np
import part1 as pt1

def indexprocessor():
    """
    This function constructs lists for number of tokens processed
    and size of vocabulary.
    """
    vocab = []
    tot_tokens = 0
    for docid in range(0, 1001):
        file = open(pt1.PATH + "Document-" + str(docid) + ".txt", 'r', encoding="utf-8")
        allcontent = file.readlines()
        stoplist = pt1.getstopwords()
        corpus = []

        stemmer = PorterStemmer()
        pt1.getatitle(allcontent, corpus)
        pt1.getmetakeywords(allcontent, corpus)
        pt1.getcontent(allcontent, corpus)
        flagfordate = 0
        for i  in range(0, len(corpus)):
            if flagfordate == 1:
                flagfordate = 0
                continue
            word = corpus[i]
            if word in pt1.MONTH:
                date = ""
                if pt1.expressionfordateb(corpus, i)  is True:
                    date = corpus[i - 1] + ' '
                date = date + corpus[i]
                if pt1.expressionfordatef(corpus, i)  is True:
                    date = date + ' ' + corpus[i + 1]
                word = date
                print(word)
            if word in stoplist:
                continue
            word = stemmer.stem(word)
            tot_tokens = tot_tokens + 1
            if word not in vocab:
                vocab.append(word)

        TOKENCOUNT.append(tot_tokens)
        VOCABSIZE.append(len(vocab))

if __name__ == "__main__":

    TOKENCOUNT = []
    VOCABSIZE = []
    indexprocessor()
    plt.plot(np.log10(TOKENCOUNT), np.log10(VOCABSIZE))
    plt.xlabel("Log(Number of tokens processed)")
    plt.ylabel("Log(Vocabulary size)")
    plt.title("Index Processor")
    plt.show()
