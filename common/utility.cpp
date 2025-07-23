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

#include "utility.h"

const QString models_dir = ".";

Utility::Utility() {}

bool createDirectoryIfNotExists(const QString &path)
{
  QDir dir(path);

  // 检查目录是否存在
  if (dir.exists())
  {
    qDebug() << "目录已存在";
    return true; // 目录已存在，返回成功
  }

  // 尝试创建目录（包括所有必要的父目录）
  qDebug() << "尝试创建目录";
  return dir.mkpath(".");
}

// 将 QDate 转换为字符串，格式："yyyy-MM-dd"
QString dateToString(const QDate &date) { return date.toString("yyyy-MM-dd"); }

// 将 QTime 转换为字符串，格式："HH:mm:ss"
QString timeToString(const QTime &time) { return time.toString("HH:mm:ss"); }

// 将 QDateTime 转换为字符串，格式："yyyy-MM-dd HH:mm:ss"
QString dateTimeToString(const QDateTime &dateTime)
{
  // return dateTime.toString("yyyy_MM_dd_HH_mm_ss");
  return dateTime.toString("yyMMdd_HHmmss");
}

bool writeStringToJsonFile(const QString &filePath, const QString &key,
                           const QString &value)
{
  QFile file(filePath);

  // 打开文件进行读写
  if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
  {
    qDebug() << "无法打开文件:" << filePath;
    return false;
  }

  QJsonDocument doc;
  QJsonObject jsonObject;

  // 如果文件不为空，读取现有 JSON 内容
  if (file.size() > 0)
  {
    QByteArray data = file.readAll();
    QJsonParseError parseError;
    doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
      qDebug() << "JSON 解析错误:" << parseError.errorString();
      file.close();
      return false;
    }

    if (doc.isObject())
    {
      jsonObject = doc.object();
    }
  }

  // 设置新的键值对
  jsonObject[key] = QJsonValue(value);

  // 重置文件指针并写入更新后的 JSON
  file.resize(0);
  QTextStream stream(&file);
  // stream.setCodec("UTF-8");//!< 在qt6中已经不用这样了.
  stream << QJsonDocument(jsonObject).toJson(QJsonDocument::Indented);

  file.close();
  return true;
}

/**
 * Starting sd.exe...
start to invoke cmd:
 sd.exe
Error: error: the following arguments are required: prompt

usage: sd.exe  [arguments]


arguments:
  -h, --help                         show this help message and exit
  -M, --mode [MODE]                  run mode, one of: [img_gen, convert],
default: img_gen -t, --threads N                    number of threads to use
during computation (default: -1) If threads <= 0, then threads will be set to
the number of CPU physical cores -m, --model [MODEL]                path to full
model
  --diffusion-model                  path to the standalone diffusion model
  --clip_l                           path to the clip-l text encoder
  --clip_g                           path to the clip-g text encoder
  --t5xxl                            path to the t5xxl text encoder
  --vae [VAE]                        path to vae
  --taesd [TAESD_PATH]               path to taesd. Using Tiny AutoEncoder for
fast decoding (low quality)
  --control-net [CONTROL_PATH]       path to control net model
  --embd-dir [EMBEDDING_PATH]        path to embeddings
  --stacked-id-embd-dir [DIR]        path to PHOTOMAKER stacked id embeddings
  --input-id-images-dir [DIR]        path to PHOTOMAKER input id images dir
  --normalize-input                  normalize PHOTOMAKER input id images
  --upscale-model [ESRGAN_PATH]      path to esrgan model. Upscale images after
generate, just RealESRGAN_x4plus_anime_6B supported by now
  --upscale-repeats                  Run the ESRGAN upscaler this many times
(default 1)
  --type [TYPE]                      weight type (examples: f32, f16, q4_0,
q4_1, q5_0, q5_1, q8_0, q2_K, q3_K, q4_K) If not specified, the default is the
type of the weight file
  --tensor-type-rules [EXPRESSION]   weight type per tensor pattern (example:
"^vae\.=f16,model\.=q8_0")
  --lora-model-dir [DIR]             lora model directory
  -i, --init-img [IMAGE]             path to the input image, required by
img2img
  --mask [MASK]                      path to the mask image, required by img2img
with mask
  --control-image [IMAGE]            path to image condition, control net
  -r, --ref-image [PATH]             reference image for Flux Kontext models
(can be used multiple times) -o, --output OUTPUT                path to write
result image to (default: ./output.png) -p, --prompt [PROMPT]              the
prompt to render -n, --negative-prompt PROMPT       the negative prompt
(default: "")
  --cfg-scale SCALE                  unconditional guidance scale:
(default: 7.0)
  --img-cfg-scale SCALE              image guidance scale for inpaint or
instruct-pix2pix models: (default: same as --cfg-scale)
  --guidance SCALE                   distilled guidance scale for models with
guidance input (default: 3.5)
  --slg-scale SCALE                  skip layer guidance (SLG) scale, only for
DiT models: (default: 0) 0 means disabled, a value of 2.5 is nice for sd3.5
medium
  --eta SCALE                        eta in DDIM, only for DDIM and TCD:
(default: 0)
  --skip-layers LAYERS               Layers to skip for SLG steps: (default:
[7,8,9])
  --skip-layer-start START           SLG enabling point: (default: 0.01)
  --skip-layer-end END               SLG disabling point: (default: 0.2)
                                     SLG will be enabled at step
int([STEPS]*[START]) and disabled at int([STEPS]*[END])
  --strength STRENGTH                strength for noising/unnoising (default:
0.75)
  --style-ratio STYLE-RATIO          strength for keeping input identity
(default: 20)
  --control-strength STRENGTH        strength to apply Control Net (default:
0.9) 1.0 corresponds to full destruction of information in init image -H,
--height H                     image height, in pixel space (default: 512) -W,
--width W                      image width, in pixel space (default: 512)
  --sampling-method {euler, euler_a, heun, dpm2, dpm++2s_a, dpm++2m, dpm++2mv2,
ipndm, ipndm_v, lcm, ddim_trailing, tcd} sampling method (default: "euler_a")
  --steps  STEPS                     number of sample steps (default: 20)
  --rng {std_default, cuda}          RNG (default: cuda)
  -s SEED, --seed SEED               RNG seed (default: 42, use random seed for
< 0) -b, --batch-count COUNT            number of images to generate
  --schedule {discrete, karras, exponential, ays, gits} Denoiser sigma schedule
(default: discrete)
  --clip-skip N                      ignore last layers of CLIP network; 1
ignores none, 2 ignores one layer (default: -1)
                                     <= 0 represents unspecified, will be 1 for
SD1.x, 2 for SD2.x
  --vae-tiling                       process vae in tiles to reduce memory usage
  --vae-on-cpu                       keep vae in cpu (for low vram)
  --clip-on-cpu                      keep clip in cpu (for low vram)
  --diffusion-fa                     use flash attention in the diffusion model
(for low vram) Might lower quality, since it implies converting k and v to f16.
                                     This might crash if it is not supported by
the backend.
  --control-net-cpu                  keep controlnet in cpu (for low vram)
  --canny                            apply canny preprocessor (edge detection)
  --color                            colors the logging tags according to level
  --chroma-disable-dit-mask          disable dit mask for chroma
  --chroma-enable-t5-mask            enable t5 mask for chroma
  --chroma-t5-mask-pad  PAD_SIZE     t5 mask pad size of chroma
  -v, --verbose                      print extra info

Process finished - Exit code: 1, Status: Normal exit
 */
