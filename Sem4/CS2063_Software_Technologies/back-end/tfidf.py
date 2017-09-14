import math
from textblob import TextBlob as tb
import unicodecsv as csv
import json
import operator
import os
from collections import OrderedDict
import logging
import requests
import sys
import mysql.connector
from mysql.connector import Error
from nltk.corpus import stopwords
from nltk.tag import pos_tag


english = {}

def tf(word, blob):
	#print(float(blob.words.count(word)) / float(len(blob.words)))
	return float(blob.words.count(word)) / float(len(blob.words))

def n_containing(word, bloblist):
	#print word
	#print sum(1 for blob in bloblist if word in blob)
	return sum(1 for blob in bloblist if word in blob)

def idf(word, bloblist):
	#print word
	#print len(bloblist) / (n_containing(word, bloblist))
	m = float(len(bloblist))
	n = float(n_containing(word, bloblist))
	return math.log( m/n )


def tfidf(word, blob, bloblist):
	#print word
	#print tf(word, blob)
	#print idf(word, bloblist)
	return tf(word, blob) * idf(word, bloblist)


bloblist = []

def emptyTable():
    query = "DELETE FROM Keywords;"
    
    try:
# connect to the database server
        conn = mysql.connector.connect(host='localhost',
                               database='test',
                               user='root',
                               password='')
    
    # execute the query
        cursor = conn.cursor()
        cursor.execute(query)
    
    # accept the change
        conn.commit()

    except Error as error:
        print(error)

    finally:
        cursor.close()
        conn.close() 	

def insertKeywords():
	query = "SELECT processedData FROM comments"
		    
	try:
	    # connect to the database server
	    conn = mysql.connector.connect(host='localhost',
	                                   database='test',
	                                   user='root',
	                                   password='')
	        
	        # execute the query
	    cursor = conn.cursor()
	    for i, blob in enumerate(bloblist):
		    #print("Top words in document {}".format(i + 1))
		    scores = {word: tfidf(word, blob, bloblist) for word in blob.words}
		    sorted_words = sorted(scores.items(), key=lambda x: x[1], reverse=True)
		    sentence = ''
		    for word,score in sorted_words[:6]:
		        #print("Word: {}, TF-IDF: {}".format(word, round(score, 5)))
		        sentence = sentence + word 
		        sentence = sentence + ' '
		    sentence = '"%s"' % (sentence)
		    command = "insert into Keywords(keywords) values (%s);" % (sentence)
		    cursor.execute(command)

	    conn.commit()        
	    # accept the change


	except Error as error:
	    print(error)

	finally:
		cursor.close()
		conn.close()        	

def loadDictionary():
	dictionaryFile = open('dictionary.txt')
	englishWords = {}
	for word in dictionaryFile.read().split('\n'):
		englishWords[word] = 1
	dictionaryFile.close()
	return englishWords

def removeNonEnglish(text):

    #stop = stop + ['IS']

    text = text.upper()

    text = ' '.join([word for word in text.split() if word in english])
    text = text.lower()
    #print(text)
    #print "\n\n"
    return text
    #print "\n\n"

def keywords():
	#bloblist = []
	#bloblist = [document1,document2,document3]
	count = 0
	query = "SELECT processedData FROM comments"
		    
	try:
	    # connect to the database server
	    conn = mysql.connector.connect(host='localhost',
	                                   database='test',
	                                   user='root',
	                                   password='')
	        
	        # execute the query
	    cursor = conn.cursor()
	    cursor.execute(query)
	    rows = cursor.fetchall()
	    for row in rows:
	    	#document = tb(row)%(row)
	    	#print("Hello")
	    	count = count + 1
	    	#data = '"%s"'(row[0])
	    	#data = "hello"
	    	data = removeNonEnglish(row[0])
	    	#print(count)
	    	#print(data)
	    	document = tb(data)
	    	bloblist.append(document)
	    	#cursor = row.fetchone()
	    # accept the change
	    insertKeywords()

	except Error as error:
	    print(error)

	finally:
		cursor.close()
		conn.close()        
	#bloblist = [document1, document2, document3]

english = loadDictionary()
if __name__  == '__main__':
	english = loadDictionary()
	#print(english)
	emptyTable()
	keywords()
