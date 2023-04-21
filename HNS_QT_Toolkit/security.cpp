#include "security.h"
#include "hns_qt_toolkit.h"
#include <QCryptographicHash>
#include <QTextStream>

unsigned char getCRC(unsigned char message[], unsigned char length);

bool CheckBackdoorPassword(const QString &password)
{
    QString tempstring;
    QTextStream out(&tempstring);
    QDate date = QDate::currentDate();
    std::vector <unsigned char> packet(2,0);
    unsigned char dayOfMonth = static_cast<unsigned char>(date.day());
    unsigned char pwU8 = 0;
    unsigned char pwL8 = 0;

    packet[0]= dayOfMonth/10;
    packet[1] = dayOfMonth - (packet[0] * 10);
    pwU8 = getCRC(packet.data(),2);

    packet[0]= (dayOfMonth + 15)/20;
    packet[1] = dayOfMonth*2  - (packet[0] * 11);
    pwL8 = getCRC(packet.data(),2);

    out << pwU8 << pwL8;

    return (password == tempstring);
}

unsigned char getCRC(unsigned char message[], unsigned char length)
{
    __uint8_t CRC7_POLY = 0x91;
    __uint8_t i, j, crc = 0;

    for (i = 0; i < length; i++)
    {
        crc ^= message[i];
        for (j = 0; j < 8; j++)
        {
            if (crc & 1)
                crc ^= CRC7_POLY;
            crc >>= 1;
        }
    }
    return crc;
}

HNS_Password::HNS_Password()
{
}

void HNS_Password::fAddUser(const QString &user, const type_user_level &level, const QString &pass)
{
    f_usernames.push_back(user);
    f_user_levels.push_back(level);
    f_hash_arrays.push_back(QCryptographicHash::hash(pass.toUtf8(),QCryptographicHash::Sha3_256));
}

void HNS_Password::fAddUserInternal(const QString &user, const type_user_level &level, const QByteArray &hash)
{
    f_usernames.push_back(user);
    f_user_levels.push_back(level);
    f_hash_arrays.push_back(hash);
}

bool HNS_Password::fLoadFile(const QByteArray &input)
{
    int i;
    int j,k;
    bool success = true;
    int index = 0;
    QString tempstring;
    QByteArray temp_array;
    type_user_level user_level = HNS_ADMIN;
    int hash_size;
    int num_records;

    //nothing present
    if(input.size() == 0)
    {
        return false;
    }

    //clear old passwords
    f_usernames.clear();
    f_user_levels.clear();
    f_hash_arrays.clear();

    //grab header
    while(input[index] != 0x0)
    {
        tempstring += input[index];
        index++;
    }
    if(tempstring != "HNS_Passwords")
    {
        return false;
    }
    index++;

    temp_array.clear();
    //grab version
    for(i=0;i<4;i++)
    {
        temp_array.append(input[index]);
        index++;
    }
    if(ByteArrayToInt(temp_array) != 10)
    {
        return false;
    }

    temp_array.clear();
    //Get number of records.
    for(i=0;i<4;i++)
    {
        temp_array.append(input[index]);
        index++;
    }
    num_records = ByteArrayToInt(temp_array);

    for(j=0;j<num_records;j++)
    {
        tempstring.clear();
        //load username
        while(input[index] != 0x0)
        {
            tempstring += input[index];
            index++;
        }
        index++;

        //load user level
        user_level = static_cast<type_user_level>(input[index]);
        index++;

        temp_array.clear();
        //load hash size
        for(i=0;i<4;i++)
        {
            temp_array.append(input[index]);
            index++;
        }
        hash_size = ByteArrayToInt(temp_array);

        temp_array.clear();
        //load password hash
        for(k=0;k<hash_size;k++)
        {
            temp_array.append(input[index]);
            index++;
        }

        fAddUserInternal(tempstring,user_level,temp_array);
    }

    return success;
}

QByteArray HNS_Password::fSaveFile()
{
    int i;
    QByteArray result;

    result += "HNS_Passwords";
    //use 0 as NULL terminator for strings
    result.append(static_cast<char>(0x0));
    //version 1.0, stored as '10'
    result.append(IntToByteArray(10));

    result.append(IntToByteArray(f_usernames.size()));

    for(i=0;i<f_usernames.size();i++)
    {
        result.append(f_usernames[i]);
        result.append(static_cast<char>(0x0));

        result.append(f_user_levels[i]);

        result.append(IntToByteArray(f_hash_arrays[i].size()));

        result.append(f_hash_arrays[i]);
    }

    return result;
}

