import DefaultTheme from 'vitepress/theme'
import './style.css'
import Layout from './Layout.vue'
import Hero from './components/Hero.vue'
import Features from './components/Features.vue'
import Download from './components/Download.vue'
import Breadcrumb from './components/Breadcrumb.vue'
import Community from './components/Community.vue'

export default {
  extends: DefaultTheme,
  Layout,
  enhanceApp({ app }) {
    app.component('Hero', Hero)
    app.component('Features', Features)
    app.component('Download', Download)
    app.component('Breadcrumb', Breadcrumb)
    app.component('Community', Community)
  }
}

