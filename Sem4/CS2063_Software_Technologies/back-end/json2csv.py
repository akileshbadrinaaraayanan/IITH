#!/usr/bin/env python

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

#logging.basicConfig(level=logging.DEBUG)

def removeStopWords(text):
    stop = stopwords.words("english")
    #stop = stop + ['IS']
    text = text.lower()
    text = ' '.join([word for word in text.split() if word not in stop])
    tagged_sent = pos_tag(text.split())
    text = ' '.join([word for word,pos in tagged_sent if pos != 'NNP'])
    #print "\n\n"
    return text
    #print "\n\n"

def addComments(z):
        tmpmsg = ""

        for i in z["data"]:
            str = ""
            if 'message' in i.keys():
                mes = i["message"]
                mes = mes.replace('"'," ")
                mes = mes.replace("'"," ")
                mes = mes.replace("."," ")
                mes = mes.replace("/"," ")
                mes = mes.replace("-"," ")
                mes = mes.replace("!"," ")
                tmpmsg = removeStopWords(mes).encode('ascii','replace')
                #print "\n"
                #print mes
                #print "\n"
                mes = '"%s"' % (removeStopWords(mes).encode('ascii','replace'))

                #print mes
            else:
                mes = '"null"'
                tmpmsg = "null"

            if 'comments' in i.keys():
                
                j = i["comments"]
                for k in j["data"]:
                    str += k["message"]
                str = str.replace('"'," ")
                str = str.replace("'"," ")
                str = str.replace(","," ")
                str = str.replace("!"," ")
                str = str.replace("-"," ")
                str = str.replace("."," ")
                tmpmsg = tmpmsg + " " + removeStopWords(str).encode('ascii','replace')
                tmpmsg = '"%s"' % (tmpmsg)

                #str = '"%s"'%(removeStopWords(str).encode('ascii','replace'))
                try:
                    # connect to the database server
                    conn = mysql.connector.connect(host='localhost',
                                                   database='test',
                                                   user='root',
                                                   password='')
                        
                                                   # execute the query
                    cursor = conn.cursor()

                    quotedStr = '"%s"' % (removeStopWords(str).encode('ascii','replace'))
                    str = '"%s"'%(str.encode('ascii','replace'))
                    command = 'insert into comments(commentsText,processedComment,processedMessage,processedData) values(%s,%s,%s,%s);' % (str,quotedStr,mes,tmpmsg)
                                                   #print command
                    cursor.execute(command)
                                                   
                                                   # accept the change
                    conn.commit()
                
                except Error as error:
                        print(error)

                finally:
    #print str
                        cursor.close()
                        conn.close()
            else:
                tmpmsg = '"%s"' % (tmpmsg)
                try:
                    # connect to the database server
                    conn = mysql.connector.connect(host='localhost',
                                                   database='test',
                                                   user='root',
                                                   password='')
                        
                                                   # execute the query
                    cursor = conn.cursor()
                    command = 'insert into comments(commentsText,processedComment,processedMessage,processedData) values (%s,%s,%s,%s);' % ('"null"','"null"',mes,tmpmsg)
                                                   #print command
                    cursor.execute(command)
                                                   
                                                   
                                                   # accept the change
                    conn.commit()
                
                except Error as error:
                    print(error)
                
                finally:
                    cursor.close()
                    conn.close()




class Json2Csv(object):
    """Process a JSON object to a CSV file"""
    collection = None

    def __init__(self, outline):
        self.rows = []

        if not type(outline) is dict:
            raise ValueError('You must pass in an outline for JSON2CSV to follow')
        elif 'map' not in outline or len(outline['map']) < 1:
            raise ValueError('You must specify at least one value for "map"')

        key_map = OrderedDict()
        for header, key in outline['map']:
            splits = key.split('.')
            splits = [int(s) if s.isdigit() else s for s in splits]
            key_map[header] = splits

        self.key_map = key_map
        if 'collection' in outline:
            self.collection = outline['collection']

    def load(self, json_file):
        proxies = {
        "https": "http://cs11m04:123@192.168.36.22:3128",
        "http":"http://cs11m04:123@192.168.36.22:3128",
        }
        
        r = requests.get('https://graph.facebook.com/BangaloreTrafficPolice/posts?fields=comments.limit(100).summary(true),likes.limit(1).summary(true),message,shares,from&access_token=769087063183565|4wOglpg1EKWkslJ153pPxu2SG9g',proxies=proxies)
        
        json.dump(r.json(),open("json_file.json",'w'),sort_keys=True, indent=4)
        
        
        #addProcessedText(r.json())
        addComments(r.json())
        
        self.process_each(r.json())
    
        for i in range(0,2):
            jsonData = r.json()
            paging = jsonData["paging"]
            nextLink = paging["next"]
            r = requests.get(nextLink,proxies=proxies)
            json.dump(r.json(),open("json_file.json",'w'),sort_keys=True, indent=4)
            #addProcessedText(r.json())
            addComments(r.json())
            self.process_each(r.json())
    
        r = requests.get('https://graph.facebook.com/HYDTP/posts?fields=comments.limit(100).summary(true),likes.limit(1).summary(true),message,shares,from&access_token=769087063183565|4wOglpg1EKWkslJ153pPxu2SG9g',proxies=proxies)
        json.dump(r.json(),open("json_file.json",'w'),sort_keys=True, indent=4)
        #addProcessedText(r.json())
        addComments(r.json())
        self.process_each(r.json())
        
        for i in range(0,2):
            jsonData = r.json()
            paging = jsonData["paging"]
            nextLink = paging["next"]
