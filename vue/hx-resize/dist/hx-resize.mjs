function i(e, s) {
  new ResizeObserver((n) => {
    s(n[0].contentRect);
  }).observe(e);
}
const r = (e) => {
  e.directive("resize", {
    mounted(s, t) {
      i(s, t.value);
    }
  });
};
i.install = r;
export {
  i as default
};
