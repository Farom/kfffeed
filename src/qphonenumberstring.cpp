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


QString QPhoneNumberString::shortFBnumber(QString countryC,
                                          QString areaC,
                                          QString num) const
{
    QString shortNumber;
//    if ( ! this->isTouchToneNumber() ) return this;
//    if ( countryCode() != countryC ) shortNumber += countryCode();
//    if ( areaCode() != areaC ) shortNumber += areaCode();
//    if ( ! this->startsWith(num) ) shortNumber += number();
//    shortNumber += phoneExtensionNumber();
    return shortNumber;
}

QString QPhoneNumberString::countryCode() const {
    return m_CountryCode;
}

QString QPhoneNumberString::areaCode() const {// Ortsnetzkennzahl
    return m_AreaCode;
}

QString QPhoneNumberString::number() const { // Durchwahlrufnummer
    return m_Number;
}
QString QPhoneNumberString::phoneExtensionNumber() const { // Nebenstellennummer
    return m_PhoneExtensionNumber;
}

void QPhoneNumberString::recognizeNumber(const RecognizeType type) {
    if ( type & NetXML ) {
        QDomDocument phoneNetDoc;
        QFile file("netnumbers.xml");
        if (!file.open(QIODevice::ReadOnly))
            return; // @TODO exceptions einbauen
        if (!phoneNetDoc.setContent(&file)) {
            file.close();
            return; // @TODO exceptions einbauen
        }
        file.close();
        QDomElement phoneNetRootE = phoneNetDoc.documentElement();
        QString remains = *this;
        QDomElement rootE = phoneNetRootE;

        // nur so dass, die Nummer mit einem Plus beginnt
        QString prefix = rootE.attribute("prefix");
        if ( ! remains.startsWith( prefix ) )
            return;
        kDebug() << "Ausgabe: " << prefix;
        remains.remove(prefix);

        m_CountryCode = recognizeNumber(rootE, remains);
        kDebug() << "Ausgabe: " << m_CountryCode;
        m_AreaCode = recognizeNumber(rootE, remains);
        kDebug() << "Ausgabe: " << m_AreaCode;
        m_Number = recognizeNumber(rootE, remains);
        kDebug() << "Ausgabe: " << m_AreaCode;
        m_PhoneExtensionNumber = recognizeNumber(rootE, remains);
        kDebug() << "Ausgabe: " << m_PhoneExtensionNumber;
        if (m_PhoneExtensionNumber.isNull()) m_PhoneExtensionNumber = remains;
    }
    m_Recognized = true;
}


QString QPhoneNumberString::recognizeNumber(QDomElement & rootE,
                                            QString & remains)
{
    kDebug() << " Try to find prefix in: " << remains;
    QDomElement element = rootE.firstChildElement();
    QString foundPrefix;
    QString prefix;
    int elementCount = 0;
    while ( !element.isNull() ) {
        prefix = element.attribute("prefix");
        //kDebug() << elementCount << "Untersuchter Prefix: " << prefix;
        if ( remains.startsWith(prefix) ) {
            rootE = element;
            remains.remove(0,prefix.length());
            return prefix;
        }
        element = element.nextSiblingElement();
        elementCount++;
    }
    return QString();
}

QString QPhoneNumberString::prettyPrint() {
    QString niceS;
    niceS += "+";
    niceS += countryCode();
    niceS += " ";
    niceS += areaCode();
    niceS += " ";
    niceS += number();
    niceS += "-";
    niceS += phoneExtensionNumber();
    return niceS;
}
