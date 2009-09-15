#ifndef FRITZBOXPHONENUMBER_H
#define FRITZBOXPHONENUMBER_H

#include <QList>
#include <QString>

/** @class FritzBoxPhoneNumber
        This class describes one PhoneNumber of the FritzBoxPhonebook
        is everything in the XML-Element <number/>
*/
class FritzBoxPhoneNumber {
    public:
        bool valid() const;
        // Wenn m_Phonenumber contains "@", quickdial and vanity required
    private:
        bool isNumber(QString string) const;
        bool isNumberOrNull(QString string) const;

        /** Attribute type */
        enum {home=0, work=1, mobile=2} m_Type;
        /** Attribute prio */
        enum {mainCallingNumber = 0, nonMainCallingNumber = 1} m_Priority;
        /** Attribute quickdial */
        QString m_QuickDial;
        /** Attribute vanity */
        QString m_Vanity;

        QString m_PhoneNumber;
};

/** @class FritzBoxPhoneNumberList
        This class describes a List of FritzBoxPhoneNumbers of
        a FritzBoxPhoneBookContact is everything in the XML-Element <telefony/>
*/
class FritzBoxPhoneNumberList {
    public:
        bool valid() const;
        //listsize max 3
        // one of home work and mobile
        // only one number can have the "prio"rity Flag
        // all Numbers are valid
    private:
        QList<FritzBoxPhoneNumber> m_NumberList;
};


#endif // FRITZBOXPHONENUMBER_H
