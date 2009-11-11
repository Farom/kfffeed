#ifndef QTPHONENUMBERSTRING_H
#define QTPHONENUMBERSTRING_H

#include <QtCore>
#include <QtXml>
#include <kdebug.h>

class QPhoneNumberString : public  QString {
public:
    enum PhoneNumberType {
        SipNumber = 0,
        TouchToneNumber = 1
    };
    enum RecognizeType {
        RegExp = 1,
        NetXML = 2
    };
    enum OutputType {
        International = 1,
        Short = 2
    };

    QPhoneNumberString();
    QPhoneNumberString(const QString & str);

    bool operator==(const QPhoneNumberString & that) const;
    bool operator!=(const QPhoneNumberString & that) const;

    bool isUnknownNumberType() const;
    bool isCallableByFB7270() const;
    bool isSipNumber() const;
    bool isTouchToneNumber() const;

    // If you live in an city with AreaCode 543 you will not have to dial this
    // with a short number in the FritzBox you see much faster, where you dial
    // The number is not recognized if you live in 0543, someone calls you with
    // 123456 and 0543 123456 is saved in you FB-PhoneBook
    QString shortFBnumber() const; // the number you do not have to dial;


    QString countryCode() const; // Ländercode
    QString areaCode() const; // Ortsnetzkennzahl
    QString number() const; // Durchwahlrufnummer
    QString phoneExtensionNumber() const; // Nebenstellennummer

    static void setNetNumbersFileName(const QString & filename) {
        m_NetNumbersFileName = filename;}

    void recognizeNumber(const RecognizeType type = NetXML);
    QString prettyPrint(const OutputType type = International) const;
    QString strippedSipNumber() const;

    static bool staticInitialize(const QString localCountryCode = "",
                                 const QString localAreaCode = "",
                                 const QString localNumber = "");
    static QDomElement  m_RootElementPhoneNetXML;
    static QChar translatedToVanity(const QChar c);
    static QString translatedToVanity(const QString str);
private:
    bool    m_Recognized;
    QString m_CountryCode;
    QString m_AreaCode;
    QString m_Number;
    QString m_PhoneExtensionNumber;

    QString recognizeNumber(QDomElement & rootE, QString & remains);

    static bool         m_StaticInitialized;
    static QString      m_LocalCountryCode;
    static QString      m_LocalAreaCode;
    static QString      m_LocalNumber;
    static QString      m_NetNumbersFileName;
};

#endif // QTPHONENUMBERSTRING_H
