#include <kdebug.h>
#include <QRegExp>

#include "fritzboxphonenumber.h"


bool FritzBoxPhoneNumber::isValid() const
{
    // vanity and quickdial contains only of numbers
    if ( isNumberOrNull( m_Vanity ) ) {
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

bool FritzBoxPhoneNumber::isNumber(const QString string) const
{
    const static QRegExp rx("^\\+?[\\d\\s]+$");
    return rx.exactMatch(string);
}

bool FritzBoxPhoneNumber::isNumberOrNull(const QString string) const
{
    const static QRegExp rx("^\\d*$");
    return string.contains(rx);
}

QDomElement FritzBoxPhoneNumber::generateDomElement(QDomDocument & doc) const {
    QDomElement foo;
    if ( ! isValid() ) return foo;
    QDomElement numberE = doc.createElement("number");
    numberE.setAttribute("prio", this->m_Priority);
    numberE.setAttribute("type", this->m_Type);
    if ( ! m_Vanity.isEmpty() )
        numberE.setAttribute("vanity", m_Vanity);
    if ( ! m_QuickDial.isEmpty() )
        numberE.setAttribute("quickdial", m_QuickDial);
    QDomText numberText = doc.createTextNode( m_PhoneNumber );
    numberE.appendChild( numberText );
    return numberE;
}

bool FritzBoxPhoneNumberList::isValid() const {
    return true;
}
