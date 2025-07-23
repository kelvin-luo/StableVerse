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
#include "commandparser.h"

CommandParser::CommandParser() {}

#include <iostream>
#include "commandstring.h"

#include <QStringList>

using json = nlohmann::json;

bool parseCommandLineToJson(const QString &commandLine, stru_command_string &cmd)
{
    // 分割命令行参数
    QStringList args = splitCommandLine(commandLine);
    if (args.isEmpty())
    {
        std::cerr << "解析命令行失败: 无法分割参数" << std::endl;
        return false;
    }

    // 初始化结果结构体
    // stru_command_string cmd;

    // 处理第一个参数作为可执行文件路径
    cmd.exeFile = args[0];

    // 提取运行目录（可执行文件所在目录）
    int lastSlash = cmd.exeFile.lastIndexOf('/');
    if (lastSlash == -1)
    {
        lastSlash = cmd.exeFile.lastIndexOf('\\');
    }
    if (lastSlash != -1)
    {
        cmd.running_abs_dir = cmd.exeFile.left(lastSlash);
    }

    // 处理其余参数
    for (int i = 1; i < args.size(); ++i)
    {
        QString arg = args[i];

        // 检查是否为选项参数
        if (arg.startsWith('-'))
        {
            QString option = arg;
            QString value;

            // 处理合并的选项和值（如：-ooutput.jpg）
            if (option.length() > 2 && !option.startsWith("--"))
            {
                value = option.mid(2);
                option = option.left(2);
            }
            // 处理长选项（如：--cfg-scale）
            else if (option.startsWith("--") && i + 1 < args.size())
            {
                value = args[++i];
            }
            // 处理短选项（如：-s 12345）
            else if (i + 1 < args.size())
            {
                value = args[++i];
            }

            // 映射选项到结构体字段
            if (option == "-m" || option == "--model")
            {
                cmd.checkpoint = value;
            }
            else if (option == "-p" || option == "--prompt")
            {
                cmd.PromptText = value;
            }
            else if (option == "-n" || option == "--negative-prompt")
            {
                cmd.negative_PromptText = value;
            }
            else if (option == "--embd-dir")
            {
                cmd.embd_dir = value;
            }
            else if (option == "--lora-model-dir")
            {
                cmd.lora_model_dir = value;
            }
            else if (option == "--cfg-scale")
            {
                cmd.cfg_scale = value.toFloat();
            }
            else if (option == "--strength")
            {
                cmd.unoising_strength = value.toFloat();
            }
            else if (option == "--steps")
            {
                cmd.steps = value.toInt();
            }
            else if (option == "--sampling-method")
            {
                cmd.sampling_method = value;
            }
            else if (option == "--schedule")
            {
                cmd.schedule = value;
            }
            else if (option == "-W")
            {
                cmd.latentImgWidth = value.toUInt();
            }
            else if (option == "-H")
            {
                cmd.latentImgHeight = value.toUInt();
            }
            else if (option == "--batch-count")
            {
                cmd.batch_count = value.toInt();
            }
            else if (option == "-o" || option == "--output")
            {
                cmd.resultImgFile = value;
            }
            else if (option == "-s" || option == "--seed")
            {
                cmd.seed = value.toInt();
            }
            else if (option == "-v" || option == "--vae")
            {
                cmd.vae = value;
            }
            // 忽略其他未处理的选项
        }
    }

    // 设置当前日期时间
    cmd.currentDateTime = QDateTime::currentDateTime();
}

