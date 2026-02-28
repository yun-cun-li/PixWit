<template>
  <Layout>
    <template #doc-before>
      <Breadcrumb />
    </template>
  </Layout>
</template>

<script setup lang="ts">
import DefaultTheme from 'vitepress/theme'
import Breadcrumb from './components/Breadcrumb.vue'

const { Layout } = DefaultTheme
</script>

