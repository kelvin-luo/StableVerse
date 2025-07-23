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

#include "mainwindow.h"

#include "ui_mainwindow.h"
#include <QDebug>
#include <QDir>
#include "utility.h"
#include <QGraphicsItem>

// const QString output_dir = "output";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    {
        QString name = QLocale::system().name().toLower();
        const auto &langMap = languageMap();
        for (auto item : langMap)
        {
            qDebug() << "item = " << item.first << "," << item.second;
            if (1)
                ui->cbbLanguage->addItem(item.first);
        }
        bool isContain =
            std::find_if(langMap.begin(), langMap.end(),
                         [&name](const std::pair<QString, QString> &pair)
                         {
                             return pair.second == name;
                         }) != langMap.end();
        ui->cbbLanguage->setCurrentText(
            toLocaleNameByVal(isContain ? name : "en_us"));

        qDebug() << "MainUI::MainUI, call loadTranslation";
        loadTranslation(isContain ? name : "en_us");
    }

    process = new QProcess(this);

    this->setWindowIcon(QIcon(":/icon_db/Colorful_images.png"));
    this->setIconSize(QSize(64, 64));

    connect(process, &QProcess::readyReadStandardOutput, this,
            &MainWindow::handleProcessOutput);
    connect(process, &QProcess::readyReadStandardError, this,
            &MainWindow::handleProcessOutput);
    connect(process,
            QOverload<QProcess::ProcessError>::of(&QProcess::errorOccurred), this,
            &MainWindow::handleProcessError);
    connect(process,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            &MainWindow::handleProcessFinished);

    ui->stopButton->setEnabled(false);
    ui->plainTextEdit_cmd->setPlainText("sd.exe --help");
    // 在构造函数中初始化
    ui->spinBox_seed->setRange(-1, 999999);

    QGraphicsScene *scene = new QGraphicsScene;
    QBrush brush(Qt::GlobalColor::black);
    scene->setBackgroundBrush(brush);
    ui->graphicsView->setScene(scene);

    // # 设置更大的范围（PyQt示例）
    ui->spinBox_W->setRange(0, 7680); // # 允许0-1000的数值
    ui->spinBox_H->setRange(0, 7680);
    int w = 512;
    int h = 512;
    ui->spinBox_W->setValue(w);
    ui->spinBox_H->setValue(h);

    ui->checkBox_seed_fixed->setChecked(false);

    //! 先于下拉框进行初始化
    tp_stru_command_string cmd;
    const QString outputdir = qGuiApp->applicationDirPath() + cmd.text2img_output_dir;
    bool creatrDir = createDirectoryIfNotExists(outputdir);
    qDebug() << "创建目录返回值: " << creatrDir;

    ui->comboBox_presets_model->addItem("anime"); //!< 二次元图像
    ui->comboBox_presets_model->addItem("logo");  //!< 徽标
    ui->comboBox_presets_model->addItem("style avatar");

    if (0)
    {
        ui->comboBox_presets_model->addItem("sd-v1-4.ckpt");
        ui->comboBox_presets_model->addItem("v1-5-pruned-emaonly");

        ui->comboBox_presets_model->addItem(
            "identityDisorderSD15V1_v15Pruned");
        ui->comboBox_presets_model->addItem("v2-1_768-nonema-pruned");
        ui->comboBox_presets_model->addItem("anything-v5");

        ui->comboBox_presets_model->addItem("sd_xl_base_1.0");
        ui->comboBox_presets_model->addItem("AnythingXL");
        ui->comboBox_presets_model->addItem(
            "sd3_medium_incl_clips_t5xxlfp16");

        ui->comboBox_presets_model->addItem("flux1-dev-Q3_K_S");
        ui->comboBox_presets_model->addItem("null");
    }
    ui->comboBox_presets_model->setCurrentText("Anime image");

    ui->comboBox_run_mode->addItem("text2img");
#if 0 //!< 暂时不开放
    ui->comboBox_run_mode->addItem("img2img");
#endif
    ui->comboBox_run_mode->setCurrentText("text2img");

    {

        // disp_result_img();
        // return ;
        QImage image("_v1-5-pruned-emaonly250723_145135_18467_2.jpg");

        {
            // 2. 创建场景和视图
            QGraphicsScene *scene = new QGraphicsScene;
            QBrush brush(Qt::GlobalColor::black);
            scene->setBackgroundBrush(brush);
            ui->graphicsView->setScene(scene);

            // 3. 将QImage转换为QPixmap并添加到场景
#if 0
            const QGraphicsPixmapItem *pixmapItem = scene->addPixmap(QPixmap::fromImage(image).scaled(QSize(380,380),Qt::KeepAspectRatio));
#endif
            {
                const QGraphicsItem *pg = scene->addPixmap(QPixmap::fromImage(image).scaled(QSize(380, 380), Qt::KeepAspectRatio));
#if 0
                // 4. 调整视图以适应图像大小
                 ui->graphicsView->fitInView(pg, Qt::AspectRatioMode::KeepAspectRatio);
#endif
            }
            ui->graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
        }

        if (0)
        {
            m_cmd.resultImgFile = "2025_07_21_20_44_51_seed_6334_3.jpg";
            // update();
            // 创建一个新的 resize 事件，这里设置新尺寸比当前尺寸大 10x10
            // QResizeEvent *resizeEvent = new QResizeEvent(this->size(), this->size() + QSize(10, 10));
            qDebug() << "----> this->size() = " << this->size();
            QResizeEvent *resizeEvent = new QResizeEvent(this->size() + QSize(100, 100), this->size() - QSize(10, 10));
            // 发送事件到当前窗口
            QApplication::sendEvent(this, resizeEvent);
            // this->sende
        }
    }
}

std::map<QString, QString> MainWindow::languageMap()
{
    static std::map<QString, QString> map = {
        {"English", "en_us"}, {"简体中文", "zh_cn"}, {"繁体中文", "zh_tw"}};
    return map;
}

#include <QTranslator>
void MainWindow::loadTranslation(QString language)
{
    static QTranslator *translator = nullptr;
    if (!translator)
        translator = new QTranslator(this);

    QString qmFile = QString("%1.qm").arg(language);

    const QString &qmDir = qGuiApp->applicationDirPath() + "/translations/";
    auto t1 = qmDir + qmFile;
    if (translator->load(t1))
    {
        bool installstatus = qApp->installTranslator(translator);
        qDebug() << "installstatus = " << installstatus;
        ui->retranslateUi(this);
    }
}

QString MainWindow::toLocaleNameByKey(const QString &language)
{
    const auto &map = languageMap();
    auto it = map.find(language);
    return it != map.cend() ? it->second : "en_us";
}

QString MainWindow::toLocaleNameByVal(const QString &locale)
{
    for (const auto &pair : languageMap())
    {
        if (pair.second == locale)
            return pair.first;
    }

    return "English";
}

