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
from nltk.stem.wordnet import WordNetLemmatizer


english = {}
keywordsArray = []
stemmed = []
stemmer = WordNetLemmatizer()


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
    #query = "SELECT message FROM TrafficProcessedData"
		    
	try:
	    # connect to the database server
	    conn = mysql.connector.connect(host='localhost',
	                                   database='test',
	                                   user='root',
	                                   password='')
	        
	        # execute the query
	    cursor = conn.cursor()
	    for str in keywordsArray:
		    sentence = '"%s"' % (str)
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
	query = "SELECT processedMessage,processedComment FROM comments"
		    
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
	    	count = 0

	    	data = removeNonEnglish(row[0])
	    	dat = ""
	    	for word in data.split():
	    		dat = dat + stemmer.lemmatize(word) + " "
	    	data = dat
	    	wordDict = {}
	    	reverse = {}
	    	for word in data.split():
	    		wordDict[word] = 1

	    	data = removeNonEnglish(row[1])
	    	dat = ""
	    	for word in data.split():
	    		dat = dat + stemmer.lemmatize(word) + " "
	    	data = dat
	    	for word in data.split():
	    		if word in wordDict.keys():
	    			wordDict[word] = wordDict[word] + 1

	    	string = ""
	    	for key in sorted(wordDict,key=wordDict.get,reverse=True):
	    		if count < 6:
	    			#print wordDict[key]
	    			string = string + key + " "
	    			#print string
	    		else:
	    			break
	    		count = count + 1
	    	#print "\n"
	    	keywordsArray.append(string)

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