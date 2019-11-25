#ifndef TEXTPARSER_H
#define TEXTPARSER_H

#include <QString>
#include <QFont>
#include <QPointF>
#include <QSize>
#include <QFontMetricsF>

// 文本及显示的位置
struct MetricText
{
    QPointF pos;
    double height;
    QString text;
};

// 根据文本框，字体计算文本的显示方式
class TextParser
{
public:
    TextParser();

    TextParser(const QString &text, const QFont &font, const QSize &size/*, QDevice *device*/);
    void init(const QString &text, const QFont &font, const QSize &size);
    QVector<MetricText> parser();

    int lineSpace() const ;
    void setLineSpace(int lineSpace);

private:
    QString _text;
    QFont _font;
    QSize _size;
    QPaintDevice *_device;
    int _lineSpace;
    int _currY;
    int _avgLen;
    int _avgCount;
    QFontMetricsF _fm;
};

#endif // TEXTPARSER_H
