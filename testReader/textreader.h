#ifndef TEXTREADER_H
#define TEXTREADER_H

#include <stack>

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include "textparser.h"

class TextReader : public QWidget
{
    Q_OBJECT
public:
    explicit TextReader(QWidget *parent = nullptr);
    virtual void paintEvent(QPaintEvent *event) override;
    void open(const QString &filename);
    void reparse();
    void nextPage();
    void prePage();

private:
    void setTextBackground();
    void clear();

private:
    QFile _file;
    QString _text;
    // QVector<MetricText> _textLines;
    int _posHead;
    int _posTail;
    int _lastPaintPageIndex;
    int _pageIndex;
    TextParser _parser;
};

#endif // TEXTREADER_H
