#ifndef HNS_QT_TOOLKIT_H
#define HNS_QT_TOOLKIT_H

#include <QImage>
#include <vector>
#include <QVector>
#include <ctime>
#include <QTime>
#include "graphics.h"

void ConvertQImageToBoolArray(const QImage &input, const QColor &on_color, bool *output, const int &outputsize);
std::vector<unsigned char> ConvertQImageToByteArray(const QImage &input, const QColor &on_color = Qt::white);
void ConvertBoolArrayToQImage(const bool *input,const int &inputsize, const int &width, const int &height, QImage &output, const QColor &on_color);
QImage ConvertByteArrayToQImage(const std::vector<unsigned char> &input, const int &width, const int &height, const QColor &on_color = Qt::white);

QImage ConvertHNS_BitmapToQImage(const HNS_Bitmap &input, const HNS_Color &input_on_color = HNS_Color(255,255,255), const QColor &output_on_color = Qt::white);
HNS_Bitmap ConvertQImagetoHNS_Bitmap(const QImage &input);

QByteArray IntToByteArray(const int &input);
int ByteArrayToInt(const QByteArray &input);

QString RemoveTag(const QString &input, const QString &tag);
QString RemoveTagAtEnd(const QString &input, const QString &tag = "");
QString IsTagAtEnd(const QString &input);
int IsTagInString(const QString &input,const QString &tag);

QString StripTags(const QString &input);

std::tm ConvertQTimetoSTLTime(const QTime &time);
QTime ConvertSTLTimetoQTime(const std::tm &time);
std::tm ConvertQDatetoSTLDate(const QDate &date);
QDate ConvertSTLDatetoQDate(const std::tm &date);

std::tm ConvertQDateTimetoSTLDateTime(const QDateTime &datetime);
QDateTime ConvertSTLDateTimetoQDateTime(const std::tm &datetime);

//flip time is in ms.  So for a half second flip time, this should be 500
QString AnimationToMulti(const QVector<int> &animation, const int &flip_time = 500);
QString AnimationToMulti(const std::vector<int> &animation, const int &flip_time = 500);

#endif // HNS_QT_TOOLKIT_H
