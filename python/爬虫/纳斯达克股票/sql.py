import json
import mysql.connector

from typing import List, Dict
from datetime import datetime
from mysql.connector import Error

class DatabaseManager:
    def __init__(self, host="localhost", user="root", password="whx051021", database="pywork"):
        self.connection_params = {
            "host": host,
            "user": user,
            "password": password,
            "database": database
        }
        self.create_table()
        
    def connect(self):
        try:
            connection = mysql.connector.connect(**self.connection_params)
            return connection
        
        except Error as e:
            print(f"数据库连接错误: {e}")
            return None

    def create_table(self) -> bool:
        connection = self.connect()
        if not connection:
            return False

        try:
            cursor = connection.cursor()
            
            create_table_query = """
            CREATE TABLE IF NOT EXISTS stock_prices (
                id INT AUTO_INCREMENT PRIMARY KEY,
                date DATE NOT NULL,
                open DECIMAL(10,2),
                high DECIMAL(10,2),
                low DECIMAL(10,2),
                close DECIMAL(10,2),
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                UNIQUE KEY unique_date (date)
            )
            """
            
            cursor.execute(create_table_query)
            connection.commit()
            print("数据表创建成功或已存在")
            return True

        except Error as e:
            print(f"创建表错误: {e}")
            return False

        finally:
            if connection.is_connected():
                cursor.close()
                connection.close()

    def save_stock_data(self, daily_data: List[Dict]) -> bool:
        connection = self.connect()
        if not connection:
            return False

        try:
            cursor = connection.cursor()

            insert_query = """
                INSERT INTO stock_prices (date, open, high, low, close)
                VALUES (%s, %s, %s, %s, %s)
                ON DUPLICATE KEY UPDATE
                open = VALUES(open),
                high = VALUES(high),
                low = VALUES(low),
                close = VALUES(close)
            """

            values = [
                (
                    datetime.strptime(item['date'], '%Y-%m-%d').date(),
                    item['open'],
                    item['high'],
                    item['low'],
                    item['close']
                )
                for item in daily_data
            ]

            cursor.executemany(insert_query, values)
            connection.commit()

            print(f"成功插入 {cursor.rowcount} 条记录")
            return True

        except Error as e:
            print(f"数据库错误: {e}")
            return False

        finally:
            if connection.is_connected():
                cursor.close()
                connection.close()

def save_stock_data_from_json(json_file_path: str) -> bool:
    try:
        with open(json_file_path, 'r', encoding='utf-8') as file:
            daily_data = json.load(file)

        db_manager = DatabaseManager()

        return db_manager.save_stock_data(daily_data)

    except Exception as e:
        print(f"错误: {e}")
        return False

if __name__ == "__main__":
    save_stock_data_from_json("./data.json")