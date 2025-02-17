import MySQLdb

# 打开数据库连接
db = MySQLdb.connect("localhost", "root", "root", "test", charset='utf8' )

# 使用cursor()方法获取操作游标
cursor = db.cursor()

# # 如果数据表已经存在使用 execute() 方法删除表。
# cursor.execute("DROP TABLE IF EXISTS EMPLOYEE")
#
# # 创建数据表SQL语句
# sql = """CREATE TABLE EMPLOYEE (
#          FIRST_NAME  CHAR(20) NOT NULL,
#          LAST_NAME  CHAR(20),
#          AGE INT,
#          SEX CHAR(1),
#          INCOME FLOAT )"""
#
# cursor.execute(sql)

# # SQL 插入语句
# # sql = """INSERT INTO EMPLOYEE(FIRST_NAME,
# #          LAST_NAME, AGE, SEX, INCOME)
# #          VALUES ('Mac', 'Mohan', 20, 'M', 2000)"""
# # try:
# #    # 执行sql语句
# #    cursor.execute(sql)
# #    # 提交到数据库执行
# #    db.commit()
# # except:
# #    # Rollback in case there is any error
# #    db.rollback()

# SQL 查询语句
# sql = "SELECT * FROM EMPLOYEE \
#        WHERE INCOME > %s" % (1000)
# try:
#    # 执行SQL语句
#    cursor.execute(sql)
#    # 获取所有记录列表
#    results = cursor.fetchall()
#    for row in results:
#       fname = row[0]
#       lname = row[1]
#       age = row[2]
#       sex = row[3]
#       income = row[4]
#       # 打印结果
#       print("fname=%s,lname=%s,age=%s,sex=%s,income=%s" % \
#              (fname, lname, age, sex, income ))
# except:
#    print ("Error: unable to fecth data")

# # SQL 更新语句
# sql = "UPDATE EMPLOYEE SET AGE = AGE + 1 WHERE SEX = '%c'" % ('M')
# try:
#    # 执行SQL语句
#    cursor.execute(sql)
#    # 提交到数据库执行
#    db.commit()
# except:
#    # 发生错误时回滚
#    db.rollback()

# SQL 删除语句
sql = "DELETE FROM EMPLOYEE WHERE AGE > %s" % (20)
try:
   # 执行SQL语句
   cursor.execute(sql)
   # 提交修改
   db.commit()
except:
   # 发生错误时回滚
   db.rollback()


# 关闭数据库连接
db.close()