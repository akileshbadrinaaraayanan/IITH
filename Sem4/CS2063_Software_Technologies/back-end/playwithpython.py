import csv
import MySQLdb

db = MySQLdb.connect(host="localhost", # The Host
                     user="root", # username
                     passwd="", # password
                     db="test") # name of the data base

sqlLoadData = 'LOAD DATA LOCAL INFILE "test.csv" INTO TABLE fb '   
sqlLoadData += 'FIELDS TERMINATED BY "," LINES TERMINATED BY "\n"'        
sqlLoadData += 'ENCLOSED BY """'
sqlLoadData += 'IGNORE 1 LINES \n'  
try:
	curs = db.cursor()
	curs.execute(sqlLoadData)
	resultSet = curs.fetchall()
	db.commit()
except StandardError, e:    
    #print e    
    db.rollback()
    db.close()