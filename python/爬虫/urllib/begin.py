"""
resp.read() - 读取响应内容
resp.status - 获取HTTP状态码
resp.headers - 获取响应头
resp.url - 获取URL
resp.close() - 关闭连接
resp.getcode() - 获取状态码的另一种方式
resp.info() - 获取响应头信息的另一种方式
"""

from urllib.request import urlopen

url = "http://www.baidu.com"

resp = urlopen(url)

with open("mybaidu.html", mode = "w") as file:
    file.write(resp.read().decode("utf-8"))

resp.close()
print("over!")
file.close()