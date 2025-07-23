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
#ifndef FANCYGRAPHICSVIEW_H
#define FANCYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QTimer>
#include <QColor>
#include <QIcon>

class FancyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit FancyGraphicsView(QWidget *parent = nullptr);
    ~FancyGraphicsView();

    // 设置主题颜色
    void setTheme(const QColor &backgroundColor, const QColor &borderColor, 
                 const QColor &shadowColor = QColor(0, 0, 0, 50));
    
    // 设置边框样式
    void setBorderStyle(int radius, int width, const QColor &color);
    
    // 设置阴影效果
    void setShadowEffect(bool enabled, int blurRadius = 10, 
                        const QColor &color = QColor(0, 0, 0, 50));
    
    // 设置背景图案
    void setBackgroundPattern(const QBrush &brush);
    void setBackgroundImage(const QString &imagePath);
    
    // 设置滚动条样式
    void setScrollBarStyle(const QColor &background, const QColor &handle, 
                          int width = 10, int radius = 5);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QColor m_backgroundColor;
    QColor m_borderColor;
    QColor m_shadowColor;
    QBrush m_backgroundBrush;
    
    int m_borderRadius;
    int m_borderWidth;
    bool m_shadowEnabled;
    int m_shadowBlurRadius;
    
    // 初始化滚动条样式
    void initScrollBars();
};

#endif // FANCYGRAPHICSVIEW_H