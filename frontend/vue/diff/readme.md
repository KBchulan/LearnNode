### 为什么要有虚拟DOM

    DOM的本质是通过js生成的AST抽象节点树，那在vue里面就是把vue语法转换为用js描述的AST，注意此时的js生成的是我们vue所指定的这些elements，并非所有的，那么为什么呢

```
'align title lang translate dir hidden inert accessKey draggable spellcheck autocapitalize editContext contentEditable enterKeyHint isContentEditable inputMode virtualKeyboardPolicy offsetParent offsetTop offsetLeft offsetWidth offsetHeight popover innerText outerText writingSuggestions onbeforexrselect onabort onbeforeinput onbeforematch onbeforetoggle onblur oncancel oncanplay oncanplaythrough onchange onclick onclose oncontentvisibilityautostatechange oncontextlost oncontextmenu oncontextrestored oncuechange ondblclick ondrag ondragend ondragenter ondragleave ondragover ondragstart ondrop ondurationchange onemptied onended onerror onfocus onformdata oninput oninvalid onkeydown onkeypress onkeyup onload onloadeddata onloadedmetadata onloadstart onmousedown onmouseenter onmouseleave onmousemove onmouseout onmouseover onmouseup onmousewheel onpause onplay onplaying onprogress onratechange onreset onresize onscroll onsecuritypolicyviolation onseeked onseeking onselect onslotchange onstalled onsubmit onsuspend ontimeupdate ontoggle onvolumechange onwaiting onwebkitanimationend onwebkitanimationiteration onwebkitanimationstart onwebkittransitionend onwheel onauxclick ongotpointercapture onlostpointercapture onpointerdown onpointermove onpointerrawupdate onpointerup onpointercancel onpointerover onpointerout onpointerenter onpointerleave onselectstart onselectionchange onanimationend onanimationiteration onanimationstart ontransitionrun ontransitionstart ontransitionend ontransitioncancel oncopy oncut onpaste dataset nonce autofocus tabIndex style attributeStyleMap attachInternals blur click focus hidePopover showPopover togglePopover onscrollend onscrollsnapchange onscrollsnapchanging namespaceURI prefix localName tagName id className classList slot attributes shadowRoot part assignedSlot innerHTML outerHTML scrollTop scrollLeft scrollWidth scrollHeight clientTop clientLeft clientWidth clientHeight onbeforecopy onbeforecut onbeforepaste onsearch elementTiming onfullscreenchange onfullscreenerror onwebkitfullscreenchange onwebkitfullscreenerror role ariaAtomic ariaAutoComplete ariaBusy ariaBrailleLabel ariaBrailleRoleDescription ariaChecked ariaColCount ariaColIndex ariaColSpan ariaCurrent ariaDescription ariaDisabled ariaExpanded ariaHasPopup ariaHidden ariaInvalid ariaKeyShortcuts ariaLabel ariaLevel ariaLive ariaModal ariaMultiLine ariaMultiSelectable ariaOrientation ariaPlaceholder ariaPosInSet ariaPressed ariaReadOnly ariaRelevant ariaRequired ariaRoleDescription ariaRowCount ariaRowIndex ariaRowSpan ariaSelected ariaSetSize ariaSort ariaValueMax ariaValueMin ariaValueNow ariaValueText children firstElementChild lastElementChild childElementCount previousElementSibling nextElementSibling after animate append attachShadow before checkVisibility closest computedStyleMap getAnimations getAttribute getAttributeNS getAttributeNames getAttributeNode getAttributeNodeNS getBoundingClientRect getClientRects getElementsByClassName getElementsByTagName getElementsByTagNameNS getHTML hasAttribute hasAttributeNS hasAttributes hasPointerCapture insertAdjacentElement insertAdjacentHTML insertAdjacentText matches prepend querySelector querySelectorAll releasePointerCapture remove removeAttribute removeAttributeNS removeAttributeNode replaceChildren replaceWith requestFullscreen requestPointerLock scroll scrollBy scrollIntoView scrollIntoViewIfNeeded scrollTo setAttribute setAttributeNS setAttributeNode setAttributeNodeNS setHTMLUnsafe setPointerCapture toggleAttribute webkitMatchesSelector webkitRequestFullScreen webkitRequestFullscreen currentCSSZoom ariaColIndexText ariaRowIndexText nodeType nodeName baseURI isConnected ownerDocument parentNode parentElement childNodes firstChild lastChild previousSibling nextSibling nodeValue textContent ELEMENT_NODE ATTRIBUTE_NODE TEXT_NODE CDATA_SECTION_NODE ENTITY_REFERENCE_NODE ENTITY_NODE PROCESSING_INSTRUCTION_NODE COMMENT_NODE DOCUMENT_NODE DOCUMENT_TYPE_NODE DOCUMENT_FRAGMENT_NODE NOTATION_NODE DOCUMENT_POSITION_DISCONNECTED DOCUMENT_POSITION_PRECEDING DOCUMENT_POSITION_FOLLOWING DOCUMENT_POSITION_CONTAINS DOCUMENT_POSITION_CONTAINED_BY DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC appendChild cloneNode compareDocumentPosition contains getRootNode hasChildNodes insertBefore isDefaultNamespace isEqualNode isSameNode lookupNamespaceURI lookupPrefix normalize removeChild replaceChild addEventListener dispatchEvent removeEventListener '
```

    我们在浏览器控制台尝试打印DOM的所有key，会知道一个DOM其实是有非常多属性的，那么采用一个对象的方式是比较愚蠢的，那么如果我们需要的属性被单独描述为树呢，是不是在性能上就比较良好了，也就是我们虚拟DOM的由来--js描述的AST。

```
<!-- 真实 DOM -->
<div class="user">
  <span>张三</span>
</div>

<!-- 对应的 VNode（简化表示） -->
{
  type: 'div',
  props: { class: 'user' },
  children: [
    {
      type: 'span',
      props: {},
      children: '张三'
    }
  ]
}
```

### Diff算法

    请参考src/APP.vue的内容
