<template>
  <div class="community-section">
    <h2 class="community-title">💬 加入交流群</h2>
    
    <div class="qr-code-container">
      <!-- QQ 群 -->
      <div class="qr-code-card">
        <img 
          src="/guide/qq群.jpg" 
          alt="PixWit QQ 交流群二维码 - 群号 133125470" 
          class="qr-code-image"
          loading="lazy"
        />
        <p class="qr-code-label">📱 QQ 群：133125470</p>
      </div>

      <!-- 微信群 -->
      <div class="qr-code-card">
        <img 
          src="/guide/weichat微信.jpg" 
          alt="PixWit 微信交流群二维码" 
          class="qr-code-image"
          loading="lazy"
        />
        <p class="qr-code-label">💬 微信交流群</p>
      </div>
    </div>

    <!-- 其他联系方式 -->
    <div class="other-links">
      <a href="https://github.com/yun-cun-li/PixWit" target="_blank" rel="noopener noreferrer" class="link-button">
        GitHub
      </a>
      <a href="https://gitee.com/bjs1999/PixWit" target="_blank" rel="noopener noreferrer" class="link-button">
        Gitee
      </a>
    </div>
  </div>
</template>

<style scoped>
.community-section {
  padding: 4rem 1.5rem;
  background: var(--vp-c-bg-soft);
  text-align: center;
}

.community-title {
  font-size: 2rem;
  margin-bottom: 2.5rem;
  color: var(--vp-c-text-1);
  font-weight: 600;
}

.qr-code-container {
  display: flex;
  gap: 3rem;
  max-width: 800px;
  margin: 0 auto 2.5rem;
  justify-content: center;
  flex-wrap: wrap;
}

.qr-code-card {
  text-align: center;
}

.qr-code-image {
  width: 280px;
  height: 280px;
  object-fit: contain;
  display: block;
  background: #fff;
  padding: 16px;
  border-radius: 12px;
  box-shadow: 0 4px 16px rgba(0, 0, 0, 0.1);
  transition: all 0.3s ease;
}

.qr-code-image:hover {
  transform: scale(1.05);
  box-shadow: 0 8px 24px rgba(0, 0, 0, 0.15);
}

.qr-code-label {
  margin-top: 1rem;
  font-size: 1.1rem;
  font-weight: 500;
  color: var(--vp-c-text-1);
}

.other-links {
  display: flex;
  gap: 1rem;
  justify-content: center;
  flex-wrap: wrap;
}

.link-button {
  padding: 0.6rem 1.5rem;
  background: var(--vp-c-bg);
  border: 2px solid var(--vp-c-divider);
  border-radius: 8px;
  text-decoration: none;
  color: var(--vp-c-text-1);
  font-weight: 500;
  transition: all 0.3s;
}

.link-button:hover {
  background: var(--vp-c-brand-1);
  color: white;
  border-color: var(--vp-c-brand-1);
  transform: translateY(-2px);
}

/* 响应式设计 */
@media (max-width: 768px) {
  .community-section {
    padding: 3rem 1rem;
  }

  .community-title {
    font-size: 1.5rem;
    margin-bottom: 2rem;
  }

  .qr-code-container {
    gap: 2rem;
  }

  .qr-code-image {
    width: 240px;
    height: 240px;
  }

  .qr-code-label {
    font-size: 1rem;
  }
}

/* 暗色模式优化 */
.dark .qr-code-image {
  background: #fff;
  box-shadow: 0 4px 16px rgba(0, 0, 0, 0.3);
}

.dark .qr-code-image:hover {
  box-shadow: 0 8px 24px rgba(0, 0, 0, 0.4);
}
</style>

