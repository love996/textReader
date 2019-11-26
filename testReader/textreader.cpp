#include "textreader.h"
#include <QPainter>
#include <QStyle>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "textparser.h"

#include <QTextCodec>

QString GetCorrectUnicode(const QByteArray &ba)
{
    QTextCodec::ConverterState state;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString text = codec->toUnicode( ba.constData(), ba.size(), &state);
    //目前假设只有utf8和gbk
    if (state.invalidChars) {
        // qDebug() << "gbk";
        text = QTextCodec::codecForName( "GB18030" )->toUnicode(ba);
    }
    else {
        // qDebug() << "utf8";
        text = ba;
    }
    return text;
}


TextReader::TextReader(QWidget *parent)
  :QWidget(parent),
   _posHead(0),
   _posTail(0),
   _lastPaintPageIndex(-1),
   _pageIndex(0)
{
    setTextBackground();
    // reparse();
}

void TextReader::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    // qDebug() << "length:" << _parser.length() << "index:" << _pageIndex;
    if (_parser.length() > _pageIndex) {
        auto page = _parser.getPage(_pageIndex);
        for (int i = 0; i < page.lines.size(); ++i) {
            // qDebug() << page.lines[i].pos.x() << page.lines[i].pos.y();
            painter.drawText(page.lines[i].pos, page.lines[i].text);
        }
    }

    return QWidget::paintEvent(event);
}

void TextReader::open(const QString &filename)
{
    if (_file.isOpen()) {
        _file.close();
    }
    _file.setFileName(filename);
    if (!_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox msg;
        msg.setText("打开文件失败!");
        msg.exec();
        return;
    }
    auto textByte = _file.readAll();
    // _text = textByte;
    _text = GetCorrectUnicode(textByte);
    clear();
    reparse();
}

void TextReader::reparse()
{
    // _parser.init(_text, this->font(), this->size());
    _parser.setText(QString(_text));
    _parser.setFont(this->font());
    _parser.setSize(this->size());
    _parser.threadParser();
    // _textLines = _parser.getSharedBuf();
}

void TextReader::nextPage()
{
    if (_pageIndex+1 >= _parser.length()) return;
    ++_pageIndex;
}

void TextReader::prePage()
{
    if (0 < _pageIndex) --_pageIndex;
}

void TextReader::setTextBackground()
{
    // 白色背景
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

void TextReader::clear()
{
    _lastPaintPageIndex = -1;
    _pageIndex = 0;
}
