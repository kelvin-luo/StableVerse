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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class DWCtrl;

#include <QDateTime>
#include <QDebug>
#include <QProcess>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

#include <QFile>

#include "utility.h"

// extern const QString output_dir;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum WDValues
    {
        WD_Stop,
        WD_Start,
        WD_Fore_enable
    };
    Q_ENUM(WDValues)

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // 重写窗口大小改变事件
    void resizeEvent(QResizeEvent *event) override;

private:
    void handleWindowResized(int width, int height);
    void disp_result_img();
private slots:
    void on_startButton_clicked();
    void on_stopButton_clicked();
private slots:
    void handleProcessOutput();
    void handleProcessError(QProcess::ProcessError error);
    void handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

private slots:
    void on_comboBox_presets_model_currentTextChanged(const QString &arg1);

    void on_comboBox_run_mode_currentTextChanged(const QString &arg1);

    void on_pushButton_InputImg_clicked();

    void on_cbbLanguage_currentTextChanged(const QString &arg1);

    void on_tabWidget_Editor_currentChanged(int index);

private:
    void preset_null();
    void preset_init_default();
    void preset_sd_1_4();
    void preset_sd_1_5();
    void preset_sdxl_v1_0();
    void preset_Anything_v5();
    void preset_AnythingXL();
    void preset_Flux_v1();
    void MainWindow::preset_juggernautXL_juggXIByRundiffusion();
    void preset_animaPencilXL_v500();

private:
    Ui::MainWindow *ui;
    QProcess *process;

    QString m_cmd_str;

    tp_stru_command_string m_cmd;

private:
    void init_preset_workflow(const QString &arg1);
    std::map<QString, QString> languageMap();
    QString toLocaleNameByKey(const QString &language);
    QString toLocaleNameByVal(const QString &locale);
    void loadTranslation(QString language);

    // int flag_init_img_disp = false;
    void sd_params_up_2_gui();
};

QString genCmd(tp_stru_command_string cmd, SD_Model_type modelCls = SD_1_5);

#endif // MAINWINDOW_H
