#ifndef FRITZBOXPHONEBOOK_H
#define FRITZBOXPHONEBOOK_H

#include <QtCore>
#include <QtXml>

#include <kabc/stdaddressbook.h>
#include "fritzboxphonebookcontact.h"

/** @class FritzBoxPhonebook
    XML-Element <phonebook/>
*/
class FritzBoxPhoneBook {
//friend KDebug & operator<< (KDebug &, const FritzBoxPhoneBook &);
public:
    FritzBoxPhoneBook(const QString & name = QString(),
                  const QString & owner = QString() );


    QString name() const { return m_Name; }
    void setName(const QString & name) { m_Name = name;}
    QString owner() const { return m_Owner; }
    void setOwner(const QString & owner) { m_Owner = owner; }

    void addContact(const FritzBoxPhoneBookContact & contact) {
        m_Contacts.addContact(contact);}

    void attach(const KABC::Addressee::List contactList);
    void attach(const QString fileName);
    void exportFile(const QString fileName) const;
    void deleteContactsWithoutNumbers() {
        m_Contacts.deleteContactsWithoutNumbers(); }

    void print() const;
private:
    /** Attribute name - This is the name of your phonebook */
    QString m_Name;
    /** Attribute owner - I have no idea, what this one does
      - so it will always be the same. */
    QString m_Owner;
    /** List of XML Element <contact/> */
    FritzBoxPhoneBookContactList m_Contacts;

};

//KDebug & operator<< (KDebug &, const FritzBoxPhoneBook &);

#endif // FRITZBOXPHONEBOOK_H