void MainWindow::on_cbbLanguage_currentTextChanged(const QString &arg1)
{
    qDebug() << "MainUI::on_cbbLanguage_currentTextChanged, call loadTranslation";

    loadTranslation(toLocaleNameByKey(arg1));
}

MainWindow::~MainWindow()
{
    if (process->state() == QProcess::Running)
    {
        process->kill();
        process->waitForFinished();
    }
    delete ui;
}
#include "commandparser.h"
void MainWindow::on_startButton_clicked()
{
    qDebug() << "invoke start执行开始:";
    ui->outputTextEdit->clear();
    ui->outputTextEdit->append("Starting sd.exe...");

    // 设置工作目录和程序路径
    process->setWorkingDirectory(QCoreApplication::applicationDirPath());

    bool checkifExists = m_cmd.checkIsExists();
    if (!checkifExists)
    {
        qDebug() << "文件不存在";
        return;
    }
    else
    {
        qDebug() << "文件存在性检查已通过.";
    }

    QString cmd_str;
    static QString cmd_str_pre = "";
    //
    //! 指令生成：
    //!  专家界面： 从汇总命令中拉取命令；
    //!  简易界面：从简易界面更新正负提示词；
    //!          cmd显示到专家界面汇总命令；
    if (ui->tabWidget_Editor->currentIndex() == 0)
    {
        m_cmd.PromptText = ui->plainTextEdit_positivePrompts->toPlainText();
        m_cmd.negative_PromptText =
            ui->plainTextEdit_negativePrompts->toPlainText();
        int w = ui->spinBox_W->value();
        int h = ui->spinBox_H->value();
        m_cmd.latentImgWidth = w;
        m_cmd.latentImgHeight = h;
        m_cmd.seed = ui->spinBox_seed->value();
        if (!ui->checkBox_seed_fixed->isChecked())
        {
            m_cmd.seed = rand();
            ui->spinBox_seed->setValue(m_cmd.seed);
        }
        m_cmd.currentDateTime = QDateTime::currentDateTime();

        QDateTime currentDateTime = QDateTime::currentDateTime();
        m_cmd.resultImgFile = dateTimeToString(currentDateTime) + "_seed_" +
                              QString::number(m_cmd.seed) + ".jpg";
        m_cmd.resultJsonFile = dateTimeToString(currentDateTime) + "_seed_" +
                               QString::number(m_cmd.seed) + ".json";

        m_cmd_str = genCmd(m_cmd);
        cmd_str = m_cmd_str;
        if (1)
        {
            ui->plainTextEdit_cmd->clear();
            ui->plainTextEdit_cmd->setPlainText(m_cmd_str);
        }
    }
    else
    {
        cmd_str = ui->plainTextEdit_cmd->toPlainText();
        if (!ui->checkBox_seed_fixed->isChecked())
        {
            if (cmd_str_pre == cmd_str)
            {
                stru_command_string cmd;
                parseCommandLineToJson(cmd_str, cmd);
                cmd.seed = rand();
                m_cmd = cmd;
                ui->spinBox_seed->setValue(cmd.seed);
                m_cmd_str = genCmd(m_cmd);
                cmd_str = m_cmd_str;
            }
        }
    }

    // 获取可执行程序所在的目录
    QString appDir = QCoreApplication::applicationDirPath();
    qDebug() << "可执行程序所在目录:" << appDir;

    ui->outputTextEdit->append("start to invoke cmd: \n " + QString(cmd_str));

    qDebug() << "cmd_str = " << cmd_str;
    qDebug() << "cmd_str_pre = " << cmd_str_pre;
    if (cmd_str_pre == cmd_str)
    {
        qDebug() << "重复的指令，忽略。" << appDir;
        ui->outputTextEdit->append("重复的指令，忽略。");
        return;
    }

    qDebug() << "invoke really start，执行正式开始:";
    // process->start("sd.exe --help");
    process->start(cmd_str);

    if (!process->waitForStarted())
    {
        ui->outputTextEdit->append("Failed to start process!");
        qDebug() << "Error:" << process->errorString();
    }
    else
    {
        ui->startButton->setEnabled(false);
        ui->stopButton->setEnabled(true);
    }
    cmd_str_pre = cmd_str;
}

void MainWindow::on_stopButton_clicked()
{
    if (process->state() == QProcess::Running)
    {
        ui->outputTextEdit->append("Attempting to kill sd.exe...");
        process->kill();
        if (!process->waitForFinished(3000))
        {
            ui->outputTextEdit->append("Failed to kill process!");
        }
    }
}

void MainWindow::handleProcessOutput()
{
    QByteArray stdoutData = process->readAllStandardOutput();
    QByteArray stderrData = process->readAllStandardError();

    if (!stdoutData.isEmpty())
    {
        ui->outputTextEdit->append(QString::fromLocal8Bit(stdoutData));
    }

    if (!stderrData.isEmpty())
    {
        ui->outputTextEdit->append("Error: " + QString::fromLocal8Bit(stderrData));
    }
}

void MainWindow::handleProcessError(QProcess::ProcessError error)
{
    QString errorMsg;
    switch (error)
    {
    case QProcess::FailedToStart:
        errorMsg = "Failed to start: " + process->errorString();
        break;
    case QProcess::Crashed:
        errorMsg = "Process crashed";
        break;
    case QProcess::Timedout:
        errorMsg = "Process timed out";
        break;
    default:
        errorMsg = "Error: " + process->errorString();
    }
    ui->outputTextEdit->append(errorMsg);
}

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>

