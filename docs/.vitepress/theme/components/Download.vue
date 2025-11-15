<template>
  <div class="download-section">
    <div class="download-cards">
      <div class="download-card recommended">
        <span class="download-badge">💚 推荐</span>
        <h3 class="download-title">绿色便携版</h3>
        <p class="download-desc">
          无需安装，解压即用<br>
          无杀毒软件拦截<br>
          适合快速体验
        </p>
        <button class="download-btn" @click="downloadPortable">
          📦 下载便携版
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
       
      </div>
    </div>

    <div style="margin-top: 3rem; padding: 2rem; background: var(--vp-c-bg-soft); border-radius: 12px;">
      <h3 style="margin-bottom: 1rem;">⚠️ 关于杀毒软件拦截</h3>
      <p style="color: var(--vp-c-text-2); line-height: 1.8;">
        由于本软件未购买代码签名证书，部分杀毒软件可能会误报。
        <strong>本软件完全开源</strong>，代码托管在 GitHub 和 Gitee，
        不包含任何恶意行为，请放心使用。
      </p>
      <ul style="margin-top: 1rem; color: var(--vp-c-text-2); line-height: 1.8;">
        <li>✅ 源代码完全开放透明</li>
        <li>✅ 无恶意行为，仅用于截图录屏</li>
        <li>✅ 推荐下载绿色便携版（误报更少）</li>
      </ul>
    </div>
  </div>
</template>

<script setup lang="ts">
const downloadPortable = () => {
  // 这里替换为实际的下载链接
  alert('请前往 GitHub Releases 页面下载最新版本')
  window.open('https://github.com/yun-cun-li/PixWit/releases', '_blank')
}

const downloadInstaller = () => {
  // 这里替换为实际的下载链接
  alert('请前往 GitHub Releases 页面下载最新版本')
  window.open('https://github.com/yun-cun-li/PixWit/releases', '_blank')
}
</script>

