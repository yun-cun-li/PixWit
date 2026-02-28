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
 * @file StylePopover.cpp
 * @brief 样式选择弹窗类实现
 * 
 * @author CapStep Team
 * @version 1.0.1
 * @date 2025-01-27
 */

#include "StylePopover.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QPainter>
#include <QPainterPath>
#include <QPolygon>
#include <QPen>
#include <QIcon>
#include <QPixmap>
#include <QDebug>
#include <limits>

StylePopover::StylePopover(QWidget *parent)
    : QWidget(parent)
    , m_colorGroup(new QButtonGroup(this))
    , m_thicknessGroup(new QButtonGroup(this))
    , m_currentColor(255,0,0)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);

    QHBoxLayout *root = new QHBoxLayout(this);
    root->setContentsMargins(12, 12 + m_arrowHeight, 12, 12);
    root->setSpacing(10);

    // 粗细选择 - 使用实心黑点表示
    QWidget *thickWrap = new QWidget(this);
    QHBoxLayout *thickLayout = new QHBoxLayout(thickWrap);
    thickLayout->setContentsMargins(0,0,0,0);
    thickLayout->setSpacing(8);
    
    m_btnThin = new QPushButton(this); m_btnThin->setCheckable(true);
    m_btnMed  = new QPushButton(this); m_btnMed->setCheckable(true);
    m_btnThick= new QPushButton(this); m_btnThick->setCheckable(true);
    
    // 为粗细按钮分别设置不同直径（小/中/大）
    applyThicknessStyle(m_btnThin, 14);
    applyThicknessStyle(m_btnMed, 18);
    applyThicknessStyle(m_btnThick, 22);
    
    m_thicknessGroup->addButton(m_btnThin, 1);
    m_thicknessGroup->addButton(m_btnMed, 2);
    m_thicknessGroup->addButton(m_btnThick, 3);
    m_btnMed->setChecked(true);
    
    thickLayout->addWidget(m_btnThin);
    thickLayout->addWidget(m_btnMed);
    thickLayout->addWidget(m_btnThick);

    // 颜色选择器 - 6种颜色，使用方块表示
    QWidget *colorWrap = new QWidget(this);
    QHBoxLayout *colorLayout = new QHBoxLayout(colorWrap);
    colorLayout->setContentsMargins(0,0,0,0);
    colorLayout->setSpacing(6);
    
    // 6种颜色按钮 
    m_colorBtns[0] = makeColorButton(QColor(0x21, 0x21, 0x21)); // 深灰色
    m_colorBtns[1] = makeColorButton(QColor(0x3B, 0x82, 0xF6)); // 蓝色
    m_colorBtns[2] = makeColorButton(QColor(0x10, 0xB9, 0x81)); // 绿色
    m_colorBtns[3] = makeColorButton(QColor(0xF5, 0x9E, 0x0B)); // 橙色
    m_colorBtns[4] = makeColorButton(QColor(0xFF, 0x00, 0x00)); // 红色
    m_colorBtns[5] = makeColorButton(QColor(0xFF, 0xFF, 0x00)); // 黄色
    
    for (int i = 0; i < 6; i++) {
        m_colorGroup->addButton(m_colorBtns[i], i + 1);
        colorLayout->addWidget(m_colorBtns[i]);
    }
    
    // 默认选中第一个颜色（深灰色）
    m_colorBtns[0]->setChecked(true);

    root->addWidget(thickWrap);
    root->addWidget(colorWrap);

    connect(m_colorGroup, &QButtonGroup::idClicked, this, [this](int id){ QColor c = idToColor(id); m_currentColor = c; emit colorSelected(c); });
    connect(m_thicknessGroup, &QButtonGroup::idClicked, this, [this](int id){ emit thicknessSelected(idToThickness(id)); });
    setFixedHeight(60 + m_arrowHeight);
}

void StylePopover::setCurrent(const QColor &color, int thickness) {
    // 颜色匹配 - 查找最接近的颜色ID
    int colorId = 1; // 默认深灰色
    QColor targetColor = color;
    
    // 精确匹配
    if (targetColor == QColor(0x21, 0x21, 0x21)) colorId = 1; // 深灰色
    else if (targetColor == QColor(0x3B, 0x82, 0xF6)) colorId = 2; // 蓝色
    else if (targetColor == QColor(0x10, 0xB9, 0x81)) colorId = 3; // 绿色
    else if (targetColor == QColor(0xF5, 0x9E, 0x0B)) colorId = 4; // 橙色
    else if (targetColor == QColor(0xFF, 0x00, 0x00)) colorId = 5; // 红色
    else if (targetColor == QColor(0xFF, 0xFF, 0x00)) colorId = 6; // 黄色
    else {
        // 如果没有精确匹配，选择最接近的颜色
        colorId = findClosestColorId(targetColor);
    }
    
    if (auto *btn = m_colorGroup->button(colorId)) btn->setChecked(true);
    m_currentColor = color;
    
    // 粗细
    int thickId = thickness <= 2 ? 1 : (thickness >= 7 ? 3 : 2);
    if (auto *btn = m_thicknessGroup->button(thickId)) btn->setChecked(true);
}

