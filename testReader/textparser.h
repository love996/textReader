#ifndef TEXTPARSER_H
#define TEXTPARSER_H

#include <QString>
#include <QFont>
#include <QPointF>
#include <QSize>

// 文本及显示的位置
struct MetricText
{
    QPointF pos;
    QString text;
};

// 根据文本框，字体计算文本的显示方式
class TextParser
{
public:
    TextParser(const QString &text, const QFont &font, const QSize &size/*, QDevice *device*/);
    QVector<MetricText> parser();
private:
    QString _text;
    QFont _font;
    QSize _size;
    QPaintDevice *_device;
    int _lineSpace;
    int _currY;
};

#endif // TEXTPARSER_H
