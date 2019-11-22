#include "textparser.h"

#include <QFontMetricsF>
#include <QDebug>

TextParser::TextParser(const QString &text, const QFont &font, const QSize &size)
    :_text(text),
     _font(font),
     _size(size),
     _lineSpace(5),
     _currY(0)
{
}

QVector<MetricText> TextParser::parser()
{
    QFontMetricsF fm(_font);
    QVector<MetricText> mts;
    // QChar lastCh;
    MetricText mt;
    QRectF textRect;
    // 如果超过控件宽度或者碰到换行符
    for (auto &ch : _text) {
        mt.text.push_back(ch);
        textRect = fm.tightBoundingRect(mt.text);
        if (textRect.width() > _size.width()
         || ch == QChar('\n') || ch == QString("\x0d\x0a")) {
            mt.text.chop(-1);
            mt.pos.setX(0);
            _currY = _currY + _lineSpace + textRect.height();
            mt.pos.setY(_currY);
            mts.push_back(std::move(mt));
            mt.text = QString(ch);
            qDebug() << mt.pos << mt.text << textRect << _size;
        }
    }
    if (mt.text.length() > 0) {
        mt.pos.setX(0);
        _currY = _currY + _lineSpace + textRect.height();
        mt.pos.setY(_currY);
    }
    return mts;
}