void MainWindow::disp_result_img()
{
    QString fileName(m_cmd.resultImgFile);
    QImage image(fileName);

    bool fileExists = QFile::exists(m_cmd.resultImgFile);
    if (!fileExists)
    {
#if 0
        qDebug() << "文件不存在: " << m_cmd.resultImgFile;
#endif
        return;
    }

    if (!image.isNull())
    {
        {
#if 0
    QLabel* imageLabel = ui->label;
    // 调整图片大小以适应标签
            QPixmap pixmap = QPixmap::fromImage(image);
            pixmap = pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

            // 在标签上显示图片
            imageLabel->setPixmap(pixmap);
            imageLabel->setToolTip(fileName);
#endif
            // return ;
        }

        qDebug() << "disp_result_img: 加载图像成功：" << m_cmd.resultImgFile;

        // ui->graphicsView->m_image = image;

#if 1
        // 加载并显示图像 \
        // QImage image("K:/MEDIA/2025-03-12_10-34-54_1640.jpg");
        {
            // 2. 创建场景和视图
            QGraphicsScene *scene = new QGraphicsScene(ui->graphicsView);
            QBrush brush(Qt::GlobalColor::black);
            scene->setBackgroundBrush(brush);
            qDebug() << "----> ui->graphicsView->size() = " << ui->graphicsView->size();
            ui->graphicsView->setScene(scene);

            // 3. 将QImage转换为QPixmap并添加到场景
            // const QGraphicsPixmapItem *pixmapItem = scene->addPixmap(QPixmap::fromImage(image));
            const QGraphicsItem *pg = scene->addPixmap(QPixmap::fromImage(image)); //!< .scaled(ui->graphicsView->size())

            qDebug() << "----> ui->graphicsView->size() = " << ui->graphicsView->size();
            qDebug() << "---->pg->scale()= " << pg->scale();

            // 4. 调整视图以适应图像大小
            ui->graphicsView->fitInView(pg, Qt::AspectRatioMode::KeepAspectRatio);
            // ui->graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
        }
        // ui->graphicsView->setScene(scene);
        // ui->openGLWidget->setImage(image);
#endif
    }
    else
    {
#if 0
        imageLabel->setText("无法加载图片");
#endif
        // ui->graphicsView->set("无法加载图片");
        qDebug() << "disp_result_img: 加载图像失败：" << m_cmd.resultImgFile;
        return;
    }

    // 将字符串 "Hello World" 写入到 "data.json" 文件的 "message" 键中
    if (!m_cmd.resultJsonFile.isEmpty())
    {
        bool success =
            writeStringToJsonFile(m_cmd.resultJsonFile, "m_cmd_str", m_cmd_str);
        if (success)
        {
            qDebug() << "save to json: successfull. 写入成功. 文件名：" << m_cmd.resultJsonFile;
        }
        else
        {
            qDebug() << "save to json: failure. 写入失败. 文件名：" << m_cmd.resultJsonFile;
        }
    }
}

void MainWindow::handleProcessFinished(int exitCode,
                                       QProcess::ExitStatus exitStatus)
{
    QString status =
        exitStatus == QProcess::NormalExit ? "Normal exit" : "Crashed";
    ui->outputTextEdit->append(
        QString("Process finished - Exit code: %1, Status: %2")
            .arg(exitCode)
            .arg(status));
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);

    disp_result_img();
}

void MainWindow::init_preset_workflow(const QString &arg1)
{

    if (arg1 == "anime")
    {
        //!< 二次元图像
        preset_animaPencilXL_v500();
        m_cmd.scene_name = "Anime";
        sd_params_up_2_gui();
        saveToJsonFile(m_cmd, m_cmd.resultImgFile + "_command.json");
    }
    else if (arg1 == "logo")
    {
        preset_juggernautXL_juggXIByRundiffusion();
        m_cmd.scene_name = "logo";
        "一个电脑图标，可爱的小牛，扁平化，插画效果，简单的 16 色图标，方形圆角边框的图标，简单的背景色。";
        m_cmd.PromptText = "A computer icon, cute little cow, flattened, illustrated effect, simple 16 color icon, square rounded border icon, simple background color.";
        sd_params_up_2_gui();
        saveToJsonFile(m_cmd, m_cmd.resultImgFile + "_command.json");
        //
        // A、左图提示词：一个暗铜色金属徽章，徽章设计，一只可爱的大熊猫，拟人化，咖啡师，穿着牛仔围裙，拿铁艺术，右手拿着一杯咖啡，斜视观众，一些花朵装饰，黄金和白银的色彩搭配，渐变色，同心圆，咖啡豆图案，文字围绕在一个圆圈中，文字是“Coffee  and  Animal barista”。
        // B、右图提示词：一个亮银色金属徽章，徽章设计，一只可爱的大灰熊，拟人化，钓鱼师，穿着牛仔围裙，右手拿着一根钓鱼竿，面对观众呐喊，一些花朵装饰，亮色白银的搭配，渐变色，同心圆，小鱼小虾的图案，文字围绕在一个圆圈中，文字是“Fishing and Animal Angler”。
        // A、左图提示词：一个电脑图标，可爱的小狮子，扁平化，插画效果，简单的 16 色图标，圆形图标
        // B、右图提示词：一个电脑图标，可爱的小牛，扁平化，插画效果，简单的 16 色图标，方形圆角边框的图标，简单的背景色。
        // 【C、D……更多的徽章例子】
        // A、左图提示词：2D 凶猛的狮子头，矢量插画，愤怒的眼睛，足球队徽标标志，2D 平面风格，居中显示。
        // B、右图提示词：2D 可爱的羊驼头，矢量插画，微笑的眼睛，篮球队徽标标志，2D 平面风格，居中显示。
        // 【C、D……更多的徽章例子】
        // A、左图提示词：模切贴纸，可爱卡哇伊风格，羊驼角色贴纸，插画极简主义，矢量图，简单渐变背景
        // B、右图提示词：模切贴纸，可爱卡哇伊风格，美人鱼角色贴纸，插画极简主义，矢量图，简单渐变背景
        // 【C、D、E、F……更多的徽章例子】
        // 7、图腾标识设计
    }
    else if (arg1 == "style avatar")
    {
        preset_juggernautXL_juggXIByRundiffusion();
        m_cmd.scene_name = "StyleAvatar";
        "构图均分成上下左右排列，水彩画风格6 个不同表情、不同肤色、不同头发颜色、不同相貌、不同服饰、不同发饰的女孩头像，色彩丰富饱满，光线柔和，简单渐变色背景";
        m_cmd.PromptText = "The composition is divided into top, bottom, left, and right arrangements, with a watercolor style. There are six girl portraits with different expressions, skin tones, hair colors, appearances, clothing, and hair accessories. The colors are rich and full, the lighting is soft, and the background is a simple gradient color";
        sd_params_up_2_gui();
        saveToJsonFile(m_cmd, m_cmd.resultImgFile + "_command.json");
    }

    else if (arg1 == "sd-v1-4.ckpt")
    {
        preset_sd_1_4();
    }
    else if (arg1 == "v1-5-pruned-emaonly")
    {
        preset_sd_1_5();
    }
    else if (arg1 == "identityDisorderSD15V1_v15Pruned")
    {
        preset_init_default();
    }

    else if (arg1 == "v2-1_768-nonema-pruned")
    {
        preset_init_default();
    }
    else if (arg1 == "anything-v5")
    {
        preset_Anything_v5();
    }

    else if (arg1 == "sd_xl_base_1.0")
    {
        preset_sdxl_v1_0();
    }
    else if (arg1 == "AnythingXL")
    {
        preset_AnythingXL();
    }

    else if (arg1 == "sd3_medium_incl_clips_t5xxlfp16")
    {
        preset_init_default();
    }
    else if (arg1 == "flux1-dev-Q3_K_S")
    {
        preset_Flux_v1();
    }
    else if (arg1 == "null")
    {
        preset_null();
    }
    else
    {
        qDebug() << "未找到匹配的模型";
        preset_init_default();
    }
}

