import DefaultTheme from 'vitepress/theme'
import './style.css'
import Hero from './components/Hero.vue'
import Features from './components/Features.vue'
import Download from './components/Download.vue'

export default {
  extends: DefaultTheme,
  enhanceApp({ app }) {
    app.component('Hero', Hero)
    app.component('Features', Features)
    app.component('Download', Download)
  }
}

