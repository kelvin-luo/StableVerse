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
#include "commandstring.h"
#include "utility.h"
using json = nlohmann::json;

CommandString::CommandString() {}

bool stru_command_string::check_exe_is_exist(QString content, QString exeFile)
{
    bool status = true;
    if (!exeFile.isEmpty())
    {
        bool fileExists = QFile::exists(exeFile);
        if (fileExists)
        {
            qDebug() << content << ":" << exeFile << ": 文件存在";
        }
        else
        {
            qDebug() << content << ":" << exeFile << ": 文件不存在";
        }
        status = fileExists;
    }
    else
    {
        qDebug() << content << ":" << exeFile << ": 未设置，忽略.";
    }
    return status;
}

bool stru_command_string::checkIsExists()
{
    qDebug() << "文件存在性检查开始:";
    bool status = true;
    status = status && check_exe_is_exist("exeFile", exeFile);
    status = status && check_exe_is_exist("checkpoint", checkpoint);
    status = status && check_exe_is_exist("vae", vae);
    status = status && check_exe_is_exist("clip_l", clip_l);
    status = status && check_exe_is_exist("t5xxl", t5xxl);
    status = status && check_exe_is_exist("PromptImgFile", PromptImgFile);
    return status;
}

void stru_command_string::set_deault()
{
    model_name = "v1-5-pruned-emaonly";
    scene_name = "";

    checkpoint =  models_dir+"\\models\\checkpoints\\stable-diffusion.cpp__weights\\v1-5-pruned-emaonly.safetensors";
    embd_dir = models_dir+"\\models\\embeddings";
    lora_model_dir = models_dir+"\\models\\loras";

    sampling_method = "dpm++2mv2";
    schedule = "karras";
    steps = 20;
    cfg_scale = 7;

    // --strength
    unoising_strength = 0.70;

    PromptText = "Pine forest, (full view), sunset, sun setting behind pine "
                 "trees, warm light, (light leak:1.2), shallow depth of field, "
                 "cinematic warm tones, melancholic mood";
    negative_PromptText =
        "b&w, black and white, monochrome, blurry, painting, 3d, anime, "
        "cartoon, tiling, deformed, text, glitch, jpeg artifacts, "
        "oversaturated, unrealistic, bad perspective";

    PromptImgFile = "";

    int w = 512;
    int h = 512;
    latentImgWidth = w;
    latentImgHeight = h;
    seed = rand();

    QDateTime currentDateTime = QDateTime::currentDateTime();
    currentDateTime = QDateTime::currentDateTime();

    resultImgFile = text2img_output_dir + "/" + scene_name + "_" + model_name + dateTimeToString(currentDateTime) + "_" +
                    QString::number(seed) + ".jpg";
    resultJsonFile = text2img_output_dir + "/" + scene_name + "_" + model_name + dateTimeToString(currentDateTime) + "_" +
                     QString::number(seed) + ".json";
}

// 将 QString 转换为 JSON 字符串
void to_json(json &j, const QString &str)
{
    j = str.toStdString();
}

// 从 JSON 字符串转换为 QString
void from_json(const json &j, QString &str)
{
    str = QString::fromStdString(j.get<std::string>());
}

// 将 QDateTime 转换为 JSON 字符串（ISO 格式）
void to_json(json &j, const QDateTime &dt)
{
    j = dt.toString(Qt::ISODate).toStdString();
}

// 从 JSON 字符串转换为 QDateTime
void from_json(const json &j, QDateTime &dt)
{
    dt = QDateTime::fromString(QString::fromStdString(j.get<std::string>()), Qt::ISODate);
}

// 将结构体转换为 JSON
void to_json(json &j, const stru_command_string &cmd)
{
    j = json{
        {"running_abs_dir", cmd.running_abs_dir},
        {"exeFile", cmd.exeFile},
        {"checkpoint", cmd.checkpoint},
        {"vae", cmd.vae},
        {"clip_l", cmd.clip_l},
        {"t5xxl", cmd.t5xxl},
        {"embd_dir", cmd.embd_dir},
        {"lora_model_dir", cmd.lora_model_dir},
        {"PromptImgFile", cmd.PromptImgFile},
        {"PromptText", cmd.PromptText},
        {"negative_PromptText", cmd.negative_PromptText},
        {"latentImgWidth", cmd.latentImgWidth},
        {"latentImgHeight", cmd.latentImgHeight},
        {"sampling_method", cmd.sampling_method},
        {"schedule", cmd.schedule},
        {"cfg_scale", cmd.cfg_scale},
        {"unoising_strength", cmd.unoising_strength},
        {"seed", cmd.seed},
        {"steps", cmd.steps},
        {"batch_count", cmd.batch_count},
        {"resultImgFile", cmd.resultImgFile},
        {"resultJsonFile", cmd.resultJsonFile},
        {"currentDateTime", cmd.currentDateTime}};
}

