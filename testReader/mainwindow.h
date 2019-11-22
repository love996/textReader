#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

const QString lastFileDialogPath("last_file_dialog_path");

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QFile _file;
    QSettings _settings;

private:
    // 打开文件
    void openFile();
    // 显示文件
    void showText();
};
#endif // MAINWINDOW_H