void StylePopover::setArrowCenterX(int x) {
    m_arrowCenterX = x;
    update();
}

void StylePopover::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    QRect r = rect().adjusted(0, m_arrowHeight, 0, 0);
    QPainterPath path;
    path.addRoundedRect(r.adjusted(0,0,0,0), 8, 8);
    p.fillPath(path, QColor(255,255,255,250));
    p.setPen(QPen(QColor(220,220,220), 1));
    p.drawPath(path);
    // 小三角指示
    int cx = (m_arrowCenterX >= 0) ? m_arrowCenterX : width()/2;
    QPoint c(cx, m_arrowHeight/2);
    QPolygon tri;
    tri << QPoint(c.x()-8, m_arrowHeight) << QPoint(c.x()+8, m_arrowHeight) << QPoint(c.x(), 0);
    p.setBrush(QColor(255,255,255,250));
    p.setPen(QPen(QColor(220,220,220), 1));
    p.drawPolygon(tri);
}

QPushButton* StylePopover::makeColorButton(const QColor &c) {
    QPushButton *b = new QPushButton(this);
    b->setCheckable(true);
    QString base = QString("QPushButton{min-width:18px;min-height:18px;max-width:18px;max-height:18px;"
                           "border-radius:2px;border:none; background:%1;}"
                           "QPushButton:checked{border:2px solid rgba(120,120,120,220);}"
                           "QPushButton:hover{background:%1;}"
                           ).arg(c.name());
    b->setStyleSheet(base);
    return b;
}

void StylePopover::applyThicknessStyle(QPushButton *button, int diameter) {
    const int radius = diameter / 2;
    QString style = QString(
        "QPushButton{background-color:rgba(255,255,255,1);"
        "border:1px solid rgba(200,200,200,0.8); width:%1px; height:%1px; min-width:%1px; min-height:%1px;"
        "max-width:%1px; max-height:%1px; border-radius:%2px;}"
        "QPushButton:checked{background-color:rgba(160,160,160,1); border:none; border-radius:%2px;}"
        "QPushButton:hover{background-color:rgba(240,240,240,0.95); border:1px solid rgba(180,180,180,0.8); border-radius:%2px;}"
    ).arg(diameter).arg(radius);
    button->setStyleSheet(style);
    button->setFixedSize(diameter, diameter);
}

QIcon StylePopover::makeDotIcon(int diameter, const QColor &color) const {
    int canvas = 20;
    QPixmap pm(canvas, canvas);
    pm.fill(Qt::transparent);
    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    p.setBrush(color);
    int x = (canvas - diameter)/2;
    int y = (canvas - diameter)/2;
    p.drawEllipse(QRect(x, y, diameter, diameter));
    return QIcon(pm);
}

QColor StylePopover::idToColor(int id) const {
    switch(id){
    case 1: return QColor(0x21, 0x21, 0x21); // 深灰色
    case 2: return QColor(0x3B, 0x82, 0xF6); // 蓝色
    case 3: return QColor(0x10, 0xB9, 0x81); // 绿色
    case 4: return QColor(0xF5, 0x9E, 0x0B); // 橙色
    case 5: return QColor(0xFF, 0x00, 0x00); // 红色
    case 6: return QColor(0xFF, 0xFF, 0x00); // 黄色
    default: return QColor(0x21, 0x21, 0x21); // 默认深灰色
    }
}

int StylePopover::idToThickness(int id) const {
    switch(id){
    case 1: return 2; // 细
    case 3: return 7; // 粗
    default: return 4; // 中
    }
}

int StylePopover::findClosestColorId(const QColor &color) const {
    // 计算颜色距离，找到最接近的颜色
    int closestId = 1;
    double minDistance = 1e10; // 使用一个很大的数值代替 std::numeric_limits<double>::max()
    
    for (int id = 1; id <= 6; id++) {
        QColor paletteColor = idToColor(id);
        double distance = std::sqrt(
            std::pow(color.red() - paletteColor.red(), 2) +
            std::pow(color.green() - paletteColor.green(), 2) +
            std::pow(color.blue() - paletteColor.blue(), 2)
        );
        if (distance < minDistance) {
            minDistance = distance;
            closestId = id;
        }
    }
    return closestId;
}
