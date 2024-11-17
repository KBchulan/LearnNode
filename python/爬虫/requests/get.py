import requests

query = input("content:")

url = f"https://www.sogou.com/web?query={query}"

headers = {
    "User-Agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/130.0.0.0 Safari/537.36"
}

try:
    resp = requests.get(url, headers=headers)
    resp.encoding = 'utf-8'

    print(f'状态码: {resp.status_code}')
    print(f'响应头: {resp.headers}')
    print(f'文本内容: {resp.text}')
    print(f'文本二进制内容: {resp.content}')
    
except Exception as e:
    print(f"发生错误: {e}")

finally:
    if 'resp' in locals():
        resp.close()
        