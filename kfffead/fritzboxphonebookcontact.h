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
    QString person() const { return m_Person; }
    void setPerson(const QString name) { m_Person = name; }
    QDomElement generateDomElement( QDomDocument & doc) const;

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
class FritzBoxPhoneBookContactList : public QList<FritzBoxPhoneBookContact> {
public:
    bool isValid() const;
private:

};

#endif // FRITZBOXPHONEBOOKCONTACT_H
