#include "hns_qt_toolkit.h"

using namespace std;

void ConvertQImageToBoolArray(const QImage &input, const QColor &on_color, bool *output, const int &outputsize)
{
    int x,y;

    if(output == nullptr || outputsize < (input.width()*input.height()))
    {
        return;
    }

    for(x=0;x<input.width();x++)
    {
        for(y=0;y<input.height();y++)
        {
            output[(y*input.width()) + x] = (input.pixelColor(x,y) == on_color);
        }
    }
}

vector<unsigned char> ConvertQImageToByteArray(const QImage &input, const QColor &on_color)
{
    vector<unsigned char> result;
    unsigned char temp;
    int i;

    bool *data = new bool[input.height() * input.width()];

    ConvertQImageToBoolArray(input,on_color,data,input.width() * input.height());

    for(i=0;i<(input.height() * input.width());i++)
    {
        if((i % 8) == 0)
        {
            if(i != 0)
            {
                result.push_back(temp);
            }
            temp = 0;
        }
        else
        {
            temp = temp << 1;
        }

        temp = temp | (data[i] ? 0x1 : 0x0);
    }

    for(;i%8 != 0;i++)
    {
        temp = temp << 1;
    }
    result.push_back(temp);

    delete [] data;
    data = nullptr;

    return result;
}

void ConvertBoolArrayToQImage(const bool *input,const int &/*inputsize*/, const int &width, const int &height, QImage &output, const QColor &on_color)
{
    int x,y;

    QImage result(width,height,QImage::Format_RGB32);

    for(y=0;y<height;y++)
    {
        for(x=0;x<width;x++)
        {
            result.setPixelColor(x,y,input[(y*width) + x] ? on_color : Qt::black);
        }
    }

    output = result;
}

QImage ConvertByteArrayToQImage(const std::vector<unsigned char> &input, const int &width, const int &height, const QColor &on_color)
{
    QImage result(width,height,QImage::Format_RGB32);
    unsigned char temp;
    size_t index;

    bool *data = new bool [width*height];

    temp = 0x00;
    index = 0;
    for(size_t i=0;i<static_cast<size_t>(width*height);i++)
    {
        if((i%8) == 0)
        {
            temp = input[index++];
        }
        else
        {
            temp = temp << 1;
        }
        data[i] = (temp & 0x80);
    }

    ConvertBoolArrayToQImage(data,width*height,width,height,result,on_color);

    delete [] data;
    data = nullptr;

    return result;
}

QImage ConvertHNS_BitmapToQImage(const HNS_Bitmap &input, const HNS_Color &input_on_color, const QColor &output_on_color)
{
    size_t x,y;
    HNS_Color temp_color;
    QImage result(input.fGetWidth(),input.fGetHeight(),QImage::Format_RGB32);

    for(y=0;y<input.fGetHeight();y++)
    {
        for(x=0;x<input.fGetWidth();x++)
        {
            temp_color = input.fGetPixel(x,y);
            result.setPixelColor(x,y,(input.fGetPixel(x,y) == input_on_color) ? output_on_color : Qt::black);
        }
    }

    return result;
}

HNS_Bitmap ConvertQImagetoHNS_Bitmap(const QImage &input)
{
    size_t x,y;
    HNS_Bitmap result(input.height(),input.width());
    QColor temp_qcolor;

    for(y=0;static_cast<int>(y)<input.height();y++)
    {
        for(x=0;static_cast<int>(x)<input.width();x++)
        {
            temp_qcolor = input.pixelColor(x,y);
            result.fSetPixel(x,y,HNS_Color(temp_qcolor.red(),temp_qcolor.green(),temp_qcolor.blue()));
        }
    }

    return result;
}

QByteArray IntToByteArray(const int &input)
{
    QByteArray result;
    int i;

    for(i=0;i<4;i++)
    {
        result.append(static_cast<char>((input & (0xFF << (i*8))) >> (i*8)));
    }

    return result;
}

int ByteArrayToInt(const QByteArray &input)
{
    int result = 0;
    int i,ilimit = min(4,input.size());

    for(i=0;i<ilimit;i++)
    {
        result |= input[i] << (i*8);
    }

    return result;
}

