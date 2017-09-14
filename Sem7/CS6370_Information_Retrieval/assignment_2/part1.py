"""
This module demonstrates building an inverted index
using the corpus created in Assignment 1.
The crawled data consists of URL, Title, Date,Meta-Keywords,
Content.
"""
import re
import math
import os
from nltk.stem import PorterStemmer

PATH = "dataA/"
MONTH = ['jan', 'feb', 'mar', 'apr', 'may']
MONTH = MONTH + ['june', 'july', 'aug', 'sep', 'oct']
MONTH = MONTH + ['nov', 'dec', 'january', 'february', 'march']
MONTH = MONTH + ['april', 'may', 'june', 'july', 'august']
MONTH = MONTH + ['september', 'october', 'november', 'december']

def getstopwords():
    """
    This function obtains the stopwords defined in
    stopwords.txt into a list stoplist.
    stopwords.txt contains stop words defined in
    http://www.lextek.com/manuals/onix/stopwords1.html

    Args:
        None

    Returns:
        list
    """
    file = open('stopWords.txt', 'r')
    stoplist = []
    for word in file.readlines():
        word = word.strip('\n')
        stoplist.append(word)
    return stoplist

def increasecount(invertedlist, word, docid):
    """
    This function adds a word to the inverted index if
    it is not already present.
    It also keeps track of the doc ids containing a
    particular word.
    In addition, it also maintains the number of occurences
    of the word in the entire corpus.

    Args:
        invertedlist (dict), word (str), docid(int)

    Returns:
        None
    """
    if len(word) == 0:
        return
    if word not in invertedlist.keys():
        invertedlist[word] = [[], 0]
    if docid not in invertedlist[word][0]:
        invertedlist[word][0].append(docid)
    invertedlist[word][1] = invertedlist[word][1] + 1

def processword(word):
    """
    This function is for pre-processing a word.
    It converts all words into lower case.
    It also removes special characters like
    ?, !, - etc.

    Args:
        word (str)

    Returns:
        word (str)
    """
    word = word.lower()
    word = word.strip('()?,!`.-:\"\n \'')
    return word

def getatitle(allcontent, corpus):
    """
    This function uses regex matching to
    get title of a document from allcontent list and adds it to
    the corpus list.

    Args:
        allcontent (list), corpus (list)

    Returns:
        None
    """
    for i in range(0, len(allcontent)):
        words = re.split(r'\s+', allcontent[i])
        if words[0] == "Title":
            for j in range(2, len(words)):
                if len(processword(words[j])) > 0:
                    corpus.append(processword(words[j]))

def getmetakeywords(allcontent, corpus):
    """
    This function uses regex matching to
    get metakeywords of a document from allcontent list and adds it to
    the corpus list

    Args:
        allcontent (list), corpus (list)

    Returns:
        None
    """
    for i in range(0, len(allcontent)):
        words = re.split("[, ]+", allcontent[i])
        if words[0] == "Meta":
            for j in range(3, len(words)):
                if len(processword(words[j])) > 0:
                    corpus.append(processword(words[j]))

def getcontent(allcontent, corpus):
    """
    This function uses regex matching to
    get content of a document from allcontent list and adds it to
    the corpus list

    Args:
        allcontent (list), corpus (list)

    Returns:
        None
    """
    contentlist = re.split(r'\s+', allcontent[-2])
    for i in range(0, len(contentlist)):
        if len(processword(contentlist[i])) > 0:
            corpus.append(processword(contentlist[i]))


def popelement(invertedlist, word):
    """
    This function removes a word from the inverted index.
    It is used in case of date, email etc. where the entire
    date or email are treated as a single entity.

    Args:
        invertedlist (dict), word(str)

    Returns:
        None
    """
    if word in invertedlist.keys():
        invertedlist[word][1] = invertedlist[word][1] - 1
        if invertedlist[word][1] > 0:
            invertedlist[word][0].pop()

def expressionfordateb(corpus, i):
    """
    This function is a helper to check if previous token
    is a digit.

    Args:
        corpus (list), i (int)

    Returns:
        bool
    """
    if i > 0 and corpus[i - 1][0].isdigit() is True and \
     (len(corpus[i - 1]) == 1 or corpus[i - 1][1].isdigit() is True):
        return True
    return False

def expressionfordatef(corpus, i):
    """
    This function is a helper to check if next token
    is a digit.

    Args:
        corpus (list), i (int)

    Returns:
        bool
    """
    if i < (len(corpus) - 1) and  corpus[i + 1].isdigit() is True:
        return True
    return False

def constructdate_expression(corpus, i, index):
    """
    This function processes the date into a
    single token.
    It uses expressionfordateb and expressionfordatef
    to check if previous and next tokens are digits.

    Args:
        corpus (list), i (int), index (list of dicts).

    Returns:
        date (str).
    """
    date = ""
    if expressionfordateb(corpus, i)  is True:
        popelement(index[0], corpus[i - 1])
        popelement(index[1], corpus[i - 1])
        popelement(index[2], corpus[i - 1])
        date = corpus[i - 1] + ' '
    date = date + corpus[i]
    if expressionfordatef(corpus, i)  is True:
        date = date + ' ' + corpus[i + 1]
    return date


