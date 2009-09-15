#include <QFile>
#include "fritzboxphonebook.h"

void FritzBoxPhoneBook::attach(const KABC::Addressee::List contactList)
{
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

}

void FritzBoxPhoneBook::attach(const QString fileName)
{


}

void FritzBoxPhoneBook::exportFile(const QString fileName) const
{
    // first we have to generate a new DOM tree
    QDomDocument doc( fileName );
    QFile file("mydocument.xml");
}
