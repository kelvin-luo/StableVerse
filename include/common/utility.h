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
#ifndef UTILITY_H
#define UTILITY_H

#include <QMessageBox>
#include <QPushButton>
#include "commandstring.h"

extern const QString models_dir;

class Utility
{
public:
    Utility();
};

bool createDirectoryIfNotExists(const QString &path);

QString dateTimeToString(const QDateTime &dateTime);

bool writeStringToJsonFile(const QString &filePath, const QString &key,
                           const QString &value);

QString genCmd(tp_stru_command_string cmd, SD_Model_type modelCls);

#endif // UTILITY_H
