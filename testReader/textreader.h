#ifndef TEXTREADER_H
#define TEXTREADER_H

#include <QWidget>
#include <QFile>
#include <QTextStream>

class TextReader : public QWidget
{
    Q_OBJECT
public:
    explicit TextReader(QWidget *parent = nullptr);
    virtual void paintEvent(QPaintEvent *event) override;
    void open(const QString &filename);

private:
    void setTextBackground();

private:
    QFile _file;
    QString _text;
    // QTextStream _textStream;
    // QVector<QString> _lines;
};

#endif // TEXTREADER_H
