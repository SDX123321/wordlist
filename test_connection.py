import pymysql.cursors
 
connection = pymysql.connect(host='localhost',
                             user='root',
                             password='12345678Aa',
                             database='wordlist',
                             cursorclass=pymysql.cursors.DictCursor)