#include "qphonenumberstring.h"


QPhoneNumberString::QPhoneNumberString():
        QString()
{
}

QPhoneNumberString::QPhoneNumberString(const QString & str):
    QString(str)
{}

bool QPhoneNumberString::operator==(const QPhoneNumberString & other) const {
    if ( other == *this ) return true;
    if ( this->countryCode() != other.countryCode() ) return false;
    if ( this->areaCode() != other.areaCode() ) return false;
    if ( this->number() != other.number() ) return false;
    if ( this->phoneExtensionNumber() != other.phoneExtensionNumber() ) return false;
    return true;
}

bool QPhoneNumberString::operator!=(const QPhoneNumberString & other) const {
    return ! ( *this == other );
}


bool QPhoneNumberString::isCallableByFB7270() const {
    QRegExp regexp("^\\d+$");
    return regexp.exactMatch( *this );
}

bool QPhoneNumberString::isSipNumber() const {
    QRegExp regexp("sip:.*@((\\w([\\w-]*\\w)?\\.)*)+",Qt::CaseInsensitive);
    //QRegExp regexp2("sip:.*@(([a-z0-9]([a-z0-9-]*[a-z0-9])?\\.)+[a-z0-9]([a-z0-9-]*[a-z0-9])?|\\[((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?|[a-z0-9-]*[a-z0-9]:([\\x01-\\x08\\x0b\\x0c\\x0e-\\x1f\\x21-\\x5a\\x53-\\x7f]|\\[\\x01-\\x09\\x0b\\x0c\\x0e-\\x7f])+)\\])",
    //                 Qt::CaseInsensitive);
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
    // @TODO partition to more than one function :-)
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

        //kDebug() << "TestRemain " << remains;
        QDomElement rootE = m_RootElementPhoneNetXML;

        // nur so dass, die Nummer mit einem Plus beginnt
        QString prefix = rootE.attribute("prefix");
        bool foundPrefixInt = false;
        if ( remains.startsWith( prefix ) ) {
//            kDebug() << "Ausgabe: " << prefix;
            remains.remove(0,prefix.length());
            foundPrefixInt = true;
        } else {
            if ( remains.startsWith("00") ) {
                remains.remove(0,QString("00").length());
                foundPrefixInt = true;
            }
        }
        bool foundPrefixCountryCode = false;
        if (foundPrefixInt) {
            m_CountryCode = recognizeNumber(rootE, remains);
            if ( m_CountryCode.isNull() ) {
                m_Number = remains;
                kDebug() << "This countryCode was not found in XML-Database :"
                         << *this;
                m_AreaCode = "";
                m_CountryCode = "";
                m_Number = *this;
                m_PhoneExtensionNumber = "";
                m_Recognized = true;
                return;
            } else {
                foundPrefixCountryCode = true;
            }
        } else {
            m_CountryCode = m_LocalCountryCode;
            QString fakeRemains = m_CountryCode;
            QString check = recognizeNumber(rootE,fakeRemains);
            //kDebug() << "Next Element :" << rootE.attribute("prefix");
            if ( check != m_CountryCode ) {
                kDebug() << "CountryCode not in XML-Database : "
                         << m_LocalCountryCode;
                m_AreaCode = "";
                m_CountryCode = "";
                m_Number = *this;
                m_PhoneExtensionNumber = "";
                m_Recognized = true;
                return;
            }
        }
//        kDebug() << "Found CountryCode: " << m_CountryCode
//                 << "    remains   " << remains;
        bool thereHasToBeAAreaCode = false;
        if ( remains.startsWith("0") && (! foundPrefixCountryCode) ) {
            remains.remove(0,QString("0").length());
            thereHasToBeAAreaCode = true;
        }
        if ( thereHasToBeAAreaCode || foundPrefixCountryCode )
            m_AreaCode = recognizeNumber(rootE, remains);
        //else kDebug() << "Strange m_AreaCode ?: " << m_AreaCode;
        // kDebug() << "Ausgabe: " << m_CountryCode;
        if ( m_AreaCode.isNull() ) {
            if ( thereHasToBeAAreaCode ) {
                kDebug() << "The AreaCode for this number was not found"
                         << remains << *this;
                m_AreaCode = "";
                m_CountryCode = "";
                m_Number = *this;
                m_PhoneExtensionNumber = "";
                m_Recognized = true;
                return;
            } else {
                m_AreaCode = m_LocalAreaCode;
                QString fakeRemains = m_LocalAreaCode;
                QString check = recognizeNumber(rootE,fakeRemains);
                //kDebug() << "Next Element :" << rootE.attribute("prefix");
                if (check != m_LocalAreaCode) {
                    kDebug() << "AreaCode not found in XML-Database : "
                             << m_LocalAreaCode;
                    m_AreaCode = "";
                    m_CountryCode = "";
                    m_Number = *this;
                    m_PhoneExtensionNumber = "";
                    m_Recognized = true;
                    return;
                }
            }
        }
//        kDebug() << "Found AreaCode: " << m_AreaCode
//                 << "    remains   " << remains;
        // kDebug() << "Ausgabe: " << m_AreaCode;
        m_Number = recognizeNumber(rootE, remains);
        if ( m_Number.isNull() ) {
            m_Number = remains;
            m_Recognized = true;
            return;
        }
        // kDebug() << "Ausgabe: " << m_Numberqqq;
        m_PhoneExtensionNumber = recognizeNumber(rootE, remains);
        // kDebug() << "Next Element :" << rootE.attribute("prefix");
        // kDebug() << "Ausgabe: " << m_PhoneExtensionNumber;
        if (m_PhoneExtensionNumber.isNull()) m_PhoneExtensionNumber = remains;
    }
    m_Recognized = true;
