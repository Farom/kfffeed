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
public:
    void attach(const KABC::Addressee::List contactList);
    void attach(const QString fileName);
    void exportFile(const QString fileName) const;
private:
    /** Attribute name - This is the name of your phonebook */
    QString m_Name;
    /** Attribute owner - I have no idea, what this one does
      - so it will always be the same. */
    QString m_Owner;
    /** List of XML Element <contact/> */
    FritzBoxPhoneBookContactList m_Contacts;

};

#endif // FRITZBOXPHONEBOOK_H
