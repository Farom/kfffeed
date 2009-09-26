#include "fritzboxphonebookcontact.h"

FritzBoxPhoneBookContact::FritzBoxPhoneBookContact(
            const QString & personName,
            const ImportanceType & importance):
        m_Category(importance),
        m_PersonRealName(personName)
{
    //kDebug() << "Contact created";
}


QDomElement
FritzBoxPhoneBookContact::generateDomElement(
        QDomDocument & doc)
const  {
    // @TODO Number-Type = Fax ... ignore Number.
    // it is not useful having a FaxNumber on a Dect Fon.
    if ( ! hasNumbers() ) return QDomElement();
    QDomElement contactE = doc.createElement("contact");
    QDomElement categoryE = doc.createElement("category");
    contactE.appendChild(categoryE);
    QString temp; temp.setNum(m_Category);
    QDomText categoryText = doc.createTextNode( temp );
    categoryE.appendChild( categoryText );
    QDomElement personE = doc.createElement("person");
    contactE.appendChild(personE);
    QDomElement realNameE = doc.createElement("realName");
    personE.appendChild( realNameE );
    QDomText    realNameText = doc.createTextNode( this->m_PersonRealName );
    realNameE.appendChild(realNameText);
    if ( ! m_PersonImageUrl.isEmpty() ) {
        QDomElement imageUrlE = doc.createElement("ImageUrl");
        personE.appendChild( imageUrlE );
        QDomText    imageUrlText = doc.createTextNode( this->m_PersonImageUrl );
        imageUrlE.appendChild(imageUrlText);
    }
    QDomElement telephonyE = doc.createElement("telephony");
    contactE.appendChild(telephonyE);

    QList<QDomNode> nodeList = m_FonNumberList.generateDomElements( doc );
    for (QList<QDomNode>::const_iterator nodeI = nodeList.begin();
                                         nodeI != nodeList.end(); nodeI++) {
        telephonyE.appendChild( *nodeI );
    }
//    for ( int i = 0; i < m_FonNumberList.size(); i++) {
//        QDomElement numberE = m_FonNumberList[i].generateDomElement( doc );
//        telephonyE.appendChild( numberE );
//    }
    // No Idea what that things are for.
    // On my FB 7270 i found them always empty
    QDomElement servicesE = doc.createElement("services");
    contactE.appendChild(servicesE);
    QDomElement setupE = doc.createElement("setup");
    contactE.appendChild(setupE);
    return contactE;
}

bool FritzBoxPhoneBookContact::isValid() const {
    bool valid = true;
    if ( m_PersonRealName.isEmpty() ) {
        valid = false;
        kDebug() << "No Personname is given for this object";
    }
    if ( ! m_FonNumberList.isValid() ) valid = false;
    return valid;
}

bool FritzBoxPhoneBookContact::addNumber(const FritzBoxPhoneNumber & number ) {
    if ( m_FonNumberList.isThereAlreadyANumberOfType( number.type() ) ) {
        return false;
    }
    m_FonNumberList.size();
    if ( m_FonNumberList.size() >= 3 ) {
        return false;
    }
    m_FonNumberList.addNumber(number);
    return true;
}

void FritzBoxPhoneBookContact::print() const {
    kDebug() << "           Kontakt: "
             << m_Category << " "
             << m_PersonRealName
             << " with "
             << m_FonNumberList.size()
             << " Numbers";
    m_FonNumberList.print();
}


bool FritzBoxPhoneBookContactList::isValid() const {
    bool valid = true;
    for ( const_iterator contactI = begin(); contactI != end(); contactI++ ) {
        if ( ! contactI->isValid() ) valid = false;
    }
    // checking that the vanity and the quickdialNumber are eqal
    for ( const_iterator contactI = begin(); contactI != end(); contactI++ )
    {
        /** @TODO for every number check all other numbers or make a list or something */
    }
    return true;
}

void FritzBoxPhoneBookContactList::print() const {
    kDebug() << "       There are "
             << size()
             << " Contacts in this List";
    for (const_iterator contactI=begin(); contactI != end(); contactI++) {
        contactI->print();
    }
}

void FritzBoxPhoneBookContactList::deleteContactsWithoutNumbers() {

    for (iterator contactI = begin(); contactI != end(); ) {
        if ( ! contactI->hasNumbers() ) {
            contactI = erase(contactI);
            continue;
        }
        contactI++;
    }


}