/**broken:
    kfffeed(27049) main: "1234"     "+49 123 7890"       "7890"
    you can not know, if 4889 is meant +49 123 456-7890 or +49 123 7890
    idea … i think there are instructions about the whole length of
    phonenumbers: If they are too short, it is a phoneExtensionNumber
*/
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
    if (isTouchToneNumber()) {
        switch (type) {
        case International:
            if (! m_CountryCode.isNull()) {
                niceS += "+";
                niceS += countryCode();
                niceS += " ";
            }
            if (! m_AreaCode.isNull()) {
                niceS += areaCode();
                niceS += " ";
            }
            if ( ! number().isNull() ) {
                niceS += number();
            }
            if ( ! m_PhoneExtensionNumber.isNull() ) {
                niceS += "-";
                niceS += phoneExtensionNumber();
            }
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
    } else {
        if (isSipNumber()) niceS = *this;
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
    kDebug() << "Initialize PhoneNetTree";
    QDomDocument phoneNetDoc;
    QFile file(m_NetNumbersFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        kDebug() << "Could not open: " << m_NetNumbersFileName;
        exit(1);
        return false; // @TODO exceptions einbauen
    }
    if (!phoneNetDoc.setContent(&file)) {
        file.close();
        kDebug() << "Could not get Content of xml-file :"
                  << m_NetNumbersFileName;
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

QString QPhoneNumberString::m_NetNumbersFileName;

QChar QPhoneNumberString::translatedToVanity(QChar c) {
    Q_ASSERT( ! c.isNull());
    if ( c.isNumber() ) return c;
    if ( c.isSpace() ) return QChar('1');
    if ( c.toLower() == 'a' ) return QChar('2');
    if ( c.toLower() == 0xE4 ) return QChar('2'); // ä
    if ( c.toLower() == 'b' ) return QChar('2');
    if ( c.toLower() == 'c' ) return QChar('2');
    if ( c.toLower() == 'd' ) return QChar('3');
    if ( c.toLower() == 'e' ) return QChar('3');
    if ( c.toLower() == 'f' ) return QChar('3');
    if ( c.toLower() == 'g' ) return QChar('4');
    if ( c.toLower() == 'h' ) return QChar('4');
    if ( c.toLower() == 'i' ) return QChar('4');
    if ( c.toLower() == 'j' ) return QChar('5');
    if ( c.toLower() == 'k' ) return QChar('5');
    if ( c.toLower() == 'l' ) return QChar('5');
    if ( c.toLower() == 'm' ) return QChar('6');
    if ( c.toLower() == 'n' ) return QChar('6');
    if ( c.toLower() == 'o' ) return QChar('6');
    if ( c.toLower() == 0xF6 ) return QChar('6'); // ö
    if ( c.toLower() == 0xF8 ) return QChar('6'); // ø
    if ( c.toLower() == 'p' ) return QChar('7');
    if ( c.toLower() == 'q' ) return QChar('7');
    if ( c.toLower() == 'r' ) return QChar('7');
    if ( c.toLower() == 's' ) return QChar('7');
    if ( c.toLower() == 0xDF ) return QChar('7'); // ß
    if ( c.toLower() == 0x1E9E ) return QChar('7'); // ẞ
    if ( c.toLower() == 0x17F ) return QChar('7'); // ſ
    if ( c.toLower() == 't' ) return QChar('8');
    if ( c.toLower() == 'u' ) return QChar('8');
    if ( c.toLower() == 0xFC ) return QChar('8'); // ü
    if ( c.toLower() == 'v' ) return QChar('8');
    if ( c.toLower() == 'w' ) return QChar('9');
    if ( c.toLower() == 'x' ) return QChar('9');
    if ( c.toLower() == 'y' ) return QChar('9');
    if ( c.toLower() == 'z' ) return QChar('9');
    kDebug() << " did not found char " << c << hex << c.unicode() << "\" to translate.";
    return( QChar() );
}

QString QPhoneNumberString::translatedToVanity(QString str) {
    QString vanityString;
    for  (QString::const_iterator i = str.begin(); i != str.end(); i++) {
        vanityString.append( translatedToVanity(*i) );
    }
    return vanityString;
}

QString QPhoneNumberString::strippedSipNumber() const {
    QRegExp regexp("sip:(.*@((\\w([\\w-]*\\w)?\\.)*)+\\w+)",Qt::CaseInsensitive);
    regexp.indexIn(*this);
    QStringList list = regexp.capturedTexts();
    // kDebug() << "Need : " << list.at(1);
    return list.at(1);
}
