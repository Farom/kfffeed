#include "fritzboxphonenumber.h"

bool FritzBoxPhoneNumber::valid() const
{
    // vanity and quickdial contains only of numbers
    if ( isNumberOr( m_Vanity ) ) {
        kDebug() << " Vanity (" << m_Vanity
                 << ") Number of " << m_PhoneNumber
                 <<" is not a Number ";
        return false;
    }

    if ( isNumber( m_QuickDial ) ) {
        kDebug() << " QuickDial Number (" << m_QuickDial
                 <<") of " << m_PhoneNumber
                 <<" is not a Number ";
        return false;
    }

    // Wenn m_Phonenumber contains "@", quickdial and vanity required
    if ( m_PhoneNumber.contains("@") ) {
        kDebug() << "Phonenumber with an @ (" << m_PhoneNumber
                 << ") must have a quickdial and a vanitynumber";
        return false;
    }

    return true;
}

bool FritzBoxPhoneNumber::isNumber(QString string)
{
    const static QRegExp rx("^\\d+$");
    return string.contains(rx);
}

bool FritzBoxPhoneNumber::isNumberOrNull(QString string)
{
    const static QRegExp rx("^\\d*$");
    return string.contains(rx);
}
