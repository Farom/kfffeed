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
class FritzBoxPhoneNumberList : private QList<FritzBoxPhoneNumber>{
    public:
        bool isValid() const;
        //listsize max 3
        // one of home work and mobile
        // only one number can have the "prio"rity Flag
        // all Numbers are valid
        int size() const { return this->size(); }
        bool isEmpty() const { return this->isEmpty(); }
        const FritzBoxPhoneNumber & operator[](const int i) const {
            return this->operator[](i);
        }
        //QDomElement generateDomElements(QDomDocument & doc) const;
        QList<QDomNode> generateDomElements(QDomDocument &doc) const;

    private:
};


#endif // FRITZBOXPHONENUMBER_H
