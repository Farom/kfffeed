#ifndef FRITZBOXPHONEBOOKCONTACT_H
#define FRITZBOXPHONEBOOKCONTACT_H

#include <QList>
#include <QString>

#include "fritzboxphonenumber.h"

/** @class FritzBoxPhoneBookContact
        This Class describes everything needed for the XML Group <contact)
  */
class FritzBoxPhoneBookContact {
public:
    QString person() const { return m_Person; }
    void setPerson(const QString name) { m_Person = name; }

private:
    /** XML-Element <category/> */
    enum {unimportant=0, important=1} m_Category;
    /** m_Person is the String that appears in your Phonebook on the DectPhone
        XML-Element <person/>
    */
    QString m_Person;

    /** XML-Element <telephony/> */
    FritzBoxPhoneNumberList m_FonNumberList;
};

/** @class FritzBoxPhoneBookContactList
    This class */
class FritzBoxPhoneBookContactList : QList<FritzBoxPhoneBookContact> {
public:
private:

};

#endif // FRITZBOXPHONEBOOKCONTACT_H
