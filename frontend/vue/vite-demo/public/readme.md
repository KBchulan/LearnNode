# Public 目录说明

## 目录结构

public/
  ├── favicon.ico        # 网站图标
  ├── images/            # 图片资源
  ├── fonts/            # 字体文件
  ├── robots.txt        # 搜索引擎爬虫配置
  └── manifest.json     # PWA 配置文件

## 主要特点

1. **不会被编译**
   - 放在 public 中的文件不会被 webpack/vite 处理
   - 会原样复制到构建输出目录（通常是 dist）

2. **使用场景**
   - 需要指定固定 URL 的资源
   - 较大的文件（>4kb），避免转为 base64
   - 某些特殊的第三方库可能需要通过 <script> 标签直接引入

3. **访问方式**
   - 在代码中可以通过 `/` 绝对路径直接访问
   - 示例：`<img src="/images/logo.png">`
   - 在 index.html 中使用 `%PUBLIC_URL%` 引用

4. **注意事项**
   - 尽量不要把可以被打包的资源放在 public 目录
   - 建议小型资源使用 src/assets 目录，以便优化和处理
   - 确保引用路径正确，使用绝对路径

## 最佳实践

1. **图片资源**
   - logo、favicon 等固定图片
   - 大型图片资源（如背景图）

2. **字体文件**
   - 自定义字体文件
   - 图标字体文件

3. **配置文件**
   - robots.txt
   - manifest.json
   - sitemap.xml

4. **第三方资源**
   - CDN 无法满足需求的第三方库
   - 需要在构建过程中保持原样的资源