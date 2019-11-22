#include "textreader.h"
#include <QPainter>
#include <QStyle>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "textparser.h"

TextReader::TextReader(QWidget *parent) : QWidget(parent)
{
    setTextBackground();
}

void TextReader::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    TextParser parser(_text, this->font(), this->size());
    auto texts = parser.parser();
//    painter.drawText(0, 0, "hello");
//    painter.drawText(10, 2, "hello");
//     painter.drawText(10, 20, "hello");
    for (auto &text : texts) {
        painter.drawText(text.pos, text.text);
    }

//    if (_textStream.device() != nullptr) {
//        while (!_textStream.atEnd()) {
//            _text = _textStream.readAll();
//        }
//        painter.drawText(QPointF(10, 10), _text);
//    }
    return QWidget::paintEvent(event);
}

void TextReader::open(const QString &filename)
{
    // _textStream.setDevice(nullptr);
    if (_file.isOpen()) {
        _file.close();
    }
    _file.setFileName(filename);
    if (!_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox msg;
        msg.setText("打开文件失败!");
        msg.exec();
    }
    _text = _file.readAll();
    // _textStream.setDevice(&_file);
}

void TextReader::setTextBackground()
{
    // 白色背景
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}
