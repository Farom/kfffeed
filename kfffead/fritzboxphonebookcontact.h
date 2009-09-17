#ifndef FRITZBOXPHONEBOOKCONTACT_H
#define FRITZBOXPHONEBOOKCONTACT_H

#include <QtCore>
#include <QtXml>

#include "fritzboxphonenumber.h"

/** @class FritzBoxPhoneBookContact
        This Class describes everything needed for the XML Group <contact)
  */
class FritzBoxPhoneBookContact {
public:
    enum ImportanceType {
        UNIMPORTANT=0,
        IMPORTANT=1
    };

    FritzBoxPhoneBookContact(
            const QString & personName = QString(),
            const ImportanceType & importance = UNIMPORTANT);

    QString person() const { return m_Person; }
    void setPerson(const QString name) { m_Person = name; }

    bool isValid() const;
    QDomElement generateDomElement( QDomDocument & doc) const;
    /// brief Method adds a PhonenumberObjekt to the contactlist
    /** return  gives, whether it was possible to add the number */
    bool addNumber(const FritzBoxPhoneNumber & number );
    void print() const;
    bool hasNumbers() const { return m_FonNumberList.size(); }

private:
    /** XML-Element <category/> */
    ImportanceType m_Category;
    /** m_Person is the String that appears in your Phonebook on the DectPhone
        XML-Element <person/>
    */
    QString m_Person;

    /** XML-Element <telephony/> */
    FritzBoxPhoneNumberList m_FonNumberList;
};

/** @class FritzBoxPhoneBookContactList
    This class */
class FritzBoxPhoneBookContactList : public QList<FritzBoxPhoneBookContact> {
public:
    bool isValid() const;
    void addContact(const FritzBoxPhoneBookContact & contact) {
        //kDebug() << "Contact will be added";
        append(contact);
        //kDebug() << "Count of Contacts: " << size();
    }
    void deleteContactsWithoutNumbers();
    void print() const;
private:

};

#endif // FRITZBOXPHONEBOOKCONTACT_H