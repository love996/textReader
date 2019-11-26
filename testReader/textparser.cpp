#include "textparser.h"

#include <QFontMetricsF>
#include <QDebug>
#include <algorithm>

#include "tool.h"


TextParser::TextParser()
    :TextParser("", QFont(), QSize())

{

}

TextParser::TextParser(QString &&text, const QFont &font, const QSize &size)
    :_lineSpace(5),
      _currY(0),
      _fm(font),
      _stop{false},
      _done{false},
      _currPageIndex(-1)
{
    setText(std::move(text));
    setFont(font);
    setSize(size);
    // init(_text, font, size);
}

TextParser::~TextParser()
{
    stopParser();
}

void TextParser::setText(QString &&text)
{
    // 先把换行统一
    text.replace("\r\n", "\n");
    _text = std::move(text);
}

void TextParser::setFont(const QFont &font)
{
    _font = font;
    _fm = QFontMetricsF(_font);
    calc();
}

void TextParser::setSize(const QSize &size)
{
    _size = size;
    calc();
}

void TextParser::calc()
{
    if (_size.width() == 0 || _size.height() == 0) return;
    // 先粗略预估一下一行大概有多少字符
    QString En = "abcdefghigklmnopqrstuvwxyz";
    En += En.toUpper();
    QString Zh = "汉字";
    auto enRect = _fm.boundingRect(En);
    auto zhRect = _fm.boundingRect(Zh);
    auto avgEnLen = enRect.width() / En.length();
    auto avgZhLen = zhRect.width()  / Zh.length();

    // 先简单假设每行中英文比例一样
    _avgLen = (avgEnLen + avgZhLen) / 2;
    _avgCount = _size.width() / _avgLen;
    qDebug() << "每行大概" << _avgCount << "个字符";
}

// 解析文本
void TextParser::parser()
{
    // QChar lastCh;
    PageText pt;
    MetricText mt;
    QRectF textRect;
    int posNextLine = 0, posEndLine;

    // 如果超过控件宽度或者碰到换行符
    QString substr;
    for (int pos = 0; pos < _text.length();) {
        if (_stop) return;
        // qDebug() << _text.length() << _text.size() << posNextLine;
        if (_text.length() - posNextLine >= _avgCount) {
            posNextLine += _avgCount;
        }
        else {
            posNextLine = _text.length();
        }
        // 如果未到达边界
        do {
            posNextLine += (_size.width() - textRect.width()) / _avgLen;
            posEndLine = indexOfEndLine(_text, pos, posNextLine);
            if (-1 != posEndLine) {
                posNextLine = posEndLine + 1;
            }
            substr = _text.mid(pos, posNextLine-pos);
            textRect = _fm.boundingRect(substr);
            if (-1 != posEndLine) break;
        } while (textRect.width() < _size.width());

        // 如果超过边界
        while (textRect.width() > _size.width()) {
            int count = (textRect.width() - _size.width()) / _avgLen;
            // 尽可能逼近边界
            if (0 == count) count = 1;
            if (2 <= count) count /= 2;
            substr.chop(count);
            posNextLine -= count;
            textRect = _fm.boundingRect(substr);
        }

        // 记录每行的显示位置
        mt.text = substr;
        mt.pos.setX(0);
        mt.height = textRect.height();
        _currY += _lineSpace + textRect.height();
        mt.pos.setY(_currY);

        pos = posNextLine;

        if (_currY >= _size.height())
        {
            {
                std::lock_guard<std::mutex> lck(_rdlck);
                _sharedBuf.push_back(std::move(pt));
                ++_currPageIndex;
            }

            // 新的一页
            pt = PageText{};
            _currY = 0;
            _currY += _lineSpace + textRect.height();
            mt.pos.setY(_currY);
            pt.lines.push_back(std::move(mt));

        }

        pt.lines.push_back(std::move(mt));


    }
    if (pt.lines.size() > 0) {
        std::lock_guard<std::mutex> lck(_rdlck);
        _sharedBuf.push_back(std::move(pt));
        ++_currPageIndex;
    }
    _done = true;
    return;
}

void TextParser::initThread()
{
    _stop = false;
    _done = false;
    _currY = 0;
    _currPageIndex = -1;
    _sharedBuf.clear();
}



int TextParser::lineSpace() const
{
    return _lineSpace;
}

void TextParser::setLineSpace(int lineSpace)
{
    _lineSpace = lineSpace;
}

void TextParser::stopParser()
{
    if (_thread.joinable()) {
        _stop = true;
        _thread.join();
    }
    qDebug() << "停止解析";
}

void TextParser::threadParser()
{
    stopParser();
    initThread();
    _thread = std::thread([&]{
        parser();
    });
}

int TextParser::length() const
{
    if (_text.length() > 0) {
        while(-1 == _currPageIndex) {
            std::this_thread::yield();
        }
    }
    return _currPageIndex+1;
}

PageText TextParser::getPage(int index)
{
    std::lock_guard<std::mutex> lck(_rdlck);
    return _sharedBuf.at(index);
}

bool TextParser::done() const
{
    return _done;
}
