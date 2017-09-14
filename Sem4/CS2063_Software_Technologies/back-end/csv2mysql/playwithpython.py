import mysql.connector
from mysql.connector import Error
 
 
def connect():
    """ Connect to MySQL database """
    try:
        conn = mysql.connector.connect(host='localhost',
                                       database='test',
                                       user='root',
                                       password='')
        if conn.is_connected():
            print('Connected to MySQL database')

        #print('querry created')
        cursor = conn.cursor()
        
        sql = """ LOAD DATA LOCAL INFILE '/Users/ybharath007/Desktop/stproject/json2csv-master/file.csv' INTO TABLE fb FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' ESCAPED BY '"' Lines terminated by '\n' IGNORE 1 LINES """
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
 
if __name__ == '__main__':
    connect()