def readcorpus(index):
    """
    This function is responsible
    for construction of indexes i1, i2 and i3.
    It reads the content of the document line-wise,
    calls functions previously defined to get title, metakeywords,
    content etc.
    It also performs stemming, stop-word removal and finally constructs
    indexes i1, i2, i3.

    Args:
        index (list of dicts).

    Returns:
        None.
    """
    for docid in range(0, 1001):
        file = open(PATH + "Document-" + str(docid) + ".txt", 'r', encoding="utf-8")
        allcontent = file.readlines()
        stoplist = getstopwords()
        corpus = []
        stemmer = PorterStemmer()
        getatitle(allcontent, corpus)
        getmetakeywords(allcontent, corpus)
        getcontent(allcontent, corpus)
        flagfordate = 0
        for i  in range(0, len(corpus)):
            if flagfordate == 1:
                flagfordate = 0
                continue
            word = corpus[i]
            if word in MONTH:
                if expressionfordateb(corpus, i) is True or expressionfordatef(corpus, i) is True:
                    word = constructdate_expression(corpus, i, index)
            increasecount(index[0], word, docid)
            if word in stoplist:
                continue
            increasecount(index[1], word, docid)
            increasecount(index[2], processword(stemmer.stem(word)), docid)

def buildindex4(invertedindex, index):
    """
    This function constructs index i4 from i3
    after removing the least frequent terms (those occuring < 2percent of the documents)

    Args:
        invertedindex (dict), index (dict).

    Returns:
        None
    """
    sortedbycount = sorted(invertedindex.items(), key=lambda x: x[1][1])
    startindex = math.floor((2*len(sortedbycount))/100) + 1
    for word, count in sortedbycount[startindex:]:
        index[word] = count
    return

def writetofile(invertedindex, filename):
    """
    This function writes the constructed inverted index into a file.

    Args:
        invertedindex (dict), filename(str)

    Returns:
        None
    """
    file = open(filename + '.txt', 'w', encoding='utf-8')
    for word in invertedindex.keys():
        file.write(word)
        file.write(' : ')
        for docid in invertedindex[word][0]:
            file.write(str(docid) + ' ')
        file.write('\n')

def writeindextofile(index):
    """
    This function writes indexes 0-4 into corresponding files.

    Args:
        index (list of dicts).

    Returns:
        None
    """
    writetofile(index[0], "invertedindex1")
    writetofile(index[1], "invertedindex2")
    writetofile(index[2], "invertedindex3")
    writetofile(index[3], "invertedindex4")

def createindexes():
    """
    This function is a wrapper function which
    calls previously defined functions to create indexes 1-4.

    Args:
        None

    Returns:
        index (list of dicts)
    """
    index = [{}, {}, {}, {}]
    readcorpus(index)
    buildindex4(index[2], index[3])
    writeindextofile(index)
    return index

def calculate(index):
    """
    This function prints the Number of Terms,
    Maximum Length of Postings List,
    Minimum Length of Postings List,
    Average Length of Postings List,
    Size of the file that stores the inverted index.
    for each of the indexes 1-4.

    Args:
        index (list of dicts).

    Returns:
        None
    """
    postingl1 = [len(x[0]) for x in index[0].values()]
    print("Number of terms in index i1 : " + str(len(postingl1)) + "\t")
    print("Maximum Length of Postings List : " + str(max(postingl1)) + "\t")
    print("Minimum Length of Postings List : " + str(min(postingl1)) + "\t")
    print("Average Length of Postings List : " + str(sum(postingl1) / float(len(postingl1))) + "\t")
    print("Size of the file that stores the index i1 : " + \
        str(os.stat('invertedindex1.txt').st_size) + " bytes\n")

    postingl2 = [len(x[0]) for x in index[1].values()]
    print("Number of terms in index i2 : " + str(len(postingl2)) + "\t")
    print("Maximum Length of Postings List : " + str(max(postingl2)) + "\t")
    print("Minimum Length of Postings List : " + str(min(postingl2)) + "\t")
    print("Average Length of Postings List : " + str(sum(postingl2) / float(len(postingl2))) + "\t")
    print("Size of the file that stores the index i2 : " + \
        str(os.stat('invertedindex2.txt').st_size) + " bytes\n")

    postingl3 = [len(x[0]) for x in index[2].values()]
    print("Number of terms in index i3 : " + str(len(postingl3)) + "\t")
    print("Maximum Length of Postings List : " + str(max(postingl3)) + "\t")
    print("Minimum Length of Postings List : " + str(min(postingl3)) + "\t")
    print("Average Length of Postings List : " + str(sum(postingl3) / float(len(postingl3))) + "\t")
    print("Size of the file that stores the index i3 : " + \
        str(os.stat('invertedindex3.txt').st_size) + " bytes\n")

    postingl4 = [len(x[0]) for x in index[3].values()]
    print("Number of terms in index i4 : " + str(len(postingl4)) + "\t")
    print("Maximum Length of Postings List : " + str(max(postingl4)) + "\t")
    print("Minimum Length of Postings List : " + str(min(postingl4)) + "\t")
    print("Average Length of Postings List : " + str(sum(postingl4) / float(len(postingl4))) + "\t")
    print("Size of the file that stores the index i4 : " + \
        str(os.stat('invertedindex4.txt').st_size) + " bytes\n")


if __name__ == "__main__":
    INDEX = createindexes()
    calculate(INDEX)
