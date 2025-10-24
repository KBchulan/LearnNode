import express from 'express'
import mongoose from 'mongoose'
import cors from 'cors'
import dotenv from 'dotenv'
import novelRoutes from './routes/novel'
import crawlerRoutes from './routes/crawler'

// 加载环境变量
dotenv.config()

const app = express()

// 中间件
app.use(cors())
app.use(express.json())

// 路由
app.use('/api/novels', novelRoutes)
app.use('/api/crawler', crawlerRoutes)

// 数据库连接
mongoose.connect(process.env.MONGODB_URI!)
    .then(() => console.log('Connected to MongoDB'))
    .catch(err => console.error('MongoDB connection error:', err))

// 错误处理
app.use((err: any, req: express.Request, res: express.Response, next: express.NextFunction) => {
    console.error(err.stack)
    res.status(500).json({ message: 'Internal Server Error' })
})

const PORT = process.env.PORT || 3000
app.listen(PORT, () => {
    console.log(`Server is running on port ${PORT}`)
})

export default app 