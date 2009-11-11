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
        QDomDocument & doc) const
{
    // if there is no phonenumber, no need to export the contact
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
    // kDebug() << "Got NodeList with "  << nodeList.size();
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
    // checking that the vanity and the quickdialNumber are unique
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

void FritzBoxPhoneBookContactList::setVanityNumbersWhereNeeded() {
    // We have to make a list of vanityNumbers and look where we need a new one
    QStringList vanityList;
    typedef QPair<QString, FritzBoxPhoneNumberList::iterator> NameNumberIPair;
    // List of "Name - Contact - Pairs" needing Vanitynumber fixed
    QList<NameNumberIPair> numberItList;
    for (iterator contactI = begin(); contactI != end(); contactI++) {
        for (FritzBoxPhoneNumberList::iterator numberI = contactI->numberList().begin();
             numberI != contactI->numberList().end(); numberI++) {
                 QString vanityNumber = numberI->vanity();
                 if ( ! vanityNumber.isEmpty() ) {
                     vanityList.append(vanityNumber);
                 }
                 int errorCode = numberI->getErrorStatus();
                 int matchingErrorcode;
                 matchingErrorcode = FritzBoxNumber::VanityNumberIsWrong;
                 matchingErrorcode |= FritzBoxNumber::VanityNumberIsMissing;
                 if ( errorCode & matchingErrorcode ) {
                     numberItList.append(qMakePair(contactI->personRealName(),numberI));
                 }
        }
    }
    kDebug() << "We have found "
             << numberItList.size()
             << " where a vanity is requiered.";
    // A Diolog with the User is needed here
    // until implementing this, will generate vanity from first Name
    // Auslagern in eigene Funktion (GUI!!!)
    for ( QList<NameNumberIPair>::iterator numberNPI=numberItList.begin();
              numberNPI != numberItList.end(); numberNPI++)
    {
        QString name = numberNPI->first;
        FritzBoxPhoneNumberList::iterator numberI = numberNPI->second;
        if ( numberI->generateUniqueVanityNumber( name, vanityList ) )
            vanityList.append( numberI->vanity() );
    }
    if ( ! numberItList.isEmpty() )
        kDebug() << "Diese Nummern brauchen eine neue Vanitynummer";
    // NumberNamePairIterator = numberNPI
    for ( QList<NameNumberIPair>::iterator numberNPI=numberItList.begin();
              numberNPI != numberItList.end(); numberNPI++)
    {
        kDebug() << numberNPI->first << "  "
                 << numberNPI->second->number() << " "
                 << numberNPI->second->vanity();

    }

    // for debugging purpose:
//    if ( ! vanityList.isEmpty() ) {
//        kDebug() << " Diese VanityNummern wurden gefunden";
//        for ( QStringList::const_iterator numberI  = vanityList.begin();
//                                      numberI != vanityList.end(); numberI++ )
//        {
//            kDebug() << *numberI;
//        }
//    }
}

void FritzBoxPhoneBookContactList::setQuickDialNumbersWhereNeeded() {
    // We have to make a list of QuickDialNumbers that are already used
    QStringList  quickDialList;
    // List of "Name - Contact - Pairs" needing quickdialnumber fixed
    QList<FritzBoxPhoneNumberList::iterator> numberItList;
    for (iterator contactI = begin(); contactI != end(); contactI++) {
        for (FritzBoxPhoneNumberList::iterator numberI = contactI->numberList().begin();
             numberI != contactI->numberList().end(); numberI++) {
                 QString quickDialNumber = numberI->quickDial();
                 if ( ! quickDialNumber.isEmpty() ) {
                     quickDialList.append(quickDialNumber);
                 }
                 int errorCode = numberI->getErrorStatus();
                 int matchingErrorCode = FritzBoxNumber::QuickDialNumberIsMissing;
                 matchingErrorCode |= FritzBoxNumber::QuickDialNumberIsWrong;
                 if ( errorCode & matchingErrorCode ) {
                     numberItList.append( numberI );
                 }
        }
    }

    // A Diolog with the User is needed here
    // until implementing this,
    // will generate vanity from first 5 letters of Name
    for ( QList<FritzBoxPhoneNumberList::iterator>::iterator numberII = numberItList.begin();
              numberII != numberItList.end(); numberII++)
    {
        FritzBoxPhoneNumberList::iterator numberI = *numberII;
        if ( numberI->generateUniqueQuickDialNumber( quickDialList ) )
            quickDialList.append( numberI->quickDial() );
    }
}

