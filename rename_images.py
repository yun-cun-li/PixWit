#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
规范化图片文件名 - 去掉时间戳和冗余信息
"""
import os
from pathlib import Path

# 需要重命名的文件映射
rename_map = {
    '截图主界面_PixWit_20251214_221948.webp': '截图主界面.webp',
    '视频裁剪_PixWit_20251214_230804.webp': '视频剪辑.webp',
    '一键复制到EXCEL.webp': '表格提取.webp',
}

def main():
    # 图片所在目录
    guide_dir = Path(__file__).parent / 'docs' / 'guide'
    
    print(f"工作目录: {guide_dir}")
    print(f"目录是否存在: {guide_dir.exists()}")
    print()
    
    success_count = 0
    error_count = 0
    
    for old_name, new_name in rename_map.items():
        old_path = guide_dir / old_name
        new_path = guide_dir / new_name
        
        print(f"处理: {old_name} -> {new_name}")
        
        if not old_path.exists():
            print(f"  ⚠️  原文件不存在: {old_path}")
            error_count += 1
            continue
        
        if new_path.exists():
            print(f"  ⚠️  目标文件已存在: {new_path}")
            error_count += 1
            continue
        
        try:
            old_path.rename(new_path)
            print(f"  ✅ 重命名成功")
            success_count += 1
        except Exception as e:
            print(f"  ❌ 重命名失败: {e}")
            error_count += 1
        
        print()
    
    print("=" * 50)
    print(f"完成! 成功: {success_count}, 失败: {error_count}")
    
    if success_count == len(rename_map):
        print("✅ 所有文件重命名成功！")
    elif success_count > 0:
        print("⚠️  部分文件重命名成功")
    else:
        print("❌ 没有文件被重命名")

if __name__ == '__main__':
    main()

