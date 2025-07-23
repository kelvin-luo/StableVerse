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
#ifndef COMMANDSTRING_H
#define COMMANDSTRING_H

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTextStream>

#include <QDir>
#include <QTranslator>
#include <QDateTime>
#include <QDebug>
#include <QProcess>
#include <QString>

#include <nlohmann/json.hpp>
#include <QString>
#include <QDateTime>
#include <fstream>
#include <iostream>

extern const QString models_dir;
// extern const QString output_dir;

enum SD_Model_type {
    SD_1_5,
    SDXL_1_0,
    FLUX_1_0,
};

typedef struct stru_command_string {
    QString model_name;
    QString scene_name;

    QString running_abs_dir;

    QString exeFile;
    QString checkpoint;
    QString vae;
    QString clip_l;
    QString t5xxl;
    QString embd_dir;
    QString lora_model_dir;
    QString PromptImgFile;
    QString PromptText;
    QString negative_PromptText;
    uint latentImgWidth;
    uint latentImgHeight;
    QString sampling_method;
    QString schedule;
    float cfg_scale;
    float unoising_strength =0.70;
    int seed;
    int steps=20;
    int batch_count = 2;
    QString resultImgFile;
    QString resultJsonFile;

    QString text2img_output_dir ;

    QDateTime currentDateTime;
    stru_command_string() {
        exeFile = "sd.exe";
         text2img_output_dir = "../output";
    }
    bool check_exe_is_exist(QString content, QString exeFile);

public:
    bool checkIsExists();
    void set_deault() ;

} tp_stru_command_string;

// 从 JSON 转换为结构体
void from_json(const nlohmann::json& j, stru_command_string& cmd);

// 将结构体转换为 JSON
void to_json(nlohmann::json& j, const stru_command_string& cmd) ;

bool saveToJsonFile(const stru_command_string& cmd, const QString& filename) ;

class CommandString
{
public:
    CommandString();
};

#endif // COMMANDSTRING_H
