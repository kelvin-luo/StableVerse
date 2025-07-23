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
#ifndef FANCYCOMBOBOX_H
#define FANCYCOMBOBOX_H

#include <QComboBox>
#include <QStyleOptionComboBox>
#include <QPainter>
#include <QEvent>
#include <QTimer>
#include <QEnterEvent>

class FancyComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit FancyComboBox(QWidget *parent = nullptr);
    ~FancyComboBox() override;

    // 设置颜色主题
    void setTheme(const QColor &normalColor, const QColor &hoverColor, 
                  const QColor &selectColor, const QColor &textColor);
    
    // 设置边框样式
    void setBorderStyle(int radius = 8, int borderWidth = 1, 
                        const QColor &borderColor = QColor(200, 200, 200));
    
    // 设置下拉箭头图标
    void setArrowIcon(const QIcon &icon);
    
    // 设置阴影效果
    void setShadowEffect(bool enabled, int blurRadius = 10, 
                         const QColor &shadowColor = QColor(0, 0, 0, 50));

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) ;//override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void showPopup() override;
    void hidePopup() override;

private:
    // 样式属性
    QColor m_normalColor;
    QColor m_hoverColor;
    QColor m_selectColor;
    QColor m_textColor;
    QColor m_borderColor;
    QColor m_shadowColor;
    
    int m_borderRadius;
    int m_borderWidth;
    int m_blurRadius;
    
    bool m_hovered;
    bool m_pressed;
    bool m_popupVisible;
    bool m_shadowEnabled;
    
    QIcon m_arrowIcon;
    QTimer *m_animationTimer;
    int m_animationValue;
};

#endif // FANCYCOMBOBOX_H    
