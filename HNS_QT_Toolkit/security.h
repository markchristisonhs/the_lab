#ifndef SECURITY_H
#define SECURITY_H

#include <QString>
#include <QVector>
#include <QByteArray>

typedef enum user_level : char
{
    HNS_ADMIN = 0,
    HNS_USER,
    HNS_VIEWER
} type_user_level;

bool CheckBackdoorPassword(const QString &password);

class HNS_Password
{
public:
    HNS_Password();

    void fAddUser(const QString &user, const type_user_level &level, const QString &pass);
    bool fLoadFile(const QByteArray &input);
    QByteArray fSaveFile();

    bool fVerifyUser(const QString &user, const QString &pass, type_user_level &access);
protected:
    int fGetNumUsers();

    QString fGetUserName(const int &index);
    type_user_level fGetUserAccess(const int &index);

    void fChangeUserName(const int &index, const QString &pass);
    void fChangeUserAcceess(const int &index, const type_user_level &access);
    void fChangePass(const int &index, const QString &pass);

    void fAddUserInternal(const QString &user, const type_user_level &level, const QByteArray &hash);
private:
    QVector<QString> f_usernames;
    QVector<type_user_level> f_user_levels;
    QVector<QByteArray> f_hash_arrays;
};

//This is a little confusing.  HNS_Password is a generic container allowing dozens or even of hundreds of unique users with their own passwords and their own access levels.
//HNS_Security is just a wrapper around HNS_Password that enforces "classic" 6M-style behavior;  One user name with three different passwords for different access levels.
class HNS_Security : public HNS_Password
{
public:
    HNS_Security();

    bool fLoadFile(const QByteArray &input);

    void fSetUserName(const QString &name);
    void fSetPass(const type_user_level &access, const QString &pass);
    void fSetAdminPass(const QString &pass);
    void fSetUserPass(const QString &pass);
    void fSetViewerPass(const QString &pass);
private:
    //makes sure f_users follows 6M-style format
    bool fValidateUsers();
    //HNS_Password f_users;
};

#endif // SECURITY_H
