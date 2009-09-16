#ifndef FRITZBOXPHONENUMBER_H
#define FRITZBOXPHONENUMBER_H

#include <QtCore>
#include <QtXml>
#include <kdebug.h>

/** @class FritzBoxPhoneNumber
        This class describes one PhoneNumber of the FritzBoxPhonebook
        is everything in the XML-Element <number/>
*/
class FritzBoxPhoneNumber {
    public:
        bool isValid() const;
        // Wenn m_Phonenumber contains "@", quickdial and vanity required
        QDomElement generateDomElement(QDomDocument & doc) const;

    private:
        bool isNumber(const QString string) const;

        bool isNumberOrNull(const QString string) const;

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
        bool isValid() const;
        int size() const { return m_NumberList.size(); }
        bool isEmpty() const { return m_NumberList.isEmpty(); }
        const FritzBoxPhoneNumber & operator[](const int i) const {
            if ( (i >= 0) && (i < size()) ) return m_NumberList.at(i);
            kDebug() << "Out of index";
        }
        //listsize max 3
        // one of home work and mobile
        // only one number can have the "prio"rity Flag
        // all Numbers are valid
    private:
        QList<FritzBoxPhoneNumber> m_NumberList;
};


#endif // FRITZBOXPHONENUMBER_H
