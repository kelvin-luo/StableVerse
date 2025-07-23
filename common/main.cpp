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
#include "mainwindow.h"
#include <QApplication>

//#pragma encoding("UTF-8")
//#pragma execution_character_set("utf-8")

#include <QLocale>
#include <QTranslator>
#include <QTextCodec>
#include <QDebug>

int main(int argc, char *argv[]) {
    // QTextCodec *codec = QTextCodec::codecForName("UTF-8");//或者"GBK",不分大小写
    // QTextCodec::setCodecForLocale(codec);
    // QByteArray encodedStr = codec->fromUnicode(QString::fromUtf8("你的UTF-8文本"));
    // QString decodedStr = codec->toUnicode(encodedStr);

    // // 设置应用程序的编码为UTF-8
    // QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}    
