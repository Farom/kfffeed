#include "qphonenumberstring.h"


QPhoneNumberString::QPhoneNumberString():
        QString()
{
}

QPhoneNumberString::QPhoneNumberString(const QString & str):
    QString(str)
{}


bool QPhoneNumberString::isCallableByFB7270() const {
    QRegExp regexp("^\\d+$");
    return regexp.exactMatch( *this );
}

bool QPhoneNumberString::isSipNumber() const {

    return false;
}
bool QPhoneNumberString::isTouchToneNumber() const {

    return false;
}

bool QPhoneNumberString::isUnknownNumberType() const {
    if ( isSipNumber() ) return false;
    if ( isTouchToneNumber() ) return false;
    return true;
}
