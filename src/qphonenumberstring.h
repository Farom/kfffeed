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

    QPhoneNumberString();
    QPhoneNumberString(const QString & str);

    bool isUnknownNumberType() const;
    bool isCallableByFB7270() const;
    bool isSipNumber() const;
    bool isTouchToneNumber() const;

    // If you live in an city with AreaCode 543 you will not have to dial this
    // with a short number in the FritzBox you see much faster, where you dial
    QString shortFBnumber(QString countryCode = "", // the number where you live in
                   QString AreaCode = "",  // Number of the city you live in
                   QString number = "") const; // the number you do not have to dial;


    QString countryCode() const; // Ländercode
    QString areaCode() const; // Ortsnetzkennzahl
    QString number() const; // Durchwahlrufnummer
    QString phoneExtensionNumber() const; // Nebenstellennummer

    void recognizeNumber(const RecognizeType type = NetXML);
    QString prettyPrint();
private:
    bool    m_Recognized;
    QString m_CountryCode;
    QString m_AreaCode;
    QString m_Number;
    QString m_PhoneExtensionNumber;

    QString recognizeNumber(QDomElement & rootE, QString & remains);
};

#endif // QTPHONENUMBERSTRING_H
