#ifndef QTPHONENUMBERSTRING_H
#define QTPHONENUMBERSTRING_H

#include <QtCore>

class QPhoneNumberString : public  QString{
public:
    enum PhoneNumberType {
        SipNumber = 0,
        TouchToneNumber = 1
    };

    QPhoneNumberString();
    QPhoneNumberString(const QString & str);

    bool isUnknownNumberType() const;
    bool isCallableByFB7270() const;
    bool isSipNumber() const;
    bool isTouchToneNumber() const;
private:
};

#endif // QTPHONENUMBERSTRING_H