// 解析命令行参数字符串并转换为JSON
bool parseCommandLineToJson(const QString &commandLine, json &resultJson)
{
    // 清空结果JSON
    resultJson.clear();

    stru_command_string cmd;
    parseCommandLineToJson(commandLine, cmd);
#if 0
    // 分割命令行参数
    QStringList args = splitCommandLine(commandLine);
    if (args.isEmpty()) {
        std::cerr << "解析命令行失败: 无法分割参数" << std::endl;
        return false;
    }

    // 初始化结果结构体
    // stru_command_string cmd;

    // 处理第一个参数作为可执行文件路径
    cmd.exeFile = args[0];

    // 提取运行目录（可执行文件所在目录）
    int lastSlash = cmd.exeFile.lastIndexOf('/');
    if (lastSlash == -1) {
        lastSlash = cmd.exeFile.lastIndexOf('\\');
    }
    if (lastSlash != -1) {
        cmd.running_abs_dir = cmd.exeFile.left(lastSlash);
    }

    // 处理其余参数
    for (int i = 1; i < args.size(); ++i) {
        QString arg = args[i];

        // 检查是否为选项参数
        if (arg.startsWith('-')) {
            QString option = arg;
            QString value;

            // 处理合并的选项和值（如：-ooutput.jpg）
            if (option.length() > 2 && !option.startsWith("--")) {
                value = option.mid(2);
                option = option.left(2);
            }
            // 处理长选项（如：--cfg-scale）
            else if (option.startsWith("--") && i + 1 < args.size()) {
                value = args[++i];
            }
            // 处理短选项（如：-s 12345）
            else if (i + 1 < args.size()) {
                value = args[++i];
            }

            // 映射选项到结构体字段
            if (option == "-m" || option == "--model") {
                cmd.checkpoint = value;
            } else if (option == "-p" || option == "--prompt") {
                cmd.PromptText = value;
            } else if (option == "-n" || option == "--negative-prompt") {
                cmd.negative_PromptText = value;
            } else if (option == "--embd-dir") {
                cmd.embd_dir = value;
            } else if (option == "--lora-model-dir") {
                cmd.lora_model_dir = value;
            } else if (option == "--cfg-scale") {
                cmd.cfg_scale = value.toFloat();
            } else if (option == "--strength") {
                cmd.unoising_strength = value.toFloat();
            } else if (option == "--steps") {
                cmd.steps = value.toInt();
            } else if (option == "--sampling-method") {
                cmd.sampling_method = value;
            } else if (option == "--schedule") {
                cmd.schedule = value;
            } else if (option == "-W") {
                cmd.latentImgWidth = value.toUInt();
            } else if (option == "-H") {
                cmd.latentImgHeight = value.toUInt();
            } else if (option == "--batch-count") {
                cmd.batch_count = value.toInt();
            } else if (option == "-o" || option == "--output") {
                cmd.resultImgFile = value;
            } else if (option == "-s" || option == "--seed") {
                cmd.seed = value.toInt();
            } else if (option == "-v" || option == "--vae") {
                cmd.vae = value;
            }
            // 忽略其他未处理的选项
        }
    }

    // 设置当前日期时间
    cmd.currentDateTime = QDateTime::currentDateTime();
#endif

    // 转换结构体为JSON
    try
    {
        // resultJson = cmd;
        to_json(resultJson, cmd);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "转换为JSON时出错: " << e.what() << std::endl;
        return false;
    }
}

// 辅助函数：分割命令行参数字符串，处理引号内的内容
QStringList splitCommandLine(const QString &commandLine)
{
    QStringList args;
    QString currentArg;
    bool inQuotes = false;
    bool inDoubleQuotes = false;

    for (int i = 0; i < commandLine.length(); ++i)
    {
        QChar c = commandLine[i];

        if (c == '\'' && !inDoubleQuotes)
        {
            inQuotes = !inQuotes;
            continue;
        }

        if (c == '\"' && !inQuotes)
        {
            inDoubleQuotes = !inDoubleQuotes;
            continue;
        }

        if (c.isSpace() && !inQuotes && !inDoubleQuotes)
        {
            if (!currentArg.isEmpty())
            {
                args.append(currentArg);
                currentArg.clear();
            }
        }
        else
        {
            currentArg.append(c);
        }
    }

    // 添加最后一个参数
    if (!currentArg.isEmpty())
    {
        args.append(currentArg);
    }

    return args;
}

#if 0
// 示例使用
int main() {
    QString commandLine = ".\\sd.exe -m ..\\models\\checkpoints\\stable-diffusion.cpp__weights\\sd-v1-4.ckpt -p \"best quality, ultra detailed, beautiful face, perfect anatomy, 1girl, solo, mememe, long hair, aqua hair, blunt bangs, pink eyes, pink dress, purple sleeves, purple thigh highs, <lora:mememe:1>, plain white background,\"  --embd-dir..\\models\\embeddings --lora-model-dir ..\\models\\loras -n \"(worst quality, low quality:1.2), easynegative, badhandv4, watermark, logo, cropped hands, extra digit, fewer digit, blurry, greyscale, (text:1.1), title, signature, jpeg artifacts, speech bubble, (multiple_views:1.2), nsfw,\"  --cfg-scale  6 --strength  0.7  --steps   20 --sampling-method  dpm++2m --schedule karras -W  512 -H  512 --batch-count  2  -v -o ../output/2025_07_23_09_50_05_seed_18467.jpg -s 18467 -t 12 ";

    json result;
    if (parseCommandLineToJson(commandLine, result)) {
        std::cout << "命令行解析成功，JSON内容:" << std::endl;
        std::cout << result.dump(4) << std::endl;

        // 保存JSON到文件
        std::ofstream file("parsed_command.json");
        if (file.is_open()) {
            file << result.dump(4);
            file.close();
            std::cout << "JSON已保存到 parsed_command.json" << std::endl;
        }
    }

    return 0;
}
#endif
