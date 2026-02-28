/*
 * CapStep - Smart Screenshot Tool
 * Copyright (C) 2024-2025 CapStep Development Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file StylePopover.h
 * @brief 样式选择弹窗类
 * 
 * 提供颜色和粗细选择的弹窗界面
 * 
 * @author CapStep Team
 * @version 1.0.1
 * @date 2025-01-27
 */

#ifndef STYLEPOPOVER_H
#define STYLEPOPOVER_H

#include <QWidget>
#include <QButtonGroup>
#include <QPushButton>
#include <QColor>

/**
 * @class StylePopover
 * @brief 底部样式选择弹窗（颜色与粗细）
 */
class StylePopover : public QWidget
{
    Q_OBJECT

public:
    explicit StylePopover(QWidget *parent = nullptr);
    
    /**
     * @brief 设置当前选中的颜色和粗细
     * @param color 当前颜色
     * @param thickness 当前粗细
     */
    void setCurrent(const QColor &color, int thickness);

    /**
     * @brief 设置箭头中心位置
     * @param x X坐标
     */
    void setArrowCenterX(int x);

signals:
    /**
     * @brief 颜色选择信号
     * @param color 选中的颜色
     */
    void colorSelected(const QColor &color);
    
    /**
     * @brief 粗细选择信号
     * @param thickness 选中的粗细
     */
    void thicknessSelected(int thickness);

protected:
    /**
     * @brief 绘制事件
     * @param event 绘制事件对象
     */
    void paintEvent(QPaintEvent *event) override;

private:
    /**
     * @brief 创建颜色按钮
     * @param c 颜色
     * @return 按钮指针
     */
    QPushButton* makeColorButton(const QColor &c);

    /**
     * @brief 应用粗细按钮样式
     * @param button 按钮指针
     * @param diameter 直径
     */
    void applyThicknessStyle(QPushButton *button, int diameter);

    /**
     * @brief 创建圆点图标
     * @param diameter 直径
     * @param color 颜色
     * @return 图标
     */
    QIcon makeDotIcon(int diameter, const QColor &color) const;

    /**
     * @brief ID转颜色
     * @param id 颜色ID
     * @return 颜色
     */
    QColor idToColor(int id) const;

    /**
     * @brief ID转粗细
     * @param id 粗细ID
     * @return 粗细值
     */
    int idToThickness(int id) const;

    /**
     * @brief 查找最接近的颜色ID
     * @param color 目标颜色
     * @return 最接近的颜色ID
     */
    int findClosestColorId(const QColor &color) const;

private:
    const int m_arrowHeight = 10;           ///< 箭头高度
    QButtonGroup *m_colorGroup;             ///< 颜色按钮组
    QButtonGroup *m_thicknessGroup;         ///< 粗细按钮组
    QColor m_currentColor;                  ///< 当前颜色
    int m_arrowCenterX = -1;                ///< 箭头中心X坐标
    QPushButton *m_colorBtns[6] = {nullptr}; ///< 6种颜色按钮数组
    QPushButton *m_btnThin = nullptr;       ///< 细线按钮
    QPushButton *m_btnMed = nullptr;        ///< 中线按钮
    QPushButton *m_btnThick = nullptr;      ///< 粗线按钮
};

#endif // STYLEPOPOVER_H
