<script setup lang="ts">
import { ref, onMounted, onUnmounted, reactive } from 'vue'

interface TextElement {
  id: number
  x: number
  y: number
  vx: number
  vy: number
  text: string
  size: number
  color: string
}

// 鼠标位置
const mousePosition = reactive({ x: 0, y: 0 })
const lastMousePosition = reactive({ x: 0, y: 0 })
const mouseSpeed = reactive({ x: 0, y: 0 })
const isMouseMoving = ref(false)

// 文字数组
const texts = ref<TextElement[]>([])
const textsContent = [
  '你好', '世界', 'Vue', 'TypeScript', '动画', 
  '物理', '效果', '鼠标', '吸引', '文字', 
  '惯性', '碰撞', '粒子', '交互', '设计',
  '前端', '开发', '有趣', '创意', '体验',
  'Vue', 'TypeScript', '动画', '物理', '效果',
  '鼠标', '吸引', '文字', '惯性', '碰撞',
  '粒子', '交互', '设计', '前端', '开发',
  '有趣', '创意', '体验', 'Vue', 'TypeScript',
  '动画', '物理', '效果', '鼠标', '吸引',
  '文字', '惯性', '碰撞', '粒子', '交互',
]

// 初始化文字元素
const initTexts = () => {
  const windowWidth = window.innerWidth
  const windowHeight = window.innerHeight
  
  texts.value = textsContent.map((text, index) => {
    const size = Math.random() * 14 + 16 // 16-30px
    
    return {
      id: index,
      x: Math.random() * (windowWidth - 100) + 50,
      y: Math.random() * (windowHeight - 100) + 50,
      vx: 0,
      vy: 0,
      text,
      size,
      color: `hsl(${Math.random() * 360}, 70%, 50%)`
    }
  })
}

// 更新文字位置
const updateTextPositions = () => {
  const attraction = 0.2 // 吸引力系数
  const friction = 0.92 // 摩擦力
  const minDistance = 30 // 最小距离，避免重叠
  const throwForce = 0.3 // 甩动力度系数
  
  // 计算鼠标速度
  mouseSpeed.x = mousePosition.x - lastMousePosition.x
  mouseSpeed.y = mousePosition.y - lastMousePosition.y
  
  lastMousePosition.x = mousePosition.x
  lastMousePosition.y = mousePosition.y
  
  isMouseMoving.value = Math.abs(mouseSpeed.x) > 8 || Math.abs(mouseSpeed.y) > 8

  texts.value.forEach(text => {
    // 计算文字到鼠标的距离
    const dx = mousePosition.x - text.x
    const dy = mousePosition.y - text.y
    const distance = Math.sqrt(dx * dx + dy * dy)
    
    if (distance < 200) { // 鼠标影响范围
      // 根据距离计算吸引力
      const force = (200 - distance) / 200
      
      if (isMouseMoving.value && distance < 80) {
        // 鼠标快速移动时，给文字施加抛出力
        text.vx += mouseSpeed.x * throwForce
        text.vy += mouseSpeed.y * throwForce
      } else {
        // 正常吸引
        text.vx += dx * force * attraction
        text.vy += dy * force * attraction
      }
    }
    
    // 文字间的碰撞检测和分离
    texts.value.forEach(otherText => {
      if (text.id !== otherText.id) {
        const odx = otherText.x - text.x
        const ody = otherText.y - text.y
        const odistance = Math.sqrt(odx * odx + ody * ody)
        const minSeparation = minDistance + (text.size + otherText.size) / 4
        
        if (odistance < minSeparation) {
          const angle = Math.atan2(ody, odx)
          const tx = text.x - Math.cos(angle) * minSeparation
          const ty = text.y - Math.sin(angle) * minSeparation
          
          const ax = (text.x - tx) * 0.05
          const ay = (text.y - ty) * 0.05
          
          text.vx -= ax
          text.vy -= ay
          otherText.vx += ax
          otherText.vy += ay
        }
      }
    })
    
    // 应用速度和摩擦力
    text.x += text.vx
    text.y += text.vy
    text.vx *= friction
    text.vy *= friction
    
    // 边界检查
    if (text.x < 0) text.x = 0
    if (text.x > window.innerWidth) text.x = window.innerWidth
    if (text.y < 0) text.y = 0
    if (text.y > window.innerHeight) text.y = window.innerHeight
  })
  
  requestAnimationFrame(updateTextPositions)
}

// 鼠标事件处理
const handleMouseMove = (e: MouseEvent) => {
  mousePosition.x = e.clientX
  mousePosition.y = e.clientY
}

onMounted(() => {
  initTexts()
  window.addEventListener('mousemove', handleMouseMove)
  requestAnimationFrame(updateTextPositions)
  
  // 初始化鼠标位置
  mousePosition.x = window.innerWidth / 2
  mousePosition.y = window.innerHeight / 2
  lastMousePosition.x = mousePosition.x
  lastMousePosition.y = mousePosition.y
})

onUnmounted(() => {
  window.removeEventListener('mousemove', handleMouseMove)
})
</script>

<template>
  <div class="hx-text-container">
    <div
      v-for="text in texts"
      :key="text.id"
      class="hx-text-particle"
      :style="{
        transform: `translate(${text.x}px, ${text.y}px)`,
        fontSize: `${text.size}px`,
        color: text.color
      }"
    >
      {{ text.text }}
    </div>
  </div>
</template>

<style lang="scss">
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

html, body {
  width: 100%;
  height: 100%;
  overflow: hidden;
  background-color: #111;
  font-family: Arial, sans-serif;
}

@include block('text-container') {
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
}

@include block('text-particle') {
  position: absolute;
  transform-origin: center center;
  will-change: transform;
  user-select: none;
  cursor: default;
  font-weight: bold;
  text-shadow: 0 0 3px rgba(0, 0, 0, 0.5);
  transition: transform 0.01s linear;
}
</style>
