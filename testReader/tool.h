#ifndef TOOL_H
#define TOOL_H
#include <QString>

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
    if (from < 0) from = 0;
    if (to  > text.length()) {
        to = text.length() - 1;
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




#endif // TOOL_H
