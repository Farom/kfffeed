#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kabc/stdaddressbook.h>
#include "fritzboxphonebook.h"
#include "qphonenumberstring.h"
#include "environment.h"

Environment env;

KCmdLineArgs * analyzeCmdLineOptions() {
    KCmdLineOptions options;
    options.add("cc");
    options.add("country-code <number>",
                ki18n("Country code for your place"));
    options.add("ac");
    options.add("area-code <number>",
                ki18n("Area code number for your place"));
    options.add("pn");
    options.add("phonebook-name <name>",ki18n("Name of you Phonebook"));
    options.add("o");
    options.add("output-file <filename>",
                ki18n("Filename where the export should be saved"));
    options.add("nn");
    options.add("netnumbers-file <filename>",
                ki18n("which phonenet xml description file to use"));
    options.add("w");
    options.add("write-to-config", ki18n("write options of commandline-configuration as defaults to configfile"));
    KCmdLineArgs::addCmdLineOptions( options );

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

    return args;
}

int main( int argc, char **argv )
{
    KAboutData aboutData("kfffeed", "kfffeed", ki18n("kfffeed"), "0.2pre2",
                         ki18n("Transfer contacts from kaddressbook to FritzBox 7270 xml format"),
                         KAboutData::License_GPL_V2);
    aboutData.addAuthor(ki18n("Björn Lässig"));
    KCmdLineArgs::init( argc, argv, &aboutData );
    KCmdLineArgs *args = analyzeCmdLineOptions();
    KApplication app( false );
    KConfig config("kfffeedrc",KConfig::SimpleConfig);
    env.setConfig( &config );
    env.readFromConfigFile();
    env.readCommandLineArgs(args);

    if (env.getWriteConfig()) {
        env.writeConfig();
        env.print();
        if (env.isValid() ) {
            kDebug() << "Config is valid. Export could be probably created.";
        } else {
            kDebug() << "Config is not valid. Change config and retry.";
        }
        exit(0);
    }

    if ( ! env.isValid() ) {
        kDebug() << "Environment is not valid:";
        env.print();
        exit(2);
    }

    // Creating a phonebook
    FritzBoxPhoneBook phoneBook( env.phoneBookName(), "1" );

    // this will implemented later
    // phoneBook.attach("FRITZ.Box_Telefonbuch.xml");

    // load the kaddressbook-contacts into memory
    KABC::AddressBook *addressBook = KABC::StdAddressBook::self( false );
    const KABC::Addressee::List contacts = addressBook->allAddressees();
    phoneBook.attach(contacts);
    //phoneBook.print();
    QPhoneNumberString::setNetNumbersFileName(env.netNumbersFile());
    QPhoneNumberString::staticInitialize(env.localCountryCode(),
                                         env.localAreaCode());
    phoneBook.exportFile(env.outputFileName());

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
//
//    QPhoneNumberString::staticInitialize("49","531","391");
//    kDebug() << QPhoneNumberString::m_RootElementPhoneNetXML.tagName();
//    for (QStringList::const_iterator numI = numList.begin();
//                                     numI != numList.end(); numI++)
//    {
//        QPhoneNumberString number = *numI;
//        number.recognizeNumber();
//        kDebug() << number << "  "
//                 << number.prettyPrint() << "   "
//                 << number.shortFBnumber();
//
//    }
