#include "qtphonenumberstring.h"


QtPhoneNumberString::QtPhoneNumberString():
        QString()
{
}

QtPhoneNumberString::QtPhoneNumberString(const QString & str):
    QString(str)
{}


bool QtPhoneNumberString::isCallableByFB7270() const {
    QRegExp regexp("^\\d+$");
    return regexp.exactMatch( *this );
}

bool QtPhoneNumberString::isSipNumber() const {

    return false;
}
bool QtPhoneNumberString::isTouchToneNumber() const {

    return false;
}

bool QtPhoneNumberString::isUnknownNumberType() const {
    if ( isSipNumber() ) return false;
    if ( isTouchToneNumber() ) return false;
    return true;
}
