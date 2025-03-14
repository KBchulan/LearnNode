# JMR项目部署指南

本文档详细记录了JMR项目（前端Vue.js + 后端FastAPI）的完整部署流程，包括环境准备、构建步骤、配置Nginx、设置系统服务以及常见问题排查。

## 1. 环境准备

### 系统要求

- 操作系统：Ubuntu/Debian系列Linux
- Node.js：用于前端构建
- Python 3.12+：用于后端服务
- Nginx：作为反向代理服务器

### 安装必要软件

```bash
# 安装Nginx
apt-get update && apt-get install -y nginx

# 确认Nginx安装成功
nginx -v
```

## 2. 前端部署

### 构建前端项目

```bash
# 进入项目根目录
cd /root/code/JMR

# 安装依赖
npm install

# 修改生产环境配置文件
VITE_API_BASE_URL=http://120.27.201.149:3000/api

# 构建项目（跳过类型检查）
npm run build-only
```

### 构建过程中的问题

在构建过程中可能会遇到TypeScript类型错误：

```
src/components/ComicCard.vue:24:28 - error TS2307: Cannot find module '@/types' or its corresponding type declarations.
```

这通常是由于路径解析问题导致的。如果只是部署，可以使用 `npm run build-only`命令跳过类型检查。

## 3. 后端部署

### 安装后端依赖

```bash
# 进入后端目录
cd /root/code/JMR/backend

# 安装依赖
pip install -r requirements.txt
```

### 创建系统服务

创建systemd服务文件以便后端可以作为服务运行：

```bash
# 创建服务文件
cat > /etc/systemd/system/jmr-backend.service << EOF
[Unit]
Description=JMR Backend Service
After=network.target

[Service]
User=root
WorkingDirectory=/root/code/JMR/backend
ExecStart=/usr/bin/bash -c "cd /root/code/JMR/backend && uvicorn app.main:app --host 0.0.0.0 --port 3000"
Restart=always
RestartSec=5
StandardOutput=syslog
StandardError=syslog
SyslogIdentifier=jmr-backend
Environment="BASE_URL=http://120.27.201.149:5173"

[Install]
WantedBy=multi-user.target
EOF

# 重新加载systemd配置
systemctl daemon-reload

# 启用并启动服务
systemctl enable jmr-backend
systemctl start jmr-backend

# 检查服务状态
systemctl status jmr-backend
```

## 4. Nginx配置

### 创建Nginx配置文件

```bash
# 创建配置文件
cat > /etc/nginx/sites-available/jmr-ip << EOF
server {
    listen 5173;
    server_name 120.27.201.149;

    # 前端静态文件
    location / {
        root /root/code/JMR/dist;
        index index.html;
        try_files \$uri \$uri/ /index.html;
    }

    # 后端API
    location /api {
        proxy_pass http://localhost:3000/api;
        proxy_http_version 1.1;
        proxy_set_header Upgrade \$http_upgrade;
        proxy_set_header Connection 'upgrade';
        proxy_set_header Host \$host;
        proxy_cache_bypass \$http_upgrade;
    }

    # WebSocket
    location /ws {
        proxy_pass http://localhost:3000/ws;
        proxy_http_version 1.1;
        proxy_set_header Upgrade \$http_upgrade;
        proxy_set_header Connection "Upgrade";
        proxy_set_header Host \$host;
    }

    # 静态资源
    location /static {
        proxy_pass http://localhost:3000/static;
        proxy_http_version 1.1;
        proxy_set_header Host \$host;
        proxy_cache_bypass \$http_upgrade;
    }

    # 健康检查
    location /health {
        proxy_pass http://localhost:3000/api/health;
        proxy_http_version 1.1;
        proxy_set_header Host \$host;
        proxy_cache_bypass \$http_upgrade;
    }

    # 下载API
    location /download {
        proxy_pass http://localhost:3000/download;
        proxy_http_version 1.1;
        proxy_set_header Host \$host;
        proxy_cache_bypass \$http_upgrade;
    }
}
EOF
```

### 启用配置并重启Nginx

```bash
# 创建符号链接启用配置
ln -s /etc/nginx/sites-available/jmr-ip /etc/nginx/sites-enabled/

# 检查配置语法
nginx -t

# 重启Nginx
systemctl restart nginx
```

## 5. 文件权限设置

由于Nginx默认以www-data用户运行，而项目文件属于root用户，需要调整权限：

```bash
# 修改前端文件权限
chmod -R 755 /root/code/JMR/dist

# 修改目录权限
chmod 755 /root
chmod 755 /root/code
chmod 755 /root/code/JMR
```

## 6. 防火墙配置

确保服务器防火墙允许访问必要的端口：

```bash
# 允许HTTP端口(80)
ufw allow 80/tcp

# 允许应用端口(5173)
ufw allow 5173/tcp

# 如果使用HTTPS，还需要允许443端口
ufw allow 443/tcp
```

## 7. 阿里云安全组配置

如果使用阿里云ECS，需要在阿里云控制台配置安全组规则：

1. 登录阿里云控制台
2. 进入ECS实例详情页
3. 点击"安全组" -> "配置规则"
4. 添加入方向规则：
   - 协议类型：TCP
   - 端口范围：5173/5173
   - 授权对象：0.0.0.0/0（允许所有IP访问）

## 8. 日志查看与问题排查

### 查看后端服务日志

```bash
# 查看后端服务的最近日志
journalctl -u jmr-backend -n 50

# 实时查看后端日志
journalctl -u jmr-backend -f
```

### 查看Nginx日志

```bash
# 查看访问日志
tail -n 100 /var/log/nginx/access.log

# 查看错误日志
tail -n 100 /var/log/nginx/error.log

# 实时查看错误日志
tail -f /var/log/nginx/error.log
```

### 常见问题及解决方案

1. **500 Internal Server Error**

   - 检查后端服务是否正常运行：`systemctl status jmr-backend`
   - 查看后端日志：`journalctl -u jmr-backend -n 50`
   - 检查Nginx错误日志：`tail -n 100 /var/log/nginx/error.log`
2. **Permission denied错误**

   - 检查文件权限：`ls -la /root/code/JMR/dist`
   - 确保Nginx用户可以访问文件：`chmod -R 755 /root/code/JMR/dist`
   - 确保目录路径可访问：`chmod 755 /root /root/code /root/code/JMR`
3. **服务无法启动**

   - 检查配置文件语法：`nginx -t`
   - 检查服务状态：`systemctl status jmr-backend`
   - 如果修改了服务文件，需要重新加载：`systemctl daemon-reload`
4. **前端无法连接后端API**

   - 检查环境变量配置：`.env.production`中的 `VITE_API_BASE_URL`
   - 确认Nginx配置中的代理设置正确
   - 检查后端服务是否正常运行

## 9. 访问应用

完成所有配置后，可以通过以下URL访问应用：

```
http://120.27.201.149:5173
```

## 10. 维护与更新

### 更新前端

```bash
# 拉取最新代码
git pull

# 重新构建
npm run build-only

# 无需重启Nginx，静态文件会自动更新
```

### 更新后端

```bash
# 拉取最新代码
git pull

# 安装新依赖（如有）
pip install -r requirements.txt

# 重启后端服务
systemctl restart jmr-backend
```

### 修改配置后重启服务

```bash
# 修改后端服务配置后
systemctl daemon-reload
systemctl restart jmr-backend

# 修改Nginx配置后
nginx -t
systemctl restart nginx
```