//! 置空
void MainWindow::preset_null()
{
    qDebug() << "preset_null: ";
    tp_stru_command_string cmd;
    // cmd.set_deault();
    {
        QDateTime currentDateTime = QDateTime::currentDateTime();
        currentDateTime = QDateTime::currentDateTime();

        cmd.resultImgFile = cmd.text2img_output_dir + "/" + cmd.scene_name + "_" + cmd.model_name + dateTimeToString(currentDateTime) + "_" +
                            QString::number(cmd.seed) + ".jpg";
        cmd.resultJsonFile = cmd.text2img_output_dir + "/" + cmd.scene_name + "_" + cmd.model_name + dateTimeToString(currentDateTime) + "_" +
                             QString::number(cmd.seed) + ".json";
    }
    qDebug() << "MainWindow::preset_null,cmd.resultImgFile=" << cmd.resultImgFile;
    qDebug() << "MainWindow::preset_null,cmd.resultJsonFile=" << cmd.resultJsonFile;

    m_cmd = cmd;
    saveToJsonFile(cmd, "preset_null.json");
    m_cmd_str = genCmd(m_cmd);

    if (1)
    {
        ui->plainTextEdit_positivePrompts->setPlainText(cmd.PromptText);
        ui->plainTextEdit_negativePrompts->setPlainText(cmd.negative_PromptText);

        ui->spinBox_W->setValue(cmd.latentImgWidth);
        ui->spinBox_H->setValue(cmd.latentImgHeight);
        ui->spinBox_seed->setValue(cmd.seed);

        ui->plainTextEdit_cmd->clear();
        ui->plainTextEdit_cmd->setPlainText(m_cmd_str);
    }
}

//! 默认配置
void MainWindow::preset_init_default()
{
    tp_stru_command_string cmd;
    cmd.set_deault();

    m_cmd = cmd;
    saveToJsonFile(cmd, "preset_init_default.json");
    m_cmd_str = genCmd(m_cmd);

    qDebug() << "MainWindow::preset_init_default,cmd.resultImgFile=" << cmd.resultImgFile;
    qDebug() << "MainWindow::preset_init_default,cmd.resultJsonFile=" << cmd.resultJsonFile;

    if (1)
    {
        ui->plainTextEdit_positivePrompts->setPlainText(cmd.PromptText);
        ui->plainTextEdit_negativePrompts->setPlainText(cmd.negative_PromptText);

        ui->spinBox_W->setValue(cmd.latentImgWidth);
        ui->spinBox_H->setValue(cmd.latentImgHeight);
        ui->spinBox_seed->setValue(cmd.seed);

        ui->plainTextEdit_cmd->clear();
        ui->plainTextEdit_cmd->setPlainText(m_cmd_str);
    }
}

//!
//! \brief MainWindow::preset_sd_1_4
//! sd-v1-4.ckpt
//! animuwultimate145_v1.4.5.safetensors
//!
void MainWindow::preset_sd_1_4()
{
    qDebug() << "click: sd-v1.4";
    tp_stru_command_string cmd;
    cmd.set_deault();

    cmd.checkpoint = models_dir+"\\models\\checkpoints\\stable-diffusion.cpp__weights\\sd-v1-4.ckpt";

    // mememe, 9c43517a-cf32-4ef8-b866-55fb667da300.jpeg
    {
        cmd.PromptText = "best quality, ultra detailed, beautiful face, perfect anatomy, 1girl, solo, mememe, long hair, aqua hair, blunt bangs, pink eyes, pink dress, purple sleeves, purple thigh highs, <lora:mememe:1>, plain white background,";
        cmd.negative_PromptText =
            "(worst quality, low quality:1.2), easynegative, badhandv4, watermark, logo, cropped hands, extra digit, fewer digit, blurry, greyscale, (text:1.1), title, signature, jpeg artifacts, speech bubble, (multiple_views:1.2), nsfw,";
        cmd.cfg_scale = 6;
        cmd.sampling_method = "dpm++2m";
        cmd.schedule = "karras";
    } //!< 需要抽卡，还可以继续微调；在抽卡的意义上，是能输出了。但是我怀疑这里的基座模型不是这个. 原文没有直接明确提供基座模型的全名，直说是v1-4.

#if 0
    //df3ff4d8-94e8-4c2f-4659-328098073100.jpeg
    /**
     * masterpiece, best quality, 1girl, huge breasts, curvy, Shidare Hotaru,
Negative prompt: EasyNegative, (worst quality, low quality:1.4), mutation, bad_prompt_version2, bad-artist, bad anatomy,
Steps: 20, CFG scale: 7, Sampler: DPM++ 2M Karras, Seed: 3580785812, Size: 576x704, Model hash: d124fc18f0, Denoising strength: 0.4, Clip skip: 2
     */
    if(0)
    {
        cmd.PromptText = "masterpiece, best quality, 1girl, huge breasts, curvy, Shidare Hotaru,";
        cmd.negative_PromptText =
            "EasyNegative, (worst quality, low quality:1.4), mutation, bad_prompt_version2, bad-artist, bad anatomy,";
        cmd.cfg_scale = 7;
        cmd.sampling_method="dpm++2m";
         cmd.schedule = "karras";
    } //!< 提示词太涩了，而且很不稳定，有畸形图，需要抽卡。但是我怀疑这里的基座模型不是这个. 原文没有直接明确提供基座模型的全名，直说是v1-4.
#endif

    // 278984.png
    /**
     *<lora:Shidare_Hotaru:0.7> , 1girl , solo  , sleeveless, :>,black long skirt , anime , sitting , side body , top view , face focus , masterpiece,  best quality,
     *
Negative prompt: (worst quality, low quality:1.4), (artist name, signature, watermark, extra digit, fewer digit, log, error, title, username, multiple views, multiple picture, comic, censored, cut), (loli, child, baby, fat) , drinking , eating ,small breasts ,  ,scrunchie
Steps: 30, CFG scale: 9, Sampler: DPM++ 2M Karras, Seed: 677390102, Size: 488x648, Model: anything-v4.5, Model hash: fbcf965a62, Hires steps: 20, Hires upscale: 2, Hires upscaler: Latent, Face restoration: CodeFormer, Denoising strength: 0.7, Clip skip: 2
     */

    //!  great output.
    /**
 * masterpiece, best quality, 1girl, white hair, medium hair, cat ears, looking at viewer, :3, cute, outdoors, nature, summer, sun, dress
Negative prompt: (worst quality, low quality:1.3), ((blurry)), (((out of frame))), ((ugly)), (((bad anatomy))), gross proportions, ((cloned arms)), ((cloned hands)), ((cross eyed)), (malformed limbs), ((fused fingers)), ((too many fingers)), (poorly drawn hands), (poorly drawn face), jpeg, ((fat)), ((((cropped))))
Steps: 35, CFG scale: 12.5, Sampler: DPM++ 3M SDE Karras, Seed: 4063856139, Size: 512x768, Model: animuwultimate0451, Version: v1.6.0, Model hash: f879184038, Hires upscale: 2, Hires upscaler: Latent, Denoising strength: 0.6
 */
    if (0)
    {
        cmd.checkpoint = "N:/models_comfyUI_J_OnlyBig/models/checkpoints/v1.4.5/animuwultimate145_v1.4.5.safetensors";
        cmd.PromptText = "masterpiece, best quality, 1girl, white hair, medium hair, cat ears, looking at viewer, :3, cute, outdoors, nature, summer, sun, dress";
        cmd.negative_PromptText =
            "(worst quality, low quality:1.3), ((blurry)), (((out of frame))), ((ugly)), (((bad anatomy))), gross proportions, ((cloned arms)), ((cloned hands)), ((cross eyed)), (malformed limbs), ((fused fingers)), ((too many fingers)), (poorly drawn hands), (poorly drawn face), jpeg, ((fat)), ((((cropped))))";
        cmd.cfg_scale = 12.5;
        // cmd.batch_count;//=4;
        cmd.sampling_method = "dpm++2m";
        cmd.schedule = "karras";
        cmd.steps = 35;

    } //!<  great output. 稳定输出，全部ok。

    m_cmd = cmd;
    saveToJsonFile(cmd, "preset_sd_1_4.json");
    m_cmd_str = genCmd(m_cmd);

    if (1)
    {
        ui->plainTextEdit_positivePrompts->setPlainText(cmd.PromptText);
        ui->plainTextEdit_negativePrompts->setPlainText(cmd.negative_PromptText);

        ui->spinBox_W->setValue(cmd.latentImgWidth);
        ui->spinBox_H->setValue(cmd.latentImgHeight);
        ui->spinBox_seed->setValue(cmd.seed);

        ui->plainTextEdit_cmd->clear();
        ui->plainTextEdit_cmd->setPlainText(m_cmd_str);
    }
}

