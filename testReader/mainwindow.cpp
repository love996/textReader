#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QList>
#include <QTextCodec>
#include <QFontDialog>
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
    QObject::connect(ui->textFont, &QAction::triggered, [&]{
       setFont();
    });
//    QString text("我在\r\n学\nQt");
//    text.replace("\r\n", "\n");
//    text = text.toUtf8();
//    qDebug() << text  << text.length() << text.size();
//    for (int i = 0; i < text.length(); ++i) {
//        qDebug() << QString(text[i]) << typeid(text[i]).name() << typeid(QChar()).name() ;
//    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    auto lastPath = _settings.value(lastFileDialogPath).toString();
    QFileDialog dlg(this, "选择文本文件", lastPath);
    auto filename =  dlg.getOpenFileName();
    if ("" == filename) {
        return;
    }

    QFileInfo fileInfo(filename);
    auto dirname = fileInfo.absoluteDir().path();
    _settings.setValue(lastFileDialogPath, dirname);
    ui->textReader->open(filename);
    QFile file(filename);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QString text = file.readAll();
    text = text.toUtf8();
    ui->textEdit->setText(text);
}

void MainWindow::setFont()
{
    QFontDialog dlg;
    bool flag;
    auto font = dlg.getFont(&flag);
    ui->textReader->setFont(font);
    ui->textReader->reparse();
}


void MainWindow::on_prePage_clicked()
{
    ui->textReader->prePage();
    ui->textReader->update();
}

void MainWindow::on_nextPage_clicked()
{
    ui->textReader->nextPage();
    ui->textReader->update();
}
