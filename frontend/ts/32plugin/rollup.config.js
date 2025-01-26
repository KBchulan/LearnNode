import ts from 'rollup-plugin-typescript2'
import path from 'path'
import { fileURLToPath } from 'url'
const metaUrl = fileURLToPath(import.meta.url)
const dirname = path.dirname(metaUrl)

export default {
    input: './src/index.ts',
    output: {
        file: path.resolve(dirname, './dist/index.js'),
    },
    plugins: [ts()]
}