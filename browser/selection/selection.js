const css = "::selection { background-color: #ff8000 !important; color: #0c0d0f !important; }";
const id = "papaya-selection";

function place() {
  const parent = document.head || document.documentElement;
  if (!parent) return;
  let style = document.getElementById(id);
  if (!style) {
    style = document.createElement("style");
    style.id = id;
    style.textContent = css;
  }
  if (style.parentNode !== parent || parent.lastElementChild !== style) {
    parent.appendChild(style);
  }
}

const watched = new WeakSet();

function watch(node) {
  if (!node || watched.has(node)) return;
  watched.add(node);
  new MutationObserver(place).observe(node, { childList: true });
}

place();
watch(document.documentElement);
watch(document.head);
document.addEventListener("DOMContentLoaded", () => {
  place();
  watch(document.head);
});
