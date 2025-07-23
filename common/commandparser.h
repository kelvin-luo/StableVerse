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

#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <nlohmann/json.hpp>
#include "commandstring.h"

class CommandParser
{
public:
    CommandParser();
};

// 辅助函数：分割命令行参数字符串，处理引号内的内容
QStringList splitCommandLine(const QString& commandLine) ;

// 解析命令行参数字符串并转换为JSON
bool parseCommandLineToJson(const QString& commandLine, nlohmann::json& resultJson) ;

bool parseCommandLineToJson(const QString& commandLine, stru_command_string& cmd);

#endif // COMMANDPARSER_H