// text2img
//! sd-v1.5
/**
 *
 * 512 -H  512  -v -o 2025_07_18_11_37_12.jpg -s 41
 */
void MainWindow::preset_sd_1_5()
{
    qDebug() << "click: sd-v1.5";
    tp_stru_command_string cmd;

    cmd.set_deault();

    cmd.sampling_method = "dpm++2m";
    cmd.schedule = "karras";
    cmd.steps = 36;               // 30;
    cmd.cfg_scale = 7;            // 12.5;
    cmd.unoising_strength = 0.85; // 0.7;//0.90;

    // sd_043.png
    if (0)
    {
        cmd.PromptText = "a close-up a young woman with vibrant red hair and striking blue eyes, she's wearing a cozy  beanie, her expression is thoughtful as she gazes off to the side, lighting is soft and diffused, the background is a simple gray wall";
        cmd.negative_PromptText =
            "perfect lighting, unnatural lighting, fake glossy skin, plastic skin, doll, like plastic, blurry, painting, 3d, anime, cartoon, tiling, deformed, text, glitch, jpeg artifacts, oversaturated, unrealistic";
    }

    // sd_050.png
    {
        cmd.PromptText =
            "analog photography a quiet cobblestone alleyway in autumn, ((low angle view)), golden leaves scattered, warm sunset light, vibrant colors, soft light, (light leak), shallow depth of field";
        cmd.negative_PromptText =
            "digital art, CGI, 3D render, HDR, oversaturated, sharp detail, anime, perfect lighting, unnatural lighting, high contrast";
    };

    // 74192f91-b7d5-4569-815a-077d04f00e6c.png
    {
        cmd.PromptText = "Pine forest, (full view), sunset, sun setting behind pine trees, warm light, (light leak:1.2), shallow depth of field, cinematic warm tones, melancholic mood";
        cmd.negative_PromptText =
            "b&w, black and white, monochrome, blurry, painting, 3d, anime, cartoon, tiling, deformed, text, glitch, jpeg artifacts, oversaturated, unrealistic, bad perspective";
    }

    // sd_007.png
    {
        cmd.PromptText = "macro shot of a red rose covered in morning dew, sharp focus, soft background, shallow depth of field, ultra detailed, realistic texture, soft lighting, bokeh";
        cmd.negative_PromptText =
            "blurry, painting, 3d, anime, cartoon, tiling, deformed, text, glitch, jpeg artifacts, oversaturated, unrealistic, bad perspective";
        if (0)
            cmd.batch_count = 8;
    } //!< 效果堪称惊艳；就是需要抽卡。良率还有点低。

    {
        cmd.PromptText = "a large monument rock in desert valley, (long view:1.3), harsh lighting, direct sunlight, blue sky, shallow depth of field, energy mood";
        cmd.negative_PromptText = "";
        cmd.batch_count = 4;
    } //!< 这个提示词良率达到了75%。

    m_cmd = cmd;
    saveToJsonFile(cmd, "preset_sd_1_5.json");
    m_cmd_str = genCmd(m_cmd);

    if (1)
    {
        ui->plainTextEdit_positivePrompts->setPlainText(cmd.PromptText);
        ui->plainTextEdit_negativePrompts->setPlainText(cmd.negative_PromptText);

        ui->spinBox_W->setValue(cmd.latentImgWidth);
        ui->spinBox_H->setValue(cmd.latentImgHeight);
        ui->spinBox_seed->setValue(cmd.seed);

        ui->plainTextEdit_cmd->clear();
        ui->plainTextEdit_cmd->setPlainText(m_cmd_str);
    }
}

/**
#5GB sd-v1-4.ckpt
 * M:\proj_StableDiffusionQEnc\StableDiffusionQEnc_0.1_Windows_MSVC_CXX_19.29.30159.0_17_x64_Windows-10.0.26100_bin\sd.exe  -m  E:\modles_comfyUI_G_SimpleModels\models\checkpoints\stable-diffusion.cpp__weights\sd-v1-4.ckpt -p "a lovely cat"

 # 7gb  sd_xl_base_1.0.safetensors
 M:\proj_StableDiffusionQEnc\StableDiffusionQEnc_0.1_Windows_MSVC_CXX_19.29.30159.0_17_x64_Windows-10.0.26100_bin\sd.exe
-m    D:\models_comfyUI_D_BIG\models\StableDiffusion\sd_xl_base_1.0.safetensors
--vae H:\models_comfyUI_G\models\vae\sdxl-vae-fp16-fix.safetensors  -H 1024 -W
1024 -p "a lovely cat" -v

 #6gb  flux1-dev-Q3_K_S.gguf
  M:\proj_StableDiffusionQEnc\StableDiffusionQEnc_0.1_Windows_MSVC_CXX_19.29.30159.0_17_x64_Windows-10.0.26100_bin\sd.exe
--diffusion-model F:\models_L\models\unet\checkpoints_Unet\flux1-dev-Q3_K_S.gguf
--vae  M:\models_comfyUI_TieGuoDun202410_BIG_indep\models\vae\ae.sft   --clip_l
M:\models_comfyUI_TieGuoDun202410_BIG_indep\models\clip\clip_l.safetensors
--t5xxl N:\models_comfyUI_J_OnlyBig\models\clip\t5xxl_fp16.safetensors   -p "a
lovely cat holding a sign says 'flux.cpp'" --cfg-scale 1.0 --sampling-method
euler -v

 # 16Gb
// sd3_medium_incl_clips_t5xxlfp16.safetensors
 M:\proj_StableDiffusionQEnc\StableDiffusionQEnc_0.1_Windows_MSVC_CXX_19.29.30159.0_17_x64_Windows-10.0.26100_bin\sd.exe
-m
E:\modles_comfyUI_G_SimpleModels\models\checkpoints\stable-diffusion.cpp__weights\sd3_medium_incl_clips_t5xxlfp16.safetensors
-H 1024 -W 1024 -p 'a lovely cat holding a sign says \"Stable Diffusion CPP\"'
--cfg-scale 4.5 --sampling-method euler -v
 */

