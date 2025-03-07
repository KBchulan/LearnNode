
class Btn extends HTMLElement {
  constructor() {
    super()

    const shallowDom = this.attachShadow({
      mode: "open"
    })

    // 这里面的样式是会被隔离的，不会影响外面的
    this.p = this.h('p')

    this.p.innerText = 'A test text'
    this.p.setAttribute('style', 'width:200px; height:200px; border:1px solid white; color: white;')

    this.template = this.h('template')
    this.template.innerHTML = `
      <style>
        div{
          width: 200px;
          height: 200px;
          border: 1px solid white;
          color: white;
        }
      </style>
      <div>
        我是一个模板
      </div>
    `
    shallowDom.appendChild(this.p)
    shallowDom.appendChild(this.template.content.cloneNode(true))
  }

  h(el) {
    return document.createElement(el)
  }
}

window.customElements.define('hx-btn', Btn)