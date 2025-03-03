import { createApp } from 'vue'

import App from './App.vue'
import './tailwindcss/index.css'

export const app = createApp(App)

app.mount('#app')

// npm install -D tailwindcss@3.4.1 postcss@latest autoprefixer@latest
// npx tailwindcss init -p
// 接着将tailwind.config.js中的content配置为['./index.html', './src/**/*.{vue,js,ts,jsx,tsx}']
// then create a tailwindcss/index.css file and add the following code:
/**
@tailwind base;
@tailwind components;
@tailwind utilities;
*/