/**
 // * flux1-dev-Q3_K_S.gguf
M:\proj_StableDiffusionQEnc\StableDiffusionQEnc_0.1_Windows_MSVC_CXX_19.29.30159.0_17_x64_Windows-10.0.26100_bin\sd.exe
--diffusion-model F:\models_L\models\unet\checkpoints_Unet\flux1-dev-Q3_K_S.gguf
--vae  M:\models_comfyUI_TieGuoDun202410_BIG_indep\models\vae\ae.sft   --clip_l
M:\models_comfyUI_TieGuoDun202410_BIG_indep\models\clip\clip_l.safetensors
--t5xxl N:\models_comfyUI_J_OnlyBig\models\clip\t5xxl_fp16.safetensors   -p "a
lovely cat holding a sign says 'flux.cpp'" --cfg-scale 1.0 --sampling-method
euler -v
*/

//! anything-v5
void MainWindow::preset_Anything_v5()
{
    qDebug() << "click: anythingv5";
    tp_stru_command_string cmd;
    cmd.set_deault();
    cmd.checkpoint = models_dir+"\\models\\checkpoints\\anything-v5.safetensors";

    cmd.vae = "";


    cmd.PromptText = "1 girl,";
    cmd.negative_PromptText = "bad hands, text, error, missing fingers, extra digit, fewer digits, cropped, worst quality, low quality, normal quality, jpeg artifacts, signature, watermark, username, blurry, Missing limbs, three arms, bad feet, text font ui, signature, blurry, malformed hands, long neck, mutated hands and fingers :1.5).(long body :1.3),(mutation ,poorly drawn :1.2), disfigured, malformed, mutated, multiple breasts, futa, yaoi, three legs, huge breasts,";

    cmd.cfg_scale = 7;
    cmd.sampling_method = "euler_a";
    cmd.schedule = "karras";

    m_cmd = cmd;
    saveToJsonFile(cmd, "preset_Anything_v5.json");
    m_cmd_str = genCmd(m_cmd);

    {
        ui->plainTextEdit_positivePrompts->setPlainText(cmd.PromptText);
        ui->plainTextEdit_negativePrompts->setPlainText(cmd.negative_PromptText);

        ui->spinBox_W->setValue(cmd.latentImgWidth);
        ui->spinBox_H->setValue(cmd.latentImgHeight);
        ui->spinBox_seed->setValue(cmd.seed);

        ui->plainTextEdit_cmd->clear();
        ui->plainTextEdit_cmd->setPlainText(m_cmd_str);
    }
}

//! juggernautXL_juggXIByRundiffusion
void MainWindow::preset_juggernautXL_juggXIByRundiffusion()
{
    tp_stru_command_string cmd;
    cmd.set_deault();
    cmd.model_name = "juggernautXL_juggXIByRundiffusion";
    cmd.scene_name = "";

    cmd.checkpoint = models_dir+"\\models/checkpoints/juggernautXL_juggXIByRundiffusion.safetensors";
    cmd.vae = "";

    cmd.PromptText = "2D Fierce lion's head,Vector illustration,Angry eyes,Football team logo,2D Plane style,Center display";

    // 7、图腾标识设计
    cmd.PromptText = "一个简单的创意标志，侧面狼头形象，居中，在极简主义的风格，纹身风格，Logo 设计，色块，极简风格";
    cmd.PromptText = "A simple creative logo,Side wolf head image,centre,In a minimalist style,Tattoo style,Logo design,color lump,Minimalist style";

    // cmd.negative_PromptText="bad hands, text, error, missing fingers, extra digit, fewer digits, cropped, worst quality, low quality, normal quality, jpeg artifacts, signature, watermark, username, blurry, Missing limbs, three arms, bad feet, text font ui, signature, blurry, malformed hands, long neck, mutated hands and fingers :1.5).(long body :1.3),(mutation ,poorly drawn :1.2), disfigured, malformed, mutated, multiple breasts, futa, yaoi, three legs, huge breasts,";
    cmd.negative_PromptText = "";

    cmd.cfg_scale = 4;
    cmd.steps = 30;
    cmd.sampling_method = "dpm++2mv2";
    cmd.schedule = "karras";
    cmd.latentImgWidth = 1152;
    cmd.latentImgHeight = 896;
    m_cmd = cmd;
}

void MainWindow::sd_params_up_2_gui()
{

    m_cmd_str = genCmd(m_cmd);

    {
        ui->plainTextEdit_positivePrompts->setPlainText(m_cmd.PromptText);
        ui->plainTextEdit_negativePrompts->setPlainText(m_cmd.negative_PromptText);

        ui->spinBox_W->setValue(m_cmd.latentImgWidth);
        ui->spinBox_H->setValue(m_cmd.latentImgHeight);
        ui->spinBox_seed->setValue(m_cmd.seed);

        ui->plainTextEdit_cmd->clear();
        ui->plainTextEdit_cmd->setPlainText(m_cmd_str);
    }
}

//! preset_animaPencilXL_v500
void MainWindow::preset_animaPencilXL_v500()
{
    tp_stru_command_string cmd;
    cmd.set_deault();

    cmd.checkpoint = models_dir+"/models/checkpoints/animaPencilXL_v500.safetensors";
    cmd.vae = "";

    cmd.PromptText = "1 girl,";
    cmd.negative_PromptText = "bad hands, text, error, missing fingers, extra digit, fewer digits, cropped, worst quality, low quality, normal quality, jpeg artifacts, signature, watermark, username, blurry, Missing limbs, three arms, bad feet, text font ui, signature, blurry, malformed hands, long neck, mutated hands and fingers :1.5).(long body :1.3),(mutation ,poorly drawn :1.2), disfigured, malformed, mutated, multiple breasts, futa, yaoi, three legs, huge breasts,";

    cmd.cfg_scale = 6;
    cmd.steps = 30;
    cmd.sampling_method = "dpm++2m";
    cmd.schedule = "karras";
    cmd.latentImgWidth = 1152;
    cmd.latentImgHeight = 896;
    m_cmd = cmd;
}

