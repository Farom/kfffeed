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
    return getErrorStatus() == FritzBoxNumber::NoError;
}

int FritzBoxPhoneNumber::getErrorStatus() const
{
    using namespace FritzBoxNumber;
    int errorCode = NoError;
    QPhoneNumberString number = m_PhoneNumber;
    if ( number.isSipNumber() ) {
        if ( m_Vanity.isEmpty() || m_QuickDial.isEmpty() ) {
            // kDebug() << "Sipnumber does not have a quickdial or vanity number";
        }
        if ( m_Vanity.isEmpty() )
            errorCode = VanityNumberIsMissing | errorCode;
        if ( m_QuickDial.isEmpty() ) errorCode |= QuickDialNumberIsMissing;
    }
    // vanity and quickdial contains only of numbers
    if ( ! isNumberOrNull( m_Vanity ) ) {
        errorCode |= VanityNumberIsWrong;
        kDebug() << " Vanity (" << m_Vanity
                 << ") Number of " << m_PhoneNumber
                 <<" is not a Number ";
    }

    if ( ! isNumberOrNull( m_QuickDial ) ) {
        errorCode |= QuickDialNumberIsWrong;
        kDebug() << " QuickDial Number (" << m_QuickDial
                 <<") of " << m_PhoneNumber
                 <<" is not a Number ";
    }
    //if (errorCode) kDebug() << "Errorcode : " << errorCode;
    return errorCode;
}

bool FritzBoxPhoneNumber::isNumber(const QString string) const
{
    const static QRegExp rx("^\\d+$");
    return rx.exactMatch(string);
}

bool FritzBoxPhoneNumber::isNumberOrNull(const QString string) const
{
    const static QRegExp rx("^\\d*$");
    return string.contains(rx);
}

QDomElement FritzBoxPhoneNumber::generateDomElement( QDomDocument & doc ) const {
    if ( ! isValid() ) {
        kDebug() << "Do not generate DomXMLElement cause "
                 << this->m_PhoneNumber << "is not valid : "
                 << getErrorStatus();
        return QDomElement();
    }
    QDomElement numberE = doc.createElement("number");
    numberE.setAttribute("prio", this->m_Priority);
    numberE.setAttribute("type", this->typeString());
    if ( ! m_Vanity.isEmpty() )
        numberE.setAttribute("vanity", m_Vanity);
    if ( ! m_QuickDial.isEmpty() )
        numberE.setAttribute("quickdial", m_QuickDial);

    QPhoneNumberString number = m_PhoneNumber;
    number.recognizeNumber();
    QString numberStr;
    if ( number.isSipNumber() )
        numberStr = number.strippedSipNumber();
    else
        numberStr = number.shortFBnumber();
    QDomText numberText = doc.createTextNode( numberStr );
    numberE.appendChild( numberText );
    return numberE;
}

QString FritzBoxPhoneNumber::typeString() const {
    if ( m_Type == Home ) return QString("home");
    if ( m_Type == Work ) return QString("work");
    if ( m_Type == Mobile ) return QString("mobile");
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

bool FritzBoxPhoneNumber::generateUniqueVanityNumber(
        QString fromString, // number that should get a vanityNumber
        const QStringList blackList) // list of already used Numbers
{
    QString candidate;
    QString translatedStr;
    translatedStr = QPhoneNumberString::translatedToVanity( fromString );
    // my default vanitynumber should contain 5 digits
    // Fritz Box allows 8 Digits
    candidate = translatedStr.left(5);
    bool vanityAdded = false;
    int retry = 1;
    while ( ! vanityAdded ) {
        if ( blackList.contains( candidate ) ) {
            candidate = translatedStr.left(5) + retry;
            retry++;
            if ( retry > 9 ) break;
        }
        else {
            m_Vanity = candidate;
            vanityAdded = true;
            kDebug() << candidate << " generated to " << m_PhoneNumber;
        }
    }
    return vanityAdded;
}

bool FritzBoxPhoneNumber::generateUniqueQuickDialNumber( const QStringList blackList ) {
    bool quickDialAdded = false;
    // we try Qickdials with 3 Numbers. We begin with 10
    int candidate = 10;
    while ( ! quickDialAdded ) {
        QString candidateStr = QString("%1").arg(candidate);
        if ( blackList.contains( candidateStr ) ) {
            candidate++;
        }
        else {
            m_QuickDial = candidateStr;
            quickDialAdded = true;
        }
    }
    return quickDialAdded;
}


bool FritzBoxPhoneNumberList::isValid() const {
    return true;
}

QList<QDomNode>
FritzBoxPhoneNumberList::generateDomElements(
        QDomDocument &doc) const
{
    QList<QDomNode> nodeList;
    if ( ! this->isValid() ) return nodeList;
    for ( QList<FritzBoxPhoneNumber>::const_iterator numberI  = begin();
                                                     numberI != end();
                                                     numberI++) {

        QDomElement numberE = numberI->generateDomElement( doc );
        if ( ! numberE.isNull() ) nodeList.append( numberE );

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
