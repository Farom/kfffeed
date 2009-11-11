#ifndef FRITZBOXPHONENUMBER_H
#define FRITZBOXPHONENUMBER_H

#include <QtCore>
#include <QtXml>
#include <kdebug.h>
#include "qphonenumberstring.h"

/** This enum has to used outside the class */
namespace FritzBoxNumber {
    enum ErrorValidType {
        NoError = 0,
        VanityNumberIsMissing = 1,
        VanityNumberIsWrong = 2,
        QuickDialNumberIsMissing = 4,
        QuickDialNumberIsWrong = 8
    };
}


/** @class FritzBoxPhoneNumber
        This class describes one PhoneNumber of the FritzBoxPhonebook
        is everything in the XML-Element <number/>
*/
class FritzBoxPhoneNumber {
public:
    enum PriorityType {
        mainCallingNumber = 0,
        nonMainCallingNumber = 1 // this is the one which will be dialed
                                 // if clicked on the name on the phone
    };
    enum NumberTypeType {
        Home=0,
        Work=1,
        Mobile=2
    };

    FritzBoxPhoneNumber(
        const QString & number = QString(),
        NumberTypeType type = FritzBoxPhoneNumber::Home,
        PriorityType prio = FritzBoxPhoneNumber::nonMainCallingNumber,
        const QString & quickdial = QString(),
        const QString & vanity = QString());

    QString number() const { return m_PhoneNumber; }
    void setNumber(const QString & number) { m_PhoneNumber = number; }
    QString vanity() const { return m_Vanity; }
    void setVanity(const QString & vanity) { m_Vanity = vanity; }
    QString quickDial() const { return m_QuickDial; }
    void setQickDial(const QString & qickDial) { m_QuickDial= qickDial; }
    PriorityType priority() const { return m_Priority; }
    void setPriority( const PriorityType & prio ) { m_Priority = prio; }
    NumberTypeType type() const { return m_Type; }
    void setType(const NumberTypeType & type) { m_Type = type; }

    int getErrorStatus() const;
    bool isValid() const;
    // Wenn m_Phonenumber contains "@", quickdial and vanity required
    QDomElement generateDomElement(QDomDocument & doc) const;

    QString typeString() const;
    void print() const;

    // returns whether a VanityNumber could be found
    bool generateUniqueVanityNumber( QString fromString,
                               const QStringList blackList );
    bool generateUniqueQuickDialNumber( const QStringList blackList );


private:
   bool isNumber(const QString string) const;

   bool isNumberOrNull(const QString string) const;

   /** Attribute type */
   NumberTypeType m_Type;
   /** Attribute prio */
   PriorityType m_Priority;
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
class FritzBoxPhoneNumberList : public QList<FritzBoxPhoneNumber>{
    public:
        bool isValid() const;
        //listsize max 3
        // one of home work and mobile
        // only one number can have the "prio"rity Flag
        // all Numbers are valid
        int size() const { return this->length(); }
        bool isEmpty() const { return size()==0; }
        const FritzBoxPhoneNumber & operator[](const int i) const {
            return this->operator[](i);
        }
        //QDomElement generateDomElements(QDomDocument & doc) const;
        QList<QDomNode> generateDomElements(QDomDocument &doc) const;

        bool isThereAlreadyANumberOfType(
            const FritzBoxPhoneNumber::NumberTypeType type) const;

        void addNumber(const FritzBoxPhoneNumber & number) {
            append(number);
        }

        void print() const;

    private:
};


#endif // FRITZBOXPHONENUMBER_H
