import DefaultTheme from 'vitepress/theme'
import Hero from './components/Hero.vue'
import Features from './components/Features.vue'
import './style.css'

export default {
  extends: DefaultTheme,
  enhanceApp({ app }) {
    app.component('Hero', Hero)
    app.component('Features', Features)
  }
}

