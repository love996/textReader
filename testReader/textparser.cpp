#include "textparser.h"

#include <QFontMetricsF>
#include <QDebug>
#include <Algorithm>

template <typename String>
bool isEndLine(const String &s)
{
    static String endLineUnix("\n");
    static String endLineWin("\r\n");
    return s == endLineUnix || s == endLineWin;
}

template <>
bool isEndLine<QChar>(const QChar &s)
{
    static QString endLineUnix("\n");
    static QString endLineWin("\r\n");
    return s == endLineUnix || s == endLineWin;
}

template <>
bool isEndLine<QCharRef>(const QCharRef &s)
{
    static QString endLineUnix("\n");
    static QString endLineWin("\r\n");
    return s == endLineUnix || s == endLineWin;
}

template <>
bool isEndLine<const char *>(const char * const &s)
{
    static std::string endLineUnix("\n");
    static std::string endLineWin("\r\n");
    return s == endLineUnix || s == endLineWin;
}


// 没有找到区间内查找的函数
int indexOf(const QString &text, const QChar &&v, int from, int to) {
    auto beg = text.begin();
    if (to - from > text.length()) {
        to = from + text.length();
    }
    auto posBeg = beg + from;
    auto posEnd = beg + to;
    auto iter_find = std::find(posBeg, posEnd, v);
    if (iter_find == text.end()) {
        return -1;
    }
    return iter_find - beg;
}

int indexOfEndLine(const QString &text, int from, int to) {
//    int pos = text.indexOf("\n");
//    if (-1 == pos) {
//        return -1;
//    }
//    if (0 == pos) {
//        return pos;
//    }
//    if (text[pos-1] == "QChar('\r')") {
//        return pos -1;
//    }
//    return pos;
    int pos = indexOf(text, QChar('\n'), from, to);
    if (-1 == pos) {
        return -1;
    }
    if (0 == pos) {
        return pos;
    }
    if (text[pos-1] == '\r') {
        return pos -1;
    }
    return pos;
}



TextParser::TextParser()
    :TextParser("", QFont(), QSize())

{

}

TextParser::TextParser(const QString &text, const QFont &font, const QSize &size)
    :_lineSpace(5),
      _currY(0),
      _fm(font)
{
    init(_text, font, size);
}

void TextParser::init(const QString &text, const QFont &font, const QSize &size)
{
    _text = text;
    _font = font;
    _size = size;
    _fm = QFontMetricsF(_font);
    // 先把换行统一
    _text.replace("\r\n", "\n");
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
    _avgCount = size.width() / _avgLen;
}

// 解析文本
QVector<MetricText> TextParser::parser()
{
    QVector<MetricText> mts;
    // QChar lastCh;
    MetricText mt;
    QRectF textRect;
    int posNextLine = 0, posEndLine;

    // 如果超过控件宽度或者碰到换行符
    QString substr;
    for (int pos = 0; pos < _text.length();) {
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
        }while (textRect.width() < _size.width());

        // 如果超过边界
        while (textRect.width() > _size.width()) {
            int count = (textRect.width() - _size.width()) / _avgLen;
            // 尽可能逼近边界
            if (0 == count) count = 1;
            if (2 <= count) count /= 2;
            // qDebug() << count;
            // count = 1;
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
        mts.push_back(std::move(mt));
        pos = posNextLine;
    }

    return mts;
}

int TextParser::lineSpace() const
{
    return _lineSpace;
}

void TextParser::setLineSpace(int lineSpace)
{
    _lineSpace = lineSpace;
}
