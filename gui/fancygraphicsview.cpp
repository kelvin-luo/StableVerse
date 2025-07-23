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
//#pragma encoding("UTF-8")
//#pragma execution_character_set("utf-8")
#include "fancygraphicsview.h"
#include <QPainter>
#include <QScrollBar>
#include <QGraphicsScene>
#include <QFile>

FancyGraphicsView::FancyGraphicsView(QWidget *parent) :
    QGraphicsView(parent),
    m_backgroundColor(255, 255, 255),
    m_borderColor(200, 200, 200),
    m_shadowColor(0, 0, 0, 50),
    m_borderRadius(8),
    m_borderWidth(1),
    m_shadowEnabled(true),
    m_shadowBlurRadius(10)
{
    // 设置视图属性
    setRenderHint(QPainter::Antialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
    setFrameShape(QFrame::NoFrame);
    
    // 初始化背景
    setBackgroundBrush(QBrush(Qt::white));
    
    // 初始化滚动条
    initScrollBars();
}

FancyGraphicsView::~FancyGraphicsView()
{
    // 清理
}

void FancyGraphicsView::setTheme(const QColor &backgroundColor, const QColor &borderColor, 
                                const QColor &shadowColor)
{
    m_backgroundColor = backgroundColor;
    m_borderColor = borderColor;
    m_shadowColor = shadowColor;
    
    // 更新背景
    setBackgroundBrush(QBrush(m_backgroundColor));
    
    update();
}

void FancyGraphicsView::setBorderStyle(int radius, int width, const QColor &color)
{
    m_borderRadius = radius;
    m_borderWidth = width;
    m_borderColor = color;
    update();
}

void FancyGraphicsView::setShadowEffect(bool enabled, int blurRadius, const QColor &color)
{
    m_shadowEnabled = enabled;
    m_shadowBlurRadius = blurRadius;
    m_shadowColor = color;
    update();
}

void FancyGraphicsView::setBackgroundPattern(const QBrush &brush)
{
    m_backgroundBrush = brush;
    setBackgroundBrush(m_backgroundBrush);
    update();
}

void FancyGraphicsView::setBackgroundImage(const QString &imagePath)
{
    QPixmap background(imagePath);
    if (!background.isNull()) {
        setBackgroundBrush(QBrush(background.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    }
}

void FancyGraphicsView::setScrollBarStyle(const QColor &background, const QColor &handle, int width, int radius)
{
    // 设置水平滚动条样式
    horizontalScrollBar()->setStyleSheet(
        QString("QScrollBar:horizontal {"
                "    background: %1;"
                "    height: %2px;"
                "    border-radius: %3px;"
                "}"
                "QScrollBar::handle:horizontal {"
                "    background: %4;"
                "    min-width: 20px;"
                "    border-radius: %3px;"
                "}"
                "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {"
                "    width: 0px;"
                "    height: 0px;"
                "}"
                "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {"
                "    background: none;"
                "}")
        .arg(background.name())
        .arg(width)
        .arg(radius)
        .arg(handle.name())
    );
    
    // 设置垂直滚动条样式
    verticalScrollBar()->setStyleSheet(
        QString("QScrollBar:vertical {"
                "    background: %1;"
                "    width: %2px;"
                "    border-radius: %3px;"
                "}"
                "QScrollBar::handle:vertical {"
                "    background: %4;"
                "    min-height: 20px;"
                "    border-radius: %3px;"
                "}"
                "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
                "    width: 0px;"
                "    height: 0px;"
                "}"
                "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
                "    background: none;"
                "}")
        .arg(background.name())
        .arg(width)
        .arg(radius)
        .arg(handle.name())
    );
}

#include<QPainterPath>
void FancyGraphicsView::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 绘制阴影
    if (m_shadowEnabled) {
        QPainterPath shadowPath;
        shadowPath.addRoundedRect(rect().adjusted(2, 2, -2, -2), 
                                 m_borderRadius, m_borderRadius);
        
        QColor shadow = m_shadowColor;
        painter.fillPath(shadowPath, shadow);
    }
    
    // 绘制背景
    QPainterPath path;
    path.addRoundedRect(rect().adjusted(m_borderWidth, m_borderWidth, 
                                       -m_borderWidth, -m_borderWidth), 
                       m_borderRadius, m_borderRadius);
    
    painter.fillPath(path, m_backgroundColor);
    
    // 绘制边框
    QPen pen(m_borderColor, m_borderWidth);
    painter.setPen(pen);
    painter.drawPath(path);
    
    // 调用基类绘制内容
    QGraphicsView::paintEvent(event);
}

#include<QResizeEvent>
void FancyGraphicsView::resizeEvent(QResizeEvent *event)
{
    // 如果设置了背景图片，调整其大小
    if (!backgroundBrush().texture().isNull()) {
        setBackgroundBrush(QBrush(backgroundBrush().texture().scaled(
            event->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    }
    
    QGraphicsView::resizeEvent(event);
}

void FancyGraphicsView::initScrollBars()
{
    // 设置默认滚动条样式
    setScrollBarStyle(QColor(240, 240, 240), QColor(180, 180, 180), 8, 4);
    
    // 确保滚动条在需要时才显示
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}    
