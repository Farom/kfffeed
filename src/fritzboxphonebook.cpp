#include "fritzboxphonebook.h"

FritzBoxPhoneBook::FritzBoxPhoneBook(
        const QString & name,
        const QString & owner):
m_Name(name),
m_Owner(owner)
{
    kDebug() << "FBPhoneBook created";
}


void FritzBoxPhoneBook::attach(const KABC::Addressee::List contacts)
{
    KABC::Addressee::List::const_iterator contactI = contacts.begin();
    for ( ; contactI != contacts.end(); contactI++) {
//        kDebug() << "Now are "
//             << m_Contacts.size()
//             << " Contacts in List ";
        // If a Name has 3 work numbers or 5 Phones it is necessary to generate
        // 2 or more entrys of the same name eg "Björn Lässig (2)"
        int count = 1; // counts the necessary doubled Names

        FritzBoxPhoneBookContact fbContact(contactI->formattedName().simplified());
        // Dies wurde eingefuegt um die Namen in Werners Adressbuch zu finden :-(
        KABC::Addressee contact = *contactI;
        if ( contact.formattedName().simplified().isEmpty() || true) {
            kDebug() << "Kontakt gefunden, der leer ist"
                    << "formattedName: "  << contact.formattedName() << endl
                    << "assembledName: "  << contact.assembledName() << endl
                    << "additionalName" << contact.additionalName() << endl
                    << "familyName" << contact.familyName() << endl
                    << "formattedName" << contact.formattedName() << endl
                    << "givenName" << contact.givenName() << endl
                    << "name" << contact.name() << endl
                    << "realname" << contact.realName() << endl;
            kDebug() << "Der Grundt dafür ist völlig unklar, bitte schicken sie diese "
                    << "Liste per email an den Entwickler (sie können sie gerne anonymisieren"
                    << endl;
        }

        KABC::PhoneNumber::List phoneNumbers = contactI->phoneNumbers();
        KABC::PhoneNumber::List::const_iterator phoneNumber = phoneNumbers.begin();
        for (; phoneNumber != phoneNumbers.end(); phoneNumber++) {
            FritzBoxPhoneNumber fbNumber(phoneNumber->number().simplified());
            if ( phoneNumber->type() & KABC::PhoneNumber::Fax ) continue;
            if ( phoneNumber->type() & KABC::PhoneNumber::Cell )
                fbNumber.setType(FritzBoxPhoneNumber::Mobile);
            if ( phoneNumber->type() & KABC::PhoneNumber::Home )
                fbNumber.setType(FritzBoxPhoneNumber::Home);
            if ( phoneNumber->type() & KABC::PhoneNumber::Work )
                fbNumber.setType(FritzBoxPhoneNumber::Work);
            // yes there is a bug, a number can have more than one Flag

            // @TODO find best numberType forthe label
            bool hasAdded = fbContact.addNumber(fbNumber);
            if (! hasAdded) {
                m_Contacts.addContact(fbContact);
                QString newName = contactI->formattedName().simplified() + " " + QString().number(count);
                count++;
                fbContact = FritzBoxPhoneBookContact(newName);
                hasAdded = fbContact.addNumber(fbNumber);
                if (! hasAdded)
                    kDebug() << "something really wicked happened";
            }
        }
        m_Contacts.addContact(fbContact);
    }

}

void FritzBoxPhoneBook::attach(const QString fileName)
{
    kDebug() << "Attaching Files not yet implemented";

}

void FritzBoxPhoneBook::exportFile(const QString fileName) {
    deleteContactsWithoutNumbers();
    m_Contacts.setVanityNumbersWhereNeeded();
    m_Contacts.setQuickDialNumbersWhereNeeded();

    /****************************************************/
    /************ initialize the xml-dom tree ***********/
    /****************************************************/

    // first we have to generate a new DOM tree
    QDomDocument phBook;
    // setting the version and encoding is necessary but looks wrong
    // @TODO ask for better style
    QString processingInstruction = "version=\"1.0\" encoding=\"iso-8859-1\"";
    // this means xml-Processing-Instructions
    QDomProcessingInstruction xPI =
            phBook.createProcessingInstruction("xml", processingInstruction);
    phBook.appendChild( xPI );

    /****************************************************/
    /************ Filling with content ******************/
    /****************************************************/
    QDomElement root=phBook.createElement("phonebooks");
    phBook.appendChild( root );

    /** this is the Basenode for all Contacts */
    QDomElement phonebookE = phBook.createElement("phonebook");
    /** @TODO these ones are optional, if empty
        do not put it in the element */
    if ( ! this->m_Name.isEmpty() )
        phonebookE.setAttribute("name", this->m_Name);
    if ( ! this->m_Owner.isEmpty() )
        phonebookE.setAttribute("owner", this->m_Owner);
    root.appendChild(phonebookE);

    FritzBoxPhoneBookContactList::const_iterator contactI = m_Contacts.begin();
    for (; contactI != m_Contacts.end(); contactI++) {
        QDomElement contactE = contactI->generateDomElement( phBook );
        if ( ! contactE.isNull() ) phonebookE.appendChild(contactE);
    }

    /****************************************************/
    /************ Writing the dom tree to a file ********/
    /****************************************************/
    QFile file( fileName );
    if ( !file.open( QIODevice::WriteOnly ) ) {
        /** @TODO exceptions verbauen */
        kDebug() << " could not write to file: "
                 << fileName;
        exit(3);
    }
    const int indentSize = 4;
    QTextStream ts( &file );
    ts.setCodec("ISO 8859-1");
    phBook.save(ts, indentSize);
    file.close();
    kDebug() << "Wrote XML exportfile for "
             << m_Contacts.size()
             << " Contacts to "
             << fileName;

}




void FritzBoxPhoneBook::print() const {
    kDebug() << "Printing Phonebook";
    kDebug() << "   Name: " << m_Name << "   Owner: "<< m_Owner;
    m_Contacts.print();

}

//KDebug & operator<< (KDebug & stream, const FritzBoxPhoneBook & book){
//    stream << "Phonebook   : Owner "
//           << book.m_Owner
//           << "     Name: "
//           << book.m_Name;
//    return stream;
//}
