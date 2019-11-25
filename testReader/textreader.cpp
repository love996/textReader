#include "textreader.h"
#include <QPainter>
#include <QStyle>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "textparser.h"

TextReader::TextReader(QWidget *parent)
  :QWidget(parent),
   _posHead(0),
   _posTail(0),
   _indexHead(0),
   _indexTail(0)
{
    setTextBackground();
    // reparse();
}

void TextReader::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    if (_textLines.size() > 0) {

        auto &textTop = _textLines[_indexHead];
        auto posTop = textTop.pos.y() - textTop.height - _parser.lineSpace();
        int i;
        for (i = _indexHead; i < _textLines.size(); ++i) {
            auto &text = _textLines[i];
            if (text.pos.y() - posTop > this->size().height() ) {
                _indexTail = i - 1;
                break;
            }
            auto drawPos = text.pos;
            drawPos.setY(drawPos.y() - posTop);
            painter.drawText(drawPos, text.text);
        }
        if ( i == _textLines.size()) {
            _indexTail = i - 1;
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
    _text = _file.readAll();
    clear();
    reparse();
}

void TextReader::reparse()
{
    _parser.init(_text, this->font(), this->size());
    _textLines = _parser.parser();
}

void TextReader::nextPage()
{
    if (_indexTail+1 >= _textLines.size()) return;
    _preIndexHead.push(_indexHead);
    _indexHead = _indexTail;
}

void TextReader::prePage()
{
    if (_preIndexHead.empty()) return;
    _indexHead = _preIndexHead.top();
    _preIndexHead.pop();
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
    _indexHead = 0;
    _indexTail = 0;
    _preIndexHead = std::stack<int>();
}
