import MySQLdb
import os


def db_sample():
    """ 接続サンプル """

    # 接続する
    con = MySQLdb.connect(
        user='kumeta',
        passwd='password',
        host='192.168.10.108',
        db='plantTest',
        charset="utf8")

    # カーソルを取得する
    cur = con.cursor(MySQLdb.cursors.DictCursor)

    # クエリを実行する
    sql = "select * from plantData"
    cur.execute(sql)

    # 実行結果をすべて取得する
    rows = cur.fetchall()

    # 一行ずつ表示する
    for row in rows:
        print(row)

    cur.close()
    con.close()


def db_request(sql):
    # 接続する
    print("接続開始")
    con = MySQLdb.connect(
        user='kumeta',
        passwd='password',
        host='192.168.10.108',
        db='plantTest',
        charset="utf8")

    # カーソルを取得する
    cur = con.cursor(MySQLdb.cursors.DictCursor)
    print(sql)
    # クエリを実行する
    cur.execute(sql)

    # 実行結果をすべて取得する
    rows = cur.fetchall()

    cur.close()
    con.commit()
    con.close()

    return rows


if __name__ == "__main__":
    # print(db_request("select * from plantData"))
    # print(db_request('INSERT INTO plantData (id,csvpath,appname) VALUES (1,"/test.csv","plantTest")'))
    print(os.path.dirname(os.path.abspath(__file__)))
