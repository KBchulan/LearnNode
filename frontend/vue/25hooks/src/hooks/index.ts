// 我们今天所写的是把我们的图片转换为base64

import { onMounted } from "vue"

type Options = {
  el: string
}

export default function (option: Options): Promise<{ baseUrl: string }> {
  return new Promise((resolve) => {
    onMounted(() => {
      let img: HTMLImageElement = document.querySelector(option.el) as HTMLImageElement

      img.onload = () => {
        resolve({
          baseUrl: base(img)
        })
      }
    })

    const base = (el: HTMLImageElement) => {
      let canvas = document.createElement('canvas')
      let ctx = canvas.getContext('2d')

      canvas.width = el.width
      canvas.height = el.height

      ctx?.drawImage(el, 0, 0, el.width, el.height)
      return canvas.toDataURL('image/jpg')
    }
  })
}