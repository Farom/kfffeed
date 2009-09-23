#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>

#include <kabc/stdaddressbook.h>

#include "fritzboxphonebook.h"
#include "qphonenumberstring.h"

int main( int argc, char **argv )
{
    KAboutData aboutData("kfffeed", "kfffeed", ki18n("kfffeed"), "0.1",
                         ki18n("Transfer Contactdate from kaddressbook to FritzBox 7270"),
                         KAboutData::License_GPL_V2);
    aboutData.addAuthor(ki18n("Björn Lässig"));

    KCmdLineArgs::init( argc, argv, &aboutData );
    KApplication app( false );


    // Here is a problem with the input-encoding if there are some "üöä" in it
    FritzBoxPhoneBook phoneBook(QString("Phonebook"),"1");

    // this will implemented later
    //phoneBook.attach("FRITZ.Box_Telefonbuch.xml");

    // load the address book into memory
    KABC::AddressBook *addressBook = KABC::StdAddressBook::self( false );
    const KABC::Addressee::List contacts = addressBook->allAddressees();
    phoneBook.attach(contacts);
    //phoneBook.print();
    phoneBook.exportFile("KAddressbook-Fritz-Box-Addressbook.xml");
    kDebug() << "outputfile written";

    // This is for testing of the new lib for phoneNumbers
    // i have to build a list of all numbers from my addressbook for checking
    // my regular expressions
//    QStringList numList;
//
//    for ( KABC::Addressee::List::const_iterator contactI = contacts.begin();
//          contactI != contacts.end(); contactI++) {
//        KABC::PhoneNumber::List phoneNumbers = contactI->phoneNumbers();
//        KABC::PhoneNumber::List::const_iterator phoneNumber = phoneNumbers.begin();
//        for (; phoneNumber != phoneNumbers.end(); phoneNumber++) {
//            numList.append( phoneNumber->number() );
//        }
//    }

//    for (QStringList::const_iterator numI = numList.begin(); numI != numList.end(); numI++) {
//        QPhoneNumberString foo = *numI;
//        kDebug() << "\"" << foo.isCallableByFB7270() <<"\""
//                << foo.countryCode()
//                << "    " << foo;
//    }

    QPhoneNumberString number("+495313492999") ;
    number.recognizeNumber();
    kDebug() << number << "  " << number.prettyPrint();
    //kDebug() << number.countryCode();
    // print out the element names of all elements that are direct children
    // of the outermost element.
//    QDomElement phoneNetRootE = phoneNetDoc.documentElement();
//    QString prefix = phoneNetRootE.attribute("prefix"); // the node really is an element.
//    int length = prefix.size();
//    if ( number.startsWith(prefix) ) {
//        kDebug() << phoneNetRootE.tagName();
//    }
//    phoneNetRootE = phoneNetRootE.nextSiblingElement("Net");
//    if ( phoneNetRootE.isNull() ) kDebug() << "es gibt nur ein Element auf root";

//    QDomNode n = phoneNetDoc.firstChild();
//    while(!n.isNull()) {
//        QDomElement e = n.toElement(); // try to convert the node to an element.
//        if(!e.isNull()) {
//    kDebug() << phoneNetRootE.tagName(); // the node really is an element.
//    kDebug() << phoneNetRootE.attribute("prefix");
//    kDebug() << phoneNetRootE.attribute("netName");
//    kDebug() << phoneNetRootE.attribute("geoDataName");
//    kDebug() << phoneNetRootE.attribute("postFixBegin");
//    kDebug() << phoneNetRootE.attribute("postFixEnd");
    //        }
//     n = n.nextSibling();
//    }
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

