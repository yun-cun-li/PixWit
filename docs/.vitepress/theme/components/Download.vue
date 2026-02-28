<template>
  <div class="download-section">
    <div class="download-cards">
      <div class="download-card recommended">
        <span class="download-badge">💚 推荐</span>
        <h3 class="download-title">绿色便携版</h3>
        <p class="download-desc">
          无需安装，解压即用<br>
          无杀毒软件拦截<br>
        </p>
        <button class="download-btn" @click="downloadPortable">
          📦 下载便携版
        </button>
        <button class="download-btn pan-btn" @click="downloadPortablePan">
          📁 网盘下载
        </button>

      </div>

      <div class="download-card">
        <h3 class="download-title">安装程序版</h3>
        <p class="download-desc">
          标准安装程序<br>
          支持开机自启<br>
          系统集成更好
        </p>
        <button class="download-btn" @click="downloadInstaller">
          🔧 下载安装版
        </button>
        <button class="download-btn pan-btn" @click="downloadInstallerPan">
          📁 网盘下载
        </button>

      </div>
    </div>


  </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'

interface DownloadConfig {
  version: string
  downloads: {
    portable_7z: {
      gitee: string
      github: string
      name: string
      description: string
      size: string
      baiduPan: string
    }
    setup: {
      gitee: string
      github: string
      name: string
      description: string
      size: string
      baiduPan: string
    }
  }
}

const downloadData = ref<DownloadConfig | null>(null)

onMounted(async () => {
  try {
    const response = await fetch('/latest.json')
    downloadData.value = await response.json()
    console.log('Download config loaded:', downloadData.value)
  } catch (error) {
    console.error('Failed to load download config:', error)
  }
})

const downloadPortable = () => {
  if (downloadData.value?.downloads?.portable_7z?.gitee) {
    window.open(downloadData.value.downloads.portable_7z.gitee, '_blank')
  } else {
    // 备用链接：跳转到 Gitee releases 页面
    window.open('https://gitee.com/bjs1999/PixWit/releases', '_blank')
  }
}

const downloadInstaller = () => {
  if (downloadData.value?.downloads?.setup?.gitee) {
    window.open(downloadData.value.downloads.setup.gitee, '_blank')
  } else {
    // 备用链接：跳转到 Gitee releases 页面
    window.open('https://gitee.com/bjs1999/PixWit/releases', '_blank')
  }
}

const downloadPortablePan = () => {
  const panUrl = downloadData.value?.downloads?.portable_7z?.baiduPan
  if (panUrl) {
    navigator.clipboard.writeText(panUrl).then(() => {
      alert(`网盘下载链接已复制到剪贴板：\n\n${panUrl}`)
      window.open(panUrl, '_blank')
    }).catch(() => {
      alert(`请手动复制下载链接：\n\n${panUrl}`)
      window.open(panUrl, '_blank')
    })
  } else {
    alert('网盘下载信息暂未配置')
  }
}

const downloadInstallerPan = () => {
  const panUrl = downloadData.value?.downloads?.setup?.baiduPan
  if (panUrl) {
    navigator.clipboard.writeText(panUrl).then(() => {
      alert(`网盘下载链接已复制到剪贴板：\n\n${panUrl}`)
      window.open(panUrl, '_blank')
    }).catch(() => {
      alert(`请手动复制下载链接：\n\n${panUrl}`)
      window.open(panUrl, '_blank')
    })
  } else {
    alert('网盘下载信息暂未配置')
  }
}
</script>

