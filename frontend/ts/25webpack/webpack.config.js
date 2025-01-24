const { Configuration } = require('webpack')
const path = require('node:path')
const HtmlWebpackPlugin = require('html-webpack-plugin')
const { VueLoaderPlugin } = require('vue-loader')
const MiniCssExtractPlugin = require('mini-css-extract-plugin')
/**
 * @type {Configuration}
 */
const config = {
    mode: "development",
    entry: './src/main.ts',     // 入口文件
    output: {
        path: path.resolve(__dirname, 'dist'),  // 生成目录
        filename: '[chunkhash].js',             // 打包之后的文件
        clean: true                             // 打包之前清空dist目录
    },
    stats: 'errors-only',
    plugins: [
        new HtmlWebpackPlugin({
            template: './index.html'
        }),
        new VueLoaderPlugin(),
        new MiniCssExtractPlugin(),
    ],      // webpack的插件都是class，都需要new
    module: {
        rules: [{
            test: /\.ts$/,      // 处理 .ts 和 .tsx 文件
            use: { loader: 'ts-loader', options: { appendTsSuffixTo: [/\.vue$/] } },     // 使用 ts-loader
        }, {
            test: /\.vue$/,
            use: "vue-loader"
        }, {
            test: /\.css$/,
            use: [MiniCssExtractPlugin.loader, 'css-loader']
        }, {
            test: /\.less$/,
            use: [MiniCssExtractPlugin.loader, 'css-loader', 'less-loader']
        }]
    },
    optimization: {
        splitChunks: {
            cacheGroups: {
                moment: {
                    test: /[\\/]node_modules[\\/]moment[\\/]/,
                    name: 'moment',
                    chunks: 'all'
                },
                common: {
                    name: 'common',
                    chunks: 'all',
                    minChunks: 2    // 至少被引用2次
                }
            }
        }
    }
}

module.exports = config