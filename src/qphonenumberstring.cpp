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
    QRegExp regexp("\\@");
    return this->contains(regexp);
}
bool QPhoneNumberString::isTouchToneNumber() const {

    return !isSipNumber();
}

bool QPhoneNumberString::isUnknownNumberType() const {
    if ( isSipNumber() ) return false;
    if ( isTouchToneNumber() ) return false;
    return true;
}


QString QPhoneNumberString::shortFBnumber() const {
    QString shortNumber;
    bool diff = false;
    if ( ! this->isTouchToneNumber() ) return *this;
//    kDebug() << "Vergleiche : Country\t" << countryCode()
//             << " "  << m_LocalCountryCode;
    if ( countryCode() == m_LocalCountryCode ) {
        // kDebug() << "CountryCode stimmt überein, muß nicht hinzugefügt werden";
    } else {
        // kDebug() << "CountryCode stimmt nicht überein";
        // kDebug() << "   Alles muß zur Nummer hinzugefügt werden";
        diff = true;
        shortNumber += countryCode();
    }
    if ( diff ) {
        shortNumber += areaCode();
    } else {
        if ( areaCode() == m_LocalAreaCode ) {
            // kDebug() << "AreaCode stimmt überein, muß nicht hinzugefügt werden";
        } else {
            // kDebug() << "AreaCode stimmt nicht überein";
            // kDebug() << "   Rest muß zur Nummer hinzugefügt werden";
            diff = true;
            shortNumber += "0" + areaCode();
        }
    }
    if ( diff ) {
        shortNumber +=number();
    } else {
        if ( number() == m_LocalNumber ) {
            // kDebug() << "Number stimmt überein, muß nicht hinzugefügt werden";
        } else {
            // kDebug() << "Number stimmt nicht überein";
            // kDebug() << "   Rest muß zur Nummer hinzugefügt werden";
            diff = true;
            shortNumber += number();
        }
    }
    shortNumber += phoneExtensionNumber();

    //    } else {
//        kDebug() << "AreaCode\t" << areaCode() << " "  << m_LocalAreaCode;
//    }
//    if ( (!diff) && ) {
//        shortNumber += areaCode();
//        diff = true;
//    } else {
//        kDebug() << "Number\t" << number() << " "  << m_LocalNumber;
//    }
//    if ( (!diff) && ( number() != m_LocalNumber )) {
//        shortNumber += number();
//    }
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
    if ( ! isTouchToneNumber() ) return ;
    if ( type & NetXML ) {
        if ( ! m_StaticInitialized ) staticInitialize();
        QString remains = *this;
        // Format Number for recognition
        remains.replace(QRegExp("\\s"),"");
        remains.replace(QRegExp("\\(0\\)"),"");
        remains.replace(QRegExp("-"),"");
        remains.replace(QRegExp("^\\+"),"00");
        remains.replace(QRegExp("\\D"),"");

        kDebug() << "TestRemain " << remains;
        QDomElement rootE = m_RootElementPhoneNetXML;

        // nur so dass, die Nummer mit einem Plus beginnt
        QString prefix = rootE.attribute("prefix");
        if ( remains.startsWith( prefix ) ) {
//            kDebug() << "Ausgabe: " << prefix;
            remains.remove(0,prefix.length());
        } else {
            if ( remains.startsWith("00") )
                remains.remove(0,QString("00").length());
            else return;
        }
        bool done =  false;
        m_CountryCode = recognizeNumber(rootE, remains);
        if ( m_CountryCode.isNull() ) {
            m_Number = remains;
            done = true;
        }
        // kDebug() << "Ausgabe: " << m_CountryCode;
        m_AreaCode = recognizeNumber(rootE, remains);
        if ( (! done) && m_CountryCode.isNull() ) {
            m_Number = remains;
            done = true;
        }
        // kDebug() << "Ausgabe: " << m_AreaCode;
        m_Number = recognizeNumber(rootE, remains);
        // kDebug() << "Ausgabe: " << m_Numberqqq;
        m_PhoneExtensionNumber = recognizeNumber(rootE, remains);
        // kDebug() << "Ausgabe: " << m_PhoneExtensionNumber;
        if (m_PhoneExtensionNumber.isNull()) m_PhoneExtensionNumber = remains;
    }
    m_Recognized = true;
}


QString QPhoneNumberString::recognizeNumber(QDomElement & rootE,
                                            QString & remains)
{
    // kDebug() << " Try to find prefix in: " << remains;
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

QString QPhoneNumberString::prettyPrint(const OutputType type) const {
    QString niceS;
    switch (type) {
        case International:
            niceS += "+";
            niceS += countryCode();
            niceS += " ";
            niceS += areaCode();
            niceS += " ";
            if ( ! number().isNull() ) {
                niceS += number();
                niceS += "-";
            }
            niceS += phoneExtensionNumber();
            break;
        case Short:
            kDebug() << "Not Yet Implemented";
            niceS = "";
            break;
        default:
            kDebug() << "Unknown Type :" << type;
            niceS = "";
            break;

    }
    return niceS;

}

bool QPhoneNumberString::m_StaticInitialized = false;
QDomElement QPhoneNumberString::m_RootElementPhoneNetXML;

bool QPhoneNumberString::staticInitialize(
        const QString localCountryCode,
        const QString localAreaCode,
        const QString localNumber)
{
    m_LocalCountryCode = localCountryCode;
    m_LocalAreaCode = localAreaCode;
    m_LocalNumber = localNumber;
    if ( m_StaticInitialized ) return true;
    kDebug() << "Initialisiere NetzBaum";
    QDomDocument phoneNetDoc;
    QFile file("netnumbers.xml");
    if (!file.open(QIODevice::ReadOnly))
        return false; // @TODO exceptions einbauen
    if (!phoneNetDoc.setContent(&file)) {
        file.close();
        return false; // @TODO exceptions einbauen
    }
    file.close();
    // @TODO … check for the real file
    m_RootElementPhoneNetXML = phoneNetDoc.documentElement();
    m_StaticInitialized = true;
    return true;
}

QString QPhoneNumberString::m_LocalCountryCode;
QString QPhoneNumberString::m_LocalAreaCode;
QString QPhoneNumberString::m_LocalNumber;