QString RemoveTag(const QString &input, const QString &tag)
{
    QString result = input;
    int beginning,end;

    beginning = input.indexOf(QString("[") + tag, Qt::CaseInsensitive);
    if(beginning >= 0)
    {
        end = input.indexOf("]",beginning, Qt::CaseInsensitive);

        if(end > beginning)
        {
            result.remove(beginning, end - beginning + 1);
        }
    }

    return result;
}

QString RemoveTagAtEnd(const QString &input, const QString &tag)
{
    QString result = input;
    QString temp_tag = IsTagAtEnd(result);
    int i;

    if(!input.isEmpty())
    {
        //tag field is not empty, only delete if the last tag is the desired one.
        if(!tag.isEmpty())
        {
            if(temp_tag.contains(tag, Qt::CaseInsensitive))
            {
                result.remove(result.size() - tag.size(), tag.size());
            }
        }
        //tag field is empty.  Don't care what is the last tag.
        else
        {
            i = result.size() - 1;
            if(result.at(i) == ']')
            {
                do
                {
                    result.chop(1);
                    i--;
                }while (result.at(i) != '[' && !result.isEmpty());
                //no opening brace found, no valid tag present, restore input and return result
                if(result.isEmpty())
                {
                    result = input;
                }
                else
                {
                    result.chop(1); //remove opening brace
                }
            }
        }
    }

    return result;
}

QString IsTagAtEnd(const QString &input)
{
    QString result;
    int ilimit = input.size();
    int i = ilimit - 1;
    bool done = false;

    if(!input.isEmpty())
    {
        if(input.at(i) == ']')
        {
            result = input.at(i);
            do
            {
                i--;
                result.prepend(input.at(i));
                if(input.at(i) == '[')
                {
                    done = true;
                }
            }while(i > 0 && !done);

            if(result.at(0) != '[')
            {
                result = "";
            }
        }
    }

    return result;
}

int IsTagInString(const QString &input, const QString &tag)
{
    return input.indexOf(tag,0,Qt::CaseInsensitive);
}

std::tm ConvertQTimetoSTLTime(const QTime &time)
{
    std::tm result;

    result.tm_hour = time.hour();
    result.tm_min = time.minute();
    result.tm_sec = time.second();

    return result;
}

QTime ConvertSTLTimetoQTime(const std::tm &time)
{
    QTime result;

    result.setHMS(time.tm_hour,time.tm_min,time.tm_sec);

    return result;
}

std::tm ConvertQDatetoSTLDate(const QDate &date)
{
    std::tm result;

    result.tm_mon = date.month() - 1;
    result.tm_mday = date.day();
    result.tm_year = date.year() - 1900;

    return result;
}

QDate ConvertSTLDatetoQDate(const std::tm &date)
{
    QDate result;

    result.setDate(date.tm_year+1900,date.tm_mon+1,date.tm_mday);

    return result;
}

std::tm ConvertQDateTimetoSTLDateTime(const QDateTime &datetime)
{
    std::tm result;

    result.tm_mon = datetime.date().month() - 1;
    result.tm_mday = datetime.date().day();
    result.tm_year = datetime.date().year() - 1900;

    result.tm_hour = datetime.time().hour();
    result.tm_min = datetime.time().minute();
    result.tm_sec = datetime.time().second();

    return result;
}

QDateTime ConvertSTLDateTimetoQDateTime(const std::tm &datetime)
{
    QDateTime result;

    result.setDate(QDate(datetime.tm_year+1900,datetime.tm_mon+1,datetime.tm_mday));
    result.setTime(QTime(datetime.tm_hour,datetime.tm_min,datetime.tm_sec));

    return result;
}

QString AnimationToMulti(const QVector<int> &animation, const int &flip_time)
{
    QString result = QString("[pt%1o0]").arg(max(1,(flip_time/100)));

    for(int i=0;i<animation.size();i++)
    {
        result += QString("[g%1]").arg(animation[i]);
        if(i != animation.size()-1)
        {
            result += "[np]";
        }
    }

    return result;
}

QString AnimationToMulti(const vector<int> &animation, const int &flip_time)
{
    return AnimationToMulti(QVector<int>::fromStdVector(animation), flip_time);
}
