#ifndef TEXTPARSER_H
#define TEXTPARSER_H

#include <QString>
#include <QFont>
#include <QPointF>
#include <QSize>
#include <QFontMetricsF>
#include <memory>
#include <thread>
#include <mutex>



// 每行文本及显示的位置
struct MetricText
{
    QPointF pos;
    double height;
    QString text;
};

// 每页的文本内容
struct PageText
{
    std::vector<MetricText> lines;
};

// 根据文本框，字体计算文本的显示方式
class TextParser
{
public:
    TextParser();

    TextParser(QString &&text, const QFont &font, const QSize &size/*, QDevice *device*/);
    ~TextParser();
    // void init(const QString &text, const QFont &font, const QSize &size);
    void setText(QString &&text);
    void setFont(const QFont &font);
    void setSize(const QSize &size);
    void threadParser();
    bool done() const;
    int length() const;
    PageText getPage(int index);
    int lineSpace() const ;
    void setLineSpace(int lineSpace);
    bool isDone();

private:
    QString _text;
    QFont _font;
    QSize _size;
    // QPaintDevice *_device;
    int _lineSpace;
    int _currY;
    int _avgLen;
    int _avgCount;
    QFontMetricsF _fm;
    std::vector<PageText> _sharedBuf;
    std::thread _thread;
    std::atomic<bool> _stop;
    std::atomic<bool> _done;
    std::atomic<int> _currPageIndex;
    std::mutex _rdlck;

private:
    void stopParser();
    void parser();
    void initThread();
    void calc();
};

#endif // TEXTPARSER_H