QString genCmd(tp_stru_command_string cmd, SD_Model_type modelCls)
{
  QString cmd_str = cmd.exeFile;

  if (modelCls == SD_1_5 || modelCls == SDXL_1_0)
  {
    if (!cmd.checkpoint.isEmpty())
      cmd_str += " -m " + cmd.checkpoint;
    // qDebug() << "这是sd1.5模型或者sdxl模型。";
  }
  else if (modelCls == FLUX_1_0)
  {
    if (!cmd.checkpoint.isEmpty())
      cmd_str += " --diffusion-model " + cmd.checkpoint;
    // qDebug() << "这是flux1.0模型。";
  }
  else
  {
    if (!cmd.checkpoint.isEmpty())
      cmd_str += " --diffusion-model " + cmd.checkpoint;
    // qDebug() << "这是其他模型。";
  }

  if (!cmd.vae.isEmpty())
    cmd_str += " --vae " + cmd.vae;
  if (!cmd.clip_l.isEmpty())
    cmd_str += " --clip_l " + cmd.clip_l;
  if (!cmd.t5xxl.isEmpty())
    cmd_str += " --t5xxl " + cmd.t5xxl;
  // if(!cmd.PromptImgFile.isEmpty())cmd_str += " -p \""+cmd.PromptText + "\" ";
  if (!cmd.PromptText.isEmpty())
    cmd_str += " -p \"" + cmd.PromptText + "\" ";
  if (!cmd.embd_dir.isEmpty())
    cmd_str += " --embd-dir" + cmd.embd_dir;
  if (!cmd.lora_model_dir.isEmpty())
    cmd_str += " --lora-model-dir " + cmd.lora_model_dir;
  if (!cmd.negative_PromptText.isEmpty())
    cmd_str += " -n \"" + cmd.negative_PromptText + "\" ";

  cmd_str += " --cfg-scale  " + QString::number(cmd.cfg_scale);
  cmd_str += " --strength  " + QString::number(cmd.unoising_strength);
  cmd_str += "  --steps   " + QString::number(cmd.steps);

  if (!cmd.sampling_method.isEmpty())
    cmd_str += " --sampling-method  " + cmd.sampling_method;

  if (!cmd.schedule.isEmpty())
    cmd_str += " --schedule " + cmd.schedule;

  cmd_str += " -W  " + QString::number(cmd.latentImgWidth);
  cmd_str += " -H  " + QString::number(cmd.latentImgHeight);
  cmd_str += " --batch-count  " + QString::number(cmd.batch_count);

  cmd_str +=
      "  -v -o " + cmd.resultImgFile + " -s " + QString::number(cmd.seed);
  cmd_str += " -t 12 ";
  return cmd_str;

#if 0
    return QString(cmd.exeFile+ " --diffusion-model "+cmd.checkpoint+ " --vae "+cmd.vae+" --clip_l "+ cmd.clip_l+"  --t5xxl "+cmd.t5xxl+"   -p  "+cmd.PromptText +" --cfg-scale "+QString::number(cmd.cfg_scale)+" --sampling-method  "+cmd.sampling_method+"  -v -o " + cmd.resultImgFile+" -s " + QString::number(cmd.seed) );
#endif
}

#include <QMessageBox>
#include <QPushButton>
