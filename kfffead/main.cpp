#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>

#include <kabc/stdaddressbook.h>

#include "fritzboxphonebook.h"
#include "../QtPhoneNumberString/qtphonenumberstring.h"

int main( int argc, char **argv )
{
    KAboutData aboutData("testapp", "testapp", ki18n("testapp"), "0.1",
                         ki18n("testapp"),
                         KAboutData::License_GPL_V2);

    KCmdLineArgs::init( argc, argv, &aboutData );
    KApplication app( false );

    // load the address book into memory
    KABC::AddressBook *addressBook = KABC::StdAddressBook::self( false );

    const KABC::Addressee::List contacts = addressBook->allAddressees();

    kDebug() << "Jetzt geht es los";

    FritzBoxPhoneBook phoneBook("Björns FritzFon","Björn");
    //phoneBook.attach("FRITZ.Box_Telefonbuch.xml");
    phoneBook.attach(contacts);
    //phoneBook.print();
    phoneBook.exportFile("KAddressbook-Fritz-Box-Addressbook.xml");
    kDebug() << "AusgabeDatei geschrieben";

    // This is for testing of the new lib for phoneNumbers
    // i have to build a list of all numbers from my addressbook for checking
    // my regular expressions
    QStringList numList;
    KABC::Addressee::List::const_iterator contactI = contacts.begin();
    for ( ; contactI != contacts.end(); contactI++) {
        KABC::PhoneNumber::List phoneNumbers = contactI->phoneNumbers();
        KABC::PhoneNumber::List::const_iterator phoneNumber = phoneNumbers.begin();
        for (; phoneNumber != phoneNumbers.end(); phoneNumber++) {
            numList.append( phoneNumber->number() );
        }
    }

    for (QStringList::const_iterator numI = numList.begin(); numI != numList.end(); numI++) {
        QtPhoneNumberString foo = *numI;
        kDebug() << "\"" << foo.isCallableByFB7270() <<"\""
                << "    " << foo;

    }
    QtPhoneNumberString foo;
    foo.isUnknownNumberType();

    return 0;
}

//    KABC::Addressee::List::const_iterator contactI = contacts.begin();
//    for ( ; contactI != contacts.end(); contactI++) {
//        kDebug() << "Name:" << contactI->assembledName();
//        KABC::PhoneNumber::List phoneNumbers = contactI->phoneNumbers();
//        KABC::PhoneNumber::List::const_iterator phoneNumber = phoneNumbers.begin();
//        for (; phoneNumber != phoneNumbers.end(); phoneNumber++) {
//            kDebug() << "    (Nummer): "
//                     << phoneNumber->number()
//                     << " Type: " << phoneNumber->typeLabel();
//        }
//    }

