#include <kdebug.h>
#include <QRegExp>

#include "fritzboxphonenumber.h"

FritzBoxPhoneNumber::FritzBoxPhoneNumber(
        const QString & number,
        NumberTypeType type,
        PriorityType prio,
        const QString & quickdial,
        const QString & vanity
):
        m_Type(type),
        m_Priority(prio),
        m_QuickDial(quickdial),
        m_Vanity(vanity),
        m_PhoneNumber(number)
{
    //kDebug() << "A PhoneNumber is created";
}

bool FritzBoxPhoneNumber::isValid() const
{
    // vanity and quickdial contains only of numbers
//    if ( isNumberOrNull( m_Vanity ) ) {
//        kDebug() << " Vanity (" << m_Vanity
//                 << ") Number of " << m_PhoneNumber
//                 <<" is not a Number ";
//        return false;
//    }
//
//    if ( isNumber( m_QuickDial ) ) {
//        kDebug() << " QuickDial Number (" << m_QuickDial
//                 <<") of " << m_PhoneNumber
//                 <<" is not a Number ";
//        return false;
//    }
//
//    // Wenn m_Phonenumber contains "@", quickdial and vanity required
//    if ( m_PhoneNumber.contains("@") ) {
//        kDebug() << "Phonenumber with an @ (" << m_PhoneNumber
//                 << ") must have a quickdial and a vanitynumber";
//        return false;
//    }

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

QDomElement FritzBoxPhoneNumber::generateDomElement( QDomDocument & doc ) const {
    QDomElement foo;
    if ( ! isValid() ) return foo;
    QDomElement numberE = doc.createElement("number");
    numberE.setAttribute("prio", this->m_Priority);
    numberE.setAttribute("type", this->typeString());
    if ( ! m_Vanity.isEmpty() )
        numberE.setAttribute("vanity", m_Vanity);
    if ( ! m_QuickDial.isEmpty() )
        numberE.setAttribute("quickdial", m_QuickDial);
    QDomText numberText = doc.createTextNode( m_PhoneNumber );
    numberE.appendChild( numberText );
    return numberE;
}

QString FritzBoxPhoneNumber::typeString() const {
    if ( m_Type == home ) return QString("home");
    if ( m_Type == work ) return QString("work");
    if ( m_Type == mobile ) return QString("mobile");
    return QString("unknown");
}

void FritzBoxPhoneNumber::print() const {
    kDebug() << "               "
             << m_Type << " "
             << m_Priority << " "
             << m_QuickDial << "\t"
             << m_Vanity << "\t"
             << m_PhoneNumber;

}

bool FritzBoxPhoneNumberList::isValid() const {
    return true;
}


QList<QDomNode>
FritzBoxPhoneNumberList::generateDomElements(
        QDomDocument &doc) const {
    QList<QDomNode> nodeList;
    if ( ! this->isValid() ) return nodeList;
    for ( QList<FritzBoxPhoneNumber>::const_iterator numberI  = begin();
                                                     numberI != end();
                                                     numberI++) {

        QDomElement numberE = numberI->generateDomElement( doc );
        nodeList.append( numberE );

    }

    return nodeList;
}

bool FritzBoxPhoneNumberList::isThereAlreadyANumberOfType(
        const FritzBoxPhoneNumber::NumberTypeType type) const {

    bool recurrence = false;
    for (const_iterator numberI = begin(); numberI != end(); numberI++) {
        if (numberI->type() == type) recurrence = true;
    }
    return recurrence;
}

void FritzBoxPhoneNumberList::print() const {
    for (const_iterator numberI = begin(); numberI != end(); numberI++) {
        numberI->print();
    }

}