// sd_xl_base_1.0
void MainWindow::preset_sdxl_v1_0()
{
    qDebug() << "click: sdxl10";
    tp_stru_command_string cmd;
    cmd.set_deault();

    cmd.checkpoint = "..\\models\\checkpoints\\sd_xl_base_1.0.safetensors";
    cmd.vae = "..\\models\\vae\\sdxl-vae-fp16-fix.safetensors";

    cmd.steps = 80;
    cmd.cfg_scale = 8;
    cmd.sampling_method = "dpm++2m";

    // ComfyUI_00147_.png
    /**
   * (perfect mix between king kong and red hulk:1.9), destroying city, blood, epic, cataclysmic, gargantuan, absurdres, best quality, realistic style
Negative prompt: (((3d, low quality, worst quality, blurry,  mutation, deformed hands))), ( three arms or more, three hands or more, three legs or more, three feet or more, comics style:1.6)

Steps: 80, CFG scale: 8, Sampler: DPM++ 2M, Seed: 116224976804502, Model: sd_xl_base_1.0, width: 1024, height: 1592
   */
    cmd.PromptText = "(perfect mix between king kong and red hulk:1.9), destroying city, blood, epic, cataclysmic, gargantuan, absurdres, best quality, realistic style";
    cmd.negative_PromptText = "(((3d, low quality, worst quality, blurry,  mutation, deformed hands))), ( three arms or more, three hands or more, three legs or more, three feet or more, comics style:1.6)";

    // ComfyUI_00106_
    /**
   * (perfect mix between venom and king ghidorah:1.9), with venom's black tentacles protruding from his entire body, destroying city, blood, epic, cataclysmic, gargantuan, absurdres, best quality, realistic style
Negative prompt: 3d, low quality, worst quality,  mutation, deformed hands, ( three arms or more, three hands or more, three legs or more, three feet or more, comics style:1.6)
Steps: 80, CFG scale: 8, Sampler: DPM++ 2M, Seed: 534104825344456, Model: sd_xl_base_1.0, width: 1024, height: 1592
   */
    {
        cmd.PromptText = "(perfect mix between venom and king ghidorah:1.9), with venom's black tentacles protruding from his entire body, destroying city, blood, epic, cataclysmic, gargantuan, absurdres, best quality, realistic style";
        cmd.negative_PromptText = "3d, low quality, worst quality,  mutation, deformed hands, ( three arms or more, three hands or more, three legs or more, three feet or more, comics style:1.6)";
    } //!< 效果太尴尬。基座模型应该不是这个，或者加了其它模型，没有给出来。
    //!< sdxl-v1.0没有实际意义，只有科研、测试的意义。

    m_cmd = cmd;
    saveToJsonFile(cmd, "preset_sdxl_v1_0.json");
    m_cmd_str = genCmd(m_cmd);

    {
        ui->plainTextEdit_positivePrompts->setPlainText(cmd.PromptText);
        ui->plainTextEdit_negativePrompts->setPlainText(cmd.negative_PromptText);

        ui->spinBox_W->setValue(cmd.latentImgWidth);
        ui->spinBox_H->setValue(cmd.latentImgHeight);
        ui->spinBox_seed->setValue(cmd.seed);

        ui->plainTextEdit_cmd->clear();
        ui->plainTextEdit_cmd->setPlainText(m_cmd_str);
    }
}

// AnythingXL_xl
void MainWindow::preset_AnythingXL()
{
    qDebug() << "click: anythingSDXLv5";
    tp_stru_command_string cmd;
    cmd.set_deault();

    cmd.checkpoint = models_dir+"\\models\\checkpoints\\AnythingXL_xl.safetensors";
    cmd.vae = models_dir+"\\models\\vae\\sdxl-vae-fp16-fix.safetensors";
    m_cmd.latentImgWidth = 1024;
    m_cmd.latentImgHeight = 1024;

    /**
   * (masterpiece), perfect anatomy, intricate, (highly detailed), masterpiece, photorealistic, perfect anatomy, cinematic lighting, shading, super detailed skin, beautiful detailed eyes, best quality, ultra-detailed, (illustration), ultra-detailed, looking at viewer, (extremely delicate eyes:1.3), wolf girl, wolf ears, wolf tail, (silver fur), bodice, silk leggings, (form-fitting fabric in vibrant colours), (extremely detailed bodice), (tribal tattoos:1.1), (extremely detailed necklace:1.1), (blue and golden robe embroidery:1.3), (lustrous fur), (red eyes), (galaxy landscape), (moon background), fantasy theme, indie aesthetic, smile, dynamic pose, vibrant colour, shadow, contrast, refraction, perspective, depth, good anatomy, stunning details
Negative prompt: badhandv4, EasyNegativeV2, ng_deepnegative_v1_75t, lowres, bad anatomy, bad hands, mutated hand, text, error, missing fingers, extra digit, fewer digits, cropped, worst quality, low quality, normal quality, jpeg artifacts, signature, watermark, username, blurry, artist name, out of focus, (((multiple views))), (((bad proportions))), (((multiple legs))), (((multiple arms))), 3D, bad_prompt, (worst quality:2.0), (low quality:2.0), inaccurate limb, lowres, bad anatomy, bad hands, text, error, missing fingers, extra digit, fewer digits, cropped, worst quality, low quality, normal quality, jpeg artifacts, signature, watermark, username, blurry, artist name, tall, (painting by bad-artist-anime:0.9), (painting by bad-artist:0.9), bad-prompt:0.5, watermark, text, error, blurry, jpeg artifacts, cropped, normal quality, jpeg artifacts, signature, watermark, username, artist name
Steps: 28, CFG scale: 7, Sampler: Euler a, Seed: 735498127, Size: 832x1216, Clip skip: 2
   */
    cmd.PromptText = "(masterpiece), perfect anatomy, intricate, (highly detailed), masterpiece, photorealistic, perfect anatomy, cinematic lighting, shading, super detailed skin, beautiful detailed eyes, best quality, ultra-detailed, (illustration), ultra-detailed, looking at viewer, (extremely delicate eyes:1.3), wolf girl, wolf ears, wolf tail, (silver fur), bodice, silk leggings, (form-fitting fabric in vibrant colours), (extremely detailed bodice), (tribal tattoos:1.1), (extremely detailed necklace:1.1), (blue and golden robe embroidery:1.3), (lustrous fur), (red eyes), (galaxy landscape), (moon background), fantasy theme, indie aesthetic, smile, dynamic pose, vibrant colour, shadow, contrast, refraction, perspective, depth, good anatomy, stunning details";
    cmd.negative_PromptText = "badhandv4, EasyNegativeV2, ng_deepnegative_v1_75t, lowres, bad anatomy, bad hands, mutated hand, text, error, missing fingers, extra digit, fewer digits, cropped, worst quality, low quality, normal quality, jpeg artifacts, signature, watermark, username, blurry, artist name, out of focus, (((multiple views))), (((bad proportions))), (((multiple legs))), (((multiple arms))), 3D, bad_prompt, (worst quality:2.0), (low quality:2.0), inaccurate limb, lowres, bad anatomy, bad hands, text, error, missing fingers, extra digit, fewer digits, cropped, worst quality, low quality, normal quality, jpeg artifacts, signature, watermark, username, blurry, artist name, tall, (painting by bad-artist-anime:0.9), (painting by bad-artist:0.9), bad-prompt:0.5, watermark, text, error, blurry, jpeg artifacts, cropped, normal quality, jpeg artifacts, signature, watermark, username, artist name";

    cmd.steps = 28;
    cmd.cfg_scale = 7;               // CFG scale: 7;
    cmd.sampling_method = "euler_a"; // ampler: Euler a, Seed: 735498127, Size: 832x1216, Clip skip: 2;

    m_cmd = cmd;
    saveToJsonFile(cmd, "preset_AnythingXL.json");
    m_cmd_str = genCmd(m_cmd);

    // # sdxl 1.0
    if (1)
    {
        ui->plainTextEdit_positivePrompts->setPlainText(cmd.PromptText);
        ui->plainTextEdit_negativePrompts->setPlainText(cmd.negative_PromptText);

        ui->spinBox_W->setValue(cmd.latentImgWidth);
        ui->spinBox_H->setValue(cmd.latentImgHeight);
        ui->spinBox_seed->setValue(cmd.seed);

        ui->plainTextEdit_cmd->clear();
        ui->plainTextEdit_cmd->setPlainText(m_cmd_str);
    }
}

