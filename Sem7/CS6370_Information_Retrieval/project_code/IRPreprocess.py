
# coding: utf-8
import sklearn, nltk
from nltk.tokenize import *
from nltk.corpus import stopwords
import json
import numpy as np, sklearn, scipy, gensim, logging
import logging
logging.basicConfig(format='%(asctime)s : %(levelname)s : %(message)s',    level=logging.INFO)
from gensim.models import word2vec
import matplotlib.pyplot as plt
from collections import Counter
import math

rawcorpus = []
k = 0
with open('sample-200K.jsonl', 'r') as jsonfile:
    content = json.load(jsonfile)
    for row in content:
        if k >= 2000:
            break
        rawcorpus.append((row["content"],row["title"]))
        k = k + 1


contentList = []
titleList = []
k = 0
tokenizerCorpus = PunktSentenceTokenizer()
#tokenizerSentence = RegexpTokenizer('\w+|\$[\d\.]+|\S+')
tokenizerSentence = RegexpTokenizer(r'\w+')

def preprocess(word):
    word.strip('\n ,!?-%::;()&-')
    word = word.lower()
    return word

def constructTokenList(article):
    """
    Construct token for the article
    """
    sentenceList = []
    tokens = tokenizerCorpus.tokenize(article) # Construct sentences from the article
    for sentences in tokens:
        """
        Tokenization and preprocessing 
        """
        wordToken = tokenizerSentence.tokenize(sentences) # tokenize sentences
        wordToken = [w.lower() for w in wordToken]
        wordToken = [w.strip('\n ,!?-%::;()&-') for w in wordToken]
        sentenceList.append(wordToken)
    return sentenceList

for row in rawcorpus:
    """
    The variables stores the article and the title
    """
    contentList.append(constructTokenList(row[0]))
    titleList.append(constructTokenList(row[1]))

numUnk = 0 # number of unknowns in the article.
def removeWords(contentList):
    """
    The additional preprocessing mentioned in the report 
    is implemented such as taking the sentences, 
    """
    newContentList = []
    word_dict = {}
    removeChar = ["\'", "-", "&", "’", "—"]
    for item in contentList:
        sentenceList = []
        for sentence in item:
            for word in sentence:
                if word not in word_dict.keys():
                    word_dict[word] = 0
                word_dict[word] = word_dict[word] + 1
        l = []
        totalWord = 0
        unknownWord = 0
        for sentence in item:
            f = []
            for word in sentence:
                if len(word.strip(' \n')) == 0:
                    continue
                if word[0] in removeChar:
                    continue
                if word_dict[word] <= numUnk:
                    f.append('<UNK>')
                    unknownWord += 1
                else:
#                     if word.isdigit() == True:
#                         word = '<NUMBER>'
                    f.append(word)
                totalWord += 1
            #print((unknownWord * 1.0) / totalWord)
            if len(f) == 0:
                continue
            l.append(f)
        if ((unknownWord * 1.0) / totalWord) >= 0.2:
            continue
        newContentList.append(l)
    return newContentList

contentList = removeWords(contentList)

def cosineSimilarity(a, b):
    """
    Calculate the cosine similarity 
    between 2 lists.
    """
    a_vals = Counter(a)  
    b_vals = Counter(b)
    words  = list(a_vals.keys() | b_vals.keys())
    a_vect = [a_vals.get(word, 0) for word in words]
    b_vect = [b_vals.get(word, 0) for word in words]     
    len_a  = sum(av*av for av in a_vect) ** 0.5      
    len_b  = sum(bv*bv for bv in b_vect) ** 0.5   
    dot = sum(av*bv for av,bv in zip(a_vect, b_vect))    
    cosine = dot / (len_a * len_b)                          
    return cosine

k = 0
newContentList= []

for item in contentList:
    """
    Construct the refined article list 
    and remove the articles with more than
    20% unknown labels. 
    """
    titlesim = titleList[k][0]
    sortList = []
    for sentence in item:
        sortList.append((cosineSimilarity(titlesim, sentence), sentence))
    sortList = sorted(sortList, reverse=True)
    f = []
    for i in range(0, min(len(item), 5)):
        f.append(sortList[i][1])
    newContentList.append(f)
    
contentList = newContentList    
l = []
k = 0

k = 0
print(len(contentList))
for item in contentList:
    if k >= 1000:
        break
    k += 1


"""
The following is the word2vec implementation.
word2Vec model is trained on the corpus 
"""

#Uncomment the follwing for word2vec model.

"""
sentence = []
k = 0
for item in contentList:
    sentence += item
    if k > 10:
        break
    k += 1
print(sentence)
num_features = 5    # Word vector dimensionality                      
min_word_count = 40   # Minimum word count                        
num_workers = 4       # Number of threads to run in parallel
context = 10          # Context window size                                                                                    
downsampling = 1e-3   # Downsample setting for frequent words

# Initialize and train the model (this will take some time)
print ("Training  Word2Vec model...")
model = word2vec.Word2Vec(sentence, workers=num_workers,             size = num_features, min_count = 1,             window = context, sample = downsampling)
print("Trained Word2Vec Model")


It construct the numpy array for the article using word2vec.

articlelist = []
for article in contentList:
    sentencelist = []
    for  sentences in article[1]:
        f = []
        for word in sentences:
            l = model[word].tolist()
            f.append(l)
        sentencelist.append(f)
    articlelist.append(sentencelist)
numpyArray = np.asarray(articlelist)
"""


"""
The following files are used for training and testing.
"""
"""
file = open('sentence_1000_NO_UNK.txt', 'w')
dicc = {}
k = 0
for item in contentList:
    if k >= 1000:
        break
    k += 1
    for sentence in item:
        str = ""
        for word in sentence:
            dicc[word] = 1
            str += word.strip(' \n\"')
            str += ' '
        file.write(str)
        file.write('\n')
file.close()
print(len(dicc.keys()))
"""
file = open('sentence_500_full_article.txt', 'w')
dicc = {}
k = 0
for index in range(0, 501):
    for item in constructTokenList(rawcorpus[index][0]):
        str = ""
        for word in item:
            if word.isdigit() == True:
                continue
#             if len(word) <= 1:
#                 continue
            dicc[word] = 1
            str += word.strip(' \n\"')
            str += ' '
        file.write(str)
        file.write('\n')
file.close()
print(len(dicc.keys()))