#sys.stdout.write(nextLink)
            r = requests.get(nextLink,proxies=proxies)
            json.dump(r.json(),open("json_file.json",'w'),sort_keys=True, indent=4)
            #addProcessedText(r.json())
            addComments(r.json())
            self.process_each(r.json())



        r = requests.get('https://graph.facebook.com/dtptraffic/posts?fields=comments.limit(100).summary(true),likes.limit(1).summary(true),message,shares,from&access_token=769087063183565|4wOglpg1EKWkslJ153pPxu2SG9g',proxies=proxies)
        json.dump(r.json(),open("json_file.json",'w'),sort_keys=True, indent=4)
        #addProcessedText(r.json())
        addComments(r.json())
        self.process_each(r.json())
        
        for i in range(0,2):
            jsonData = r.json()
            paging = jsonData["paging"]
            nextLink = paging["next"]
#sys.stdout.write(nextLink)
            r = requests.get(nextLink,proxies=proxies)
            json.dump(r.json(),open("json_file.json",'w'),sort_keys=True, indent=4)
            #addProcessedText(r.json())
            addComments(r.json())
            self.process_each(r.json())


        r = requests.get('https://graph.facebook.com/KolkataTrafficPolice/posts?fields=comments.limit(100).summary(true),likes.limit(1).summary(true),message,shares,from&access_token=769087063183565|4wOglpg1EKWkslJ153pPxu2SG9g',proxies=proxies)
        json.dump(r.json(),open("json_file.json",'w'),sort_keys=True, indent=4)
        #addProcessedText(r.json())
        addComments(r.json())
        self.process_each(r.json())
        
        for i in range(0,2):
            jsonData = r.json()
            paging = jsonData["paging"]
            nextLink = paging["next"]
#sys.stdout.write(nextLink)
            r = requests.get(nextLink,proxies=proxies)
            json.dump(r.json(),open("json_file.json",'w'),sort_keys=True, indent=4)
            #addProcessedText(r.json())
            addComments(r.json())
            self.process_each(r.json())



            
        
	    query = "DELETE FROM TrafficProcessedData;"
	    

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
        

        try:
            conn = mysql.connector.connect(host='localhost',
                                                   database='test',
                                                   user='root',
                                                   password='')
            if conn.is_connected():
                print('Connected to MySQL database')
    
    #print('querry created')
            cursor = conn.cursor()
        
            sql = """ LOAD DATA LOCAL INFILE '/Users/ybharath007/Desktop/stproject/json2csv-master/file.csv' INTO TABLE TrafficProcessedData FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' ESCAPED BY '"' Lines terminated by '\n' IGNORE 1 LINES; """
            #print sql;
            #print('Cursor created')
            cursor.execute(sql)
            #print('query executed')
            conn.commit()
    
        except Error as e:
            print(e)


        finally:
            conn.close()
            cursor.close()
        


    def process_each(self, data):
        """Process each item of a json-loaded dict
        """
        if self.collection and self.collection in data:
            data = data[self.collection]

        for d in data:
            logging.info(d)
            self.rows.append(self.process_row(d))

    def process_row(self, item):
        """Process a row of json data against the key map
        """
        row = {}

        for header, keys in self.key_map.items():
            try:
                row[header] = reduce(operator.getitem, keys, item)
            except (KeyError, TypeError):
                row[header] = None

        return row

    def write_csv(self, filename='output.csv'):
        """Write the processed rows to the given filename
        """
        if (len(self.rows) <= 0):
            raise AttributeError('No rows were loaded')
        with open(filename, 'wb+') as f:
            writer = csv.DictWriter(f, self.key_map.keys())
            writer.writeheader()
            writer.writerows(self.rows)



class MultiLineJson2Csv(Json2Csv):
    def load(self, json_file):
        self.process_each(json_file)

    def process_each(self, data, collection=None):
        """Load each line of an iterable collection (ie. file)"""
        for line in data:
            d = json.loads(line)
            if self.collection in data:
                data = data[self.collection]
            self.rows.append(self.process_row(d))


def init_parser():
    import argparse
    parser = argparse.ArgumentParser(description="Converts JSON to CSV")
    parser.add_argument('json_file', type=argparse.FileType('r'), help="Path to JSON data file to load")
    parser.add_argument('key_map', type=argparse.FileType('r'), help="File containing JSON key-mapping file to load")
    parser.add_argument('-e', '--each-line', action="store_true", default=False, 
                        help="Process each line of JSON file separately")
    parser.add_argument('-o', '--output-csv', type=str, default=None,
                        help="Path to csv file to output")

    return parser

if __name__ == '__main__':
    parser = init_parser()
    args = parser.parse_args()

    key_map = json.load(args.key_map)
    loader = None
    if args.each_line:
        loader = MultiLineJson2Csv(key_map)
    else:
        loader = Json2Csv(key_map)

    loader.load(args.json_file)

    outfile = args.output_csv
    if outfile is None:
        fileName, fileExtension = os.path.splitext(args.json_file.name)
        outfile = fileName + '.csv'

    loader.write_csv(filename=outfile)