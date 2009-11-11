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

    QString personRealName() const { return m_PersonRealName; }
    void setPersonRealName(const QString name) { m_PersonRealName = name; }
    QString personImageUrl() const { return m_PersonImageUrl; }
    void setPersonImageUrl(const QString url) { m_PersonImageUrl = url; }

    bool isValid() const;
    QDomElement generateDomElement( QDomDocument & doc) const;
    /// brief Method adds a PhonenumberObjekt to the contactlist
    /** return  gives, whether it was possible to add the number */
    bool addNumber(const FritzBoxPhoneNumber & number );
    FritzBoxPhoneNumberList& numberList() {return m_FonNumberList; }
    void print() const;
    bool hasNumbers() const { return ! m_FonNumberList.isEmpty(); }

private:
    /** XML-Element <category/> */
    ImportanceType m_Category;
    /** m_PersonRealName is the String that appears in your Phonebook on the DectPhone
        XML-Element <person> <realName /> </person>
    */
    QString m_PersonRealName;
    /** m_PersonImageUrl is the String that appears in your Phonebook on the DectPhone
        XML-Element <person> <ImageUrl/> </person>
    */
    QString m_PersonImageUrl;

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
    void setVanityNumbersWhereNeeded();
    void setQuickDialNumbersWhereNeeded();
private:

};

#endif // FRITZBOXPHONEBOOKCONTACT_H
