/*
    Copyright (C) 2021  kelvin-luo

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    My Github homepage: https://github.com/kelvin-luo
*/
#include "fancycombobox.h"

FancyComboBox::FancyComboBox(QWidget *parent) :
    QComboBox(parent),
    m_normalColor(245, 245, 245),
    m_hoverColor(225, 225, 225),
    m_selectColor(200, 200, 200),
    m_textColor(50, 50, 50),
    m_borderColor(195, 195, 195),
    m_shadowColor(0, 0, 0, 50),
    m_borderRadius(4),
    m_borderWidth(1),
    m_blurRadius(10),
    m_hovered(false),
    m_pressed(false),
    m_popupVisible(false),
    m_shadowEnabled(true),
    m_animationValue(0)
{
    // 启用鼠标追踪以检测悬停状态
    setMouseTracking(true);
    
    // 创建动画计时器
    m_animationTimer = new QTimer(this);
    connect(m_animationTimer, &QTimer::timeout, this, [this]() {
        if (m_popupVisible) {
            m_animationValue = qMin(m_animationValue + 5, 100);
        } else {
            m_animationValue = qMax(m_animationValue - 5, 0);
        }
        update();
    });
    
    // 设置默认箭头图标
    setArrowIcon(QIcon(":/icons/arrow-down.png"));
}

FancyComboBox::~FancyComboBox()
{
    if (m_animationTimer->isActive()) {
        m_animationTimer->stop();
    }
}

void FancyComboBox::setTheme(const QColor &normalColor, const QColor &hoverColor, 
                            const QColor &selectColor, const QColor &textColor)
{
    m_normalColor = normalColor;
    m_hoverColor = hoverColor;
    m_selectColor = selectColor;
    m_textColor = textColor;
    update();
}

void FancyComboBox::setBorderStyle(int radius, int borderWidth, const QColor &borderColor)
{
    m_borderRadius = radius;
    m_borderWidth = borderWidth;
    m_borderColor = borderColor;
    update();
}

void FancyComboBox::setArrowIcon(const QIcon &icon)
{
    m_arrowIcon = icon;
    update();
}

void FancyComboBox::setShadowEffect(bool enabled, int blurRadius, const QColor &shadowColor)
{
    m_shadowEnabled = enabled;
    m_blurRadius = blurRadius;
    m_shadowColor = shadowColor;
    update();
}

#include<QPainterPath>
void FancyComboBox::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 计算背景颜色（考虑悬停、按下和弹出状态）
    QColor backgroundColor = m_normalColor;
    if (m_pressed) {
        backgroundColor = m_selectColor;
    } else if (m_hovered) {
        backgroundColor = m_hoverColor;
    }
    
    // 绘制阴影（如果启用）
    if (m_shadowEnabled && m_animationValue > 0) {
        QPainterPath shadowPath;
        shadowPath.addRoundedRect(rect().adjusted(2, 2, -2, -2), 
                                 m_borderRadius, m_borderRadius);
        
        QColor shadow = m_shadowColor;
        shadow.setAlpha(shadow.alpha() * m_animationValue / 100);
        
        painter.fillPath(shadowPath, shadow);
    }
    
    // 绘制背景
    QPainterPath path;
    path.addRoundedRect(rect().adjusted(m_borderWidth, m_borderWidth, 
                                       -m_borderWidth, -m_borderWidth), 
                       m_borderRadius, m_borderRadius);
    
    painter.fillPath(path, backgroundColor);
    
    // 绘制边框
    QPen pen(m_borderColor, m_borderWidth);
    painter.setPen(pen);
    painter.drawPath(path);
    
    // 绘制文本
    QStyleOptionComboBox opt;
    initStyleOption(&opt);
    
    QRect textRect = rect().adjusted(10, 0, -30, 0);
    painter.setPen(m_textColor);
    painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, currentText());
    
    // 绘制下拉箭头
    QRect arrowRect(width() - 24, 0, 24, height());
    m_arrowIcon.paint(&painter, arrowRect, Qt::AlignCenter);
}

void FancyComboBox::enterEvent(QEnterEvent *event)
{
    m_hovered = true;
    update();
    QComboBox::enterEvent(event);
}

void FancyComboBox::leaveEvent(QEvent *event)
{
    m_hovered = false;
    update();
    QComboBox::leaveEvent(event);
}

void FancyComboBox::mousePressEvent(QMouseEvent *event)
{
    m_pressed = true;
    update();
    QComboBox::mousePressEvent(event);
}

void FancyComboBox::showPopup()
{
    m_popupVisible = true;
    m_animationTimer->start(10);
    QComboBox::showPopup();
}

void FancyComboBox::hidePopup()
{
    m_popupVisible = false;
    m_animationTimer->start(10);
    QComboBox::hidePopup();
}    
