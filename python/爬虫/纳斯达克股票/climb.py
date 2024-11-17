import json
import time
import random
import requests

from datetime import datetime, timedelta

def get_stock_data(symbol, retries=3, delay=1):
    end_date = datetime.now()
    start_date = end_date - timedelta(days=100)
    
    url = f"https://query1.finance.yahoo.com/v8/finance/chart/{symbol}"
    
    params = {
        "period1": int(start_date.timestamp()),
        "period2": int(end_date.timestamp()),
        "interval": "1d"
    }
    
    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36",
        "Accept": "application/json",
        "Referer": "https://finance.yahoo.com"
    }

    for attempt in range(retries):
        try:
            # 防止访问过于频繁
            time.sleep(delay + random.uniform(0.5, 1.5))
            
            response = requests.get(url, params=params, headers=headers)
            response.raise_for_status()
            
            data = response.json()
            
            if not data.get("chart") or not data["chart"]["result"]:
                raise ValueError("未获取到数据")
                
            result = data["chart"]["result"][0]
            timestamps = result["timestamp"]
            quote = result["indicators"]["quote"][0]
            
            daily_data = []
            for i in range(len(timestamps)):
                daily_info = {
                    "date": datetime.fromtimestamp(timestamps[i]).strftime("%Y-%m-%d"),
                    "open": round(quote["open"][i], 2) if quote["open"][i] else None,
                    "high": round(quote["high"][i], 2) if quote["high"][i] else None,
                    "low": round(quote["low"][i], 2) if quote["low"][i] else None,
                    "close": round(quote["close"][i], 2) if quote["close"][i] else None
                }
                daily_data.append(daily_info)
            
            with open("data.json", "w", encoding="utf-8") as f:
                json.dump(daily_data, f, ensure_ascii=False, indent=4)
                
            print(f"成功获取 {symbol} 的数据并保存到 data.json")
            return True
            
        except requests.RequestException as e:
            print(f"请求错误 (尝试 {attempt + 1}/{retries}): {str(e)}")
            
        except ValueError as e:
            print(f"数据解析错误 (尝试 {attempt + 1}/{retries}): {str(e)}")
            
        except Exception as e:
            print(f"未知错误 (尝试 {attempt + 1}/{retries}): {str(e)}")
            
        if attempt < retries - 1:
            delay *= 2
            print(f"等待 {delay} 秒后重试...")
            time.sleep(delay)
            
    print("达到最大重试次数，获取数据失败")
    return False

if __name__ == "__main__":
    symbol = "NDAQ"
    get_stock_data(symbol)