//! flux1-dev-Q3_K_S.gguf
void MainWindow::preset_Flux_v1()
{
    qDebug() << "click: flux1";
    tp_stru_command_string cmd;
    cmd.set_deault();
    cmd.checkpoint = models_dir+"\\models\\unet\\checkpoints_Unet\\flux1-dev-Q3_K_S.gguf";
    cmd.vae = models_dir+"\\models\\vae\\ae.sft";
    cmd.clip_l = models_dir+"\\models\\clip\\clip_l.safetensors";
    cmd.t5xxl = models_dir+"\\models\\clip\\t5xxl_fp16.safetensors";
    m_cmd.PromptText =
        "master piece,4k,high res,a lovely cat holding a sign says 'flux.cpp'";
    cmd.latentImgWidth = 1024;
    cmd.latentImgHeight = 1024;
    cmd.sampling_method = "euler";
    cmd.cfg_scale = 1.0;
    cmd.seed = rand();
    m_cmd = cmd;
    saveToJsonFile(cmd, "preset_Flux_v1.json");
    m_cmd_str = genCmd(m_cmd, SD_Model_type::FLUX_1_0);

    if (1)
    {
        ui->plainTextEdit_positivePrompts->setPlainText(cmd.PromptText);
        ui->plainTextEdit_negativePrompts->setPlainText(cmd.negative_PromptText);

        ui->spinBox_W->setValue(cmd.latentImgWidth);
        ui->spinBox_H->setValue(cmd.latentImgHeight);
        ui->spinBox_seed->setValue(cmd.seed);

        ui->plainTextEdit_cmd->clear();
        ui->plainTextEdit_cmd->setPlainText(m_cmd_str);
    }
}

void MainWindow::on_comboBox_presets_model_currentTextChanged(
    const QString &arg1)
{
    init_preset_workflow(arg1);
}

#include <QFileDialog>

void MainWindow::on_comboBox_run_mode_currentTextChanged(const QString &arg1)
{
    if (arg1 == "text2img")
    {
        ui->lineEdit->hide();
        ui->pushButton_InputImg->hide();
    }
    else if (arg1 == "img2img")
    {
        ui->lineEdit->show();
        ui->pushButton_InputImg->show();
    }
    else
    {
        ui->lineEdit->hide();
        ui->pushButton_InputImg->hide();
    }
}

void MainWindow::on_pushButton_InputImg_clicked()
{
    // QString openfiledi
    QString openfile = QFileDialog::getOpenFileName(
        this, "选择图片", "", "图片文件 (*.jpg *.png *.jpeg)");
    if (openfile.isEmpty())
    {
        qDebug() << "未选择图片";
        return;
    }
    ui->lineEdit->setText(openfile);
    qDebug() << "选择的图片:" << openfile;
    m_cmd.PromptImgFile = openfile;
    ui->outputTextEdit->append("选择的图片:" + m_cmd.PromptImgFile);

    {
        // QString fileName(m_cmd.PromptImgFile);//"output.png");
        QImage image(m_cmd.PromptImgFile);

        if (!image.isNull())
        {
            // 2. 创建场景和视图
            QGraphicsScene *scene = new QGraphicsScene;
            QBrush brush(Qt::GlobalColor::blue);
            scene->setBackgroundBrush(brush);
            ui->graphicsView->setScene(scene);

            // 3. 将QImage转换为QPixmap并添加到场景
            const QGraphicsPixmapItem *pixmapItem =
                scene->addPixmap(QPixmap::fromImage(image));
            const QGraphicsItem *pg = scene->addPixmap(QPixmap::fromImage(image));

            // 4. 调整视图以适应图像大小
            ui->graphicsView->fitInView(pg, Qt::AspectRatioMode::KeepAspectRatio);
            ui->graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
        }
    }
}

// 重写窗口大小改变事件
void MainWindow::resizeEvent(QResizeEvent *event)
{

    // 调用基类的事件处理函数
    QMainWindow::resizeEvent(event);

    // 获取窗口新的大小
    int width = event->size().width();
    int height = event->size().height();

    // 处理缩放逻辑
    handleWindowResized(width, height);
}

// 处理窗口缩放的函数
void MainWindow::handleWindowResized(int width, int height)
{
    // 这里添加你的缩放响应代码
    qDebug() << "窗口大小已改变:" << width << "x" << height;

    // 例如：调整子控件的大小或位置
    // ui->centralWidget->layout()->setGeometry(QRect(0, 0, width, height));
    // showMatOnLabel(m_img,ui->label);
    disp_result_img();
}

#include <QMessageBox>

void MainWindow::on_tabWidget_Editor_currentChanged(int index)
{
    if (index == 0)
    {
        ui->tabWidget_Editor->setCurrentIndex(0);
    }
    else if (index == 1)
    {
        ui->tabWidget_Editor->setCurrentIndex(1);
    }
}
