#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QList>
#include <QTextCodec>
// #pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _settings("textReader.ini")
{
    ui->setupUi(this);

    QObject::connect(ui->fileOpen, &QAction::triggered, [&]{
       openFile();
    });
//    QString text("我在学Qt");
//    text = text.toUtf8();
//    qDebug() << text;
//    for (auto &ch : text) {
//        qDebug() << ch << QString(ch);
//    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    auto lastPath = _settings.value(lastFileDialogPath).toString();
    qDebug() << "lastPath:" << lastPath;
    QFileDialog dlg(this, "选择文本文件", lastPath);
    auto filename =  dlg.getOpenFileName();
    if ("" == filename) {
        return;
    }

    QFileInfo fileInfo(filename);
    auto dirname = fileInfo.absoluteDir().path();
    _settings.setValue(lastFileDialogPath, dirname);
    qDebug() << "dirname:" << dirname << "filename:" << filename;
    ui->textReader->open(filename);
    QFile file(filename);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QString text = file.readAll();
    text = text.toUtf8();
    ui->textEdit->setText(text);
}