bool HNS_Password::fVerifyUser(const QString &user, const QString &pass, type_user_level &access)
{
    bool result = false;
    int i;
    QByteArray hash;

    for(i=0;i<f_usernames.size();i++)
    {
        if(f_usernames[i] == user)
        {
            hash = QCryptographicHash::hash(pass.toUtf8(),QCryptographicHash::Sha3_256);
            if(hash == f_hash_arrays[i])
            {
                access = f_user_levels[i];
                return true;
            }
        }
    }
    access = HNS_VIEWER;
    return result;
}

int HNS_Password::fGetNumUsers()
{
    return f_usernames.size();
}

QString HNS_Password::fGetUserName(const int &index)
{
    if(index >= 0 && index < f_usernames.size())
    {
        return f_usernames[index];
    }
    return "";
}

type_user_level HNS_Password::fGetUserAccess(const int &index)
{
    if(index >= 0 && index < f_user_levels.size())
    {
        return f_user_levels[index];
    }
    return HNS_VIEWER;
}

void HNS_Password::fChangeUserName(const int &index, const QString &pass)
{
    if(index >= 0 && index < f_usernames.size())
    {
        f_usernames[index] = pass;
    }
}

void HNS_Password::fChangeUserAcceess(const int &index, const type_user_level &access)
{
    if(index >= 0 && index < f_user_levels.size())
    {
        f_user_levels[index] = access;
    }
}

void HNS_Password::fChangePass(const int &index, const QString &pass)
{
    if(index >= 0 && index < f_hash_arrays.size())
    {
        f_hash_arrays[index] = QCryptographicHash::hash(pass.toUtf8(),QCryptographicHash::Sha3_256);
    }
}

HNS_Security::HNS_Security()
{

}

bool HNS_Security::fLoadFile(const QByteArray &input)
{
    bool success = true;

    success = HNS_Password::fLoadFile(input);

    if(success)
    {
        success = fValidateUsers();
    }

    return success;
}

void HNS_Security::fSetUserName(const QString &name)
{
    int i;

    for(i=0;i<fGetNumUsers();i++)
    {
        fChangeUserName(i,name);
    }
}

void HNS_Security::fSetPass(const type_user_level &access, const QString &pass)
{
    int i;

    for(i=0;i<fGetNumUsers();i++)
    {
        if(fGetUserAccess(i) == access)
        {
            fChangePass(i,pass);
            break;
        }
    }
}

void HNS_Security::fSetAdminPass(const QString &pass)
{
    fSetPass(HNS_ADMIN,pass);
}

void HNS_Security::fSetUserPass(const QString &pass)
{
    fSetPass(HNS_USER,pass);
}

void HNS_Security::fSetViewerPass(const QString &pass)
{
    fSetPass(HNS_VIEWER,pass);
}

bool HNS_Security::fValidateUsers()
{
    bool valid = true;
    int i;
    QString username;
    bool found_viewer = false,found_user = false,found_admin = false;

    //Should only be 3 users.
    valid = (fGetNumUsers() == 3);

    if(valid)
    {
        //All user names should be the same
        for(i=0;i<fGetNumUsers();i++)
        {
            if(i == 0)
            {
                username = fGetUserName(i);
            }
            else
            {
                valid = valid & (username == fGetUserName(i));
            }
        }
    }

    //should be only one user of each category, viewer user and admin.
    if(valid)
    {
        for(i=0;i<fGetNumUsers();i++)
        {
            switch(fGetUserAccess(i))
            {
            case HNS_USER:
                if(found_user)
                {
                    valid = false;
                }
                else
                {
                    found_user = true;
                }
                break;
            case HNS_ADMIN:
                if(found_admin)
                {
                    valid = false;
                }
                else
                {
                    found_admin = true;
                }
                break;
            case HNS_VIEWER:
                if(found_viewer)
                {
                    valid = false;
                }
                else
                {
                    found_viewer = true;
                }
                break;
            }
        }
    }

    return valid;
}
