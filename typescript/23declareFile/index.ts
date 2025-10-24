// 声明文件也就是我们的index.d.ts

// 如下两个导入，第一个的axios是有声明文件的，故不报错(可以ctrl点进去)
import axios from "axios"

// 这个没有声明文件，我们就需要手写声明文件或者尝试npm i --save-dev @types/express这种
import express from 'express'

axios.get('localhost')