// 从 JSON 转换为结构体
void from_json(const json &j, stru_command_string &cmd)
{
    cmd.running_abs_dir = j.at("running_abs_dir").get<QString>();
    cmd.exeFile = j.at("exeFile").get<QString>();
    cmd.checkpoint = j.at("checkpoint").get<QString>();
    cmd.vae = j.at("vae").get<QString>();
    cmd.clip_l = j.at("clip_l").get<QString>();
    cmd.t5xxl = j.at("t5xxl").get<QString>();
    cmd.embd_dir = j.at("embd_dir").get<QString>();
    cmd.lora_model_dir = j.at("lora_model_dir").get<QString>();
    cmd.PromptImgFile = j.at("PromptImgFile").get<QString>();
    cmd.PromptText = j.at("PromptText").get<QString>();
    cmd.negative_PromptText = j.at("negative_PromptText").get<QString>();
    cmd.latentImgWidth = j.at("latentImgWidth").get<uint>();
    cmd.latentImgHeight = j.at("latentImgHeight").get<uint>();
    cmd.sampling_method = j.at("sampling_method").get<QString>();
    cmd.schedule = j.at("schedule").get<QString>();
    cmd.cfg_scale = j.at("cfg_scale").get<float>();
    cmd.unoising_strength = j.at("unoising_strength").get<float>();
    cmd.seed = j.at("seed").get<int>();
    cmd.steps = j.at("steps").get<int>();
    cmd.batch_count = j.at("batch_count").get<int>();
    cmd.resultImgFile = j.at("resultImgFile").get<QString>();
    cmd.resultJsonFile = j.at("resultJsonFile").get<QString>();
    cmd.currentDateTime = j.at("currentDateTime").get<QDateTime>();
}

// 保存结构体数据到 JSON 文件
bool saveToJsonFile(const stru_command_string &cmd, const QString &filename)
{
    try
    {
        json j = cmd;
        std::ofstream file(filename.toStdString());
        if (file.is_open())
        {
            file << j.dump(4); // 缩进4个空格，使JSON更易读
            file.close();
            return true;
        }
        return false;
    }
    catch (const std::exception &e)
    {
        std::cerr << "保存JSON文件时出错: " << e.what() << std::endl;
        return false;
    }
}

// 从 JSON 文件加载数据到结构体
bool loadFromJsonFile(stru_command_string &cmd, const QString &filename)
{
    try
    {
        std::ifstream file(filename.toStdString());
        if (file.is_open())
        {
            json j;
            file >> j;
            cmd = j.get<stru_command_string>();
            file.close();
            return true;
        }
        return false;
    }
    catch (const std::exception &e)
    {
        std::cerr << "加载JSON文件时出错: " << e.what() << std::endl;
        return false;
    }
}

// 解析 JSON 字符串到结构体
bool parseJsonString(stru_command_string &cmd, const QString &jsonString)
{
    try
    {
        json j = json::parse(jsonString.toStdString());
        cmd = j.get<stru_command_string>();
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "解析JSON字符串时出错: " << e.what() << std::endl;
        return false;
    }
}

#if 0
// 示例使用
int main() {
    // 创建一个示例结构体
    stru_command_string cmd;
    cmd.running_abs_dir = "/path/to/running/dir";
    cmd.exeFile = "generator.exe";
    cmd.cfg_scale = 7.5;
    cmd.seed = 42;
    cmd.currentDateTime = QDateTime::currentDateTime();

    // 保存到JSON文件
    if (saveToJsonFile(cmd, "config.json")) {
        std::cout << "JSON文件保存成功" << std::endl;
    }

    // 从JSON文件加载
    stru_command_string loadedCmd;
    if (loadFromJsonFile(loadedCmd, "config.json")) {
        std::cout << "JSON文件加载成功" << std::endl;
        std::cout << "加载的配置: seed=" << loadedCmd.seed
                  << ", cfg_scale=" << loadedCmd.cfg_scale << std::endl;
    }

    // 解析JSON字符串
    QString jsonString = R"({
        "running_abs_dir": "/path/from/string",
        "exeFile": "string_loader.exe",
        "cfg_scale": 8.0,
        "seed": 12345,
        "currentDateTime": "2023-10-20T12:34:56"
    })";

    stru_command_string parsedCmd;
    if (parseJsonString(parsedCmd, jsonString)) {
        std::cout << "JSON字符串解析成功" << std::endl;
        std::cout << "解析的配置: seed=" << parsedCmd.seed
                  << ", cfg_scale=" << parsedCmd.cfg_scale << std::endl;
    }

    return 0;
}
#endif
