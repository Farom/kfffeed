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
    kDebug() << "Contact Liste wird gebaut";

    KABC::Addressee::List::const_iterator contactI = contacts.begin();
    for ( ; contactI != contacts.end(); contactI++) {
        kDebug() << "Now are "
             << m_Contacts.size()
             << " Contacts in List ";
        // If a Name has 3 work numbers or 5 Phones it is necessary to generate
        // 2 or more entrys of the same name eg "Björn Lässig (2)"
        int count = 1; // counts the necessary doubled Names

        FritzBoxPhoneBookContact fbContact(contactI->assembledName());

        KABC::PhoneNumber::List phoneNumbers = contactI->phoneNumbers();
        KABC::PhoneNumber::List::const_iterator phoneNumber = phoneNumbers.begin();
        for (; phoneNumber != phoneNumbers.end(); phoneNumber++) {
            FritzBoxPhoneNumber fbNumber(phoneNumber->number());
            // @TODO find best numberType forthe label
            kDebug() << fbContact.person() <<  "    (Nummer): " << phoneNumber->number()
                     << " Type: " << phoneNumber->typeLabel();
            bool hasAdded = fbContact.addNumber(fbNumber);
            if (! hasAdded) {
                kDebug() << "Beim hinzufüge ist etwas schief gegangen";
                m_Contacts.addContact(fbContact);
                QString newName = contactI->assembledName() + " " + QString(count);
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

void FritzBoxPhoneBook::exportFile(const QString fileName) const
{
    kDebug() << "Create XML exportfile for "
             << m_Contacts.size()
             << " Contacts";
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
    QDomElement phonebook = phBook.createElement("phonebook");
    /** @TODO these ones are optional, if empty
        do not put it in the element */
    if ( ! this->m_Name.isEmpty() )
        phonebook.setAttribute("name", this->m_Name);
    if ( ! this->m_Owner.isEmpty() )
        phonebook.setAttribute("owner", this->m_Owner);
    root.appendChild(phonebook);

    FritzBoxPhoneBookContactList::const_iterator contactI = m_Contacts.begin();
    for (; contactI != m_Contacts.end(); contactI++) {
        QDomElement contactElement = contactI->generateDomElement( phBook );
        phonebook.appendChild(contactElement);
    }


    /****************************************************/
    /************ Writing the dom tree to a file ********/
    /****************************************************/
    QFile file( fileName );
    if ( !file.open( QIODevice::WriteOnly ) )
        /** @TODO exceptions verbauen */
        return;
    const int indentSize = 4;
    QTextStream ts( &file );
    phBook.save(ts, indentSize);
    file.close();

}
