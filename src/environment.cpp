#include "environment.h"

Environment::Environment():
    m_LocalCountryCode("49"),
    m_LocalAreaCode(),
    m_PhoneBookName("Telefonbuch"),
    m_NetNumbersFile("/src/foo/netnumbers.xml"),
    m_OutputFileName("export.xml"),
    m_Config(NULL)

{
}

void Environment::setLocalCountryCode(const QString countryCode) {
    m_LocalCountryCode = countryCode;
}
void Environment::setLocalAreaCode(const QString areaCode)  {
    m_LocalAreaCode = areaCode;
}
void Environment::setPhoneBookName(const QString phoneBookName) {
    m_PhoneBookName = phoneBookName;
}
void Environment::setNetNumbersFile(const QString netNumbersFile) {
    m_NetNumbersFile = netNumbersFile;
}
void Environment::setOutputFileName(const QString outputFileName) {
    m_OutputFileName = outputFileName;
}

void Environment::setWriteConfig(bool DoWriteConfig){
    m_WriteToConfig = DoWriteConfig;
}



void Environment::readFromConfigFile() {
    KConfigGroup configG = m_Config->group("kfffeed");
    QString entry =  configG.readEntry("country-code");
    if ( ! entry.isEmpty())
        setLocalCountryCode( entry );
    entry =  configG.readEntry("area-code");
    if ( ! entry.isEmpty())
        setLocalAreaCode( entry );
    entry = configG.readEntry("phonebook-name");
    if ( ! entry.isEmpty())
        setPhoneBookName( entry );
    entry = configG.readEntry("output-file");
    if ( ! entry.isEmpty())
        setOutputFileName( entry );
    entry = configG.readEntry("netnumbers-file");
    if ( ! entry.isEmpty())
        setNetNumbersFile( entry );
}

void Environment::readCommandLineArgs(const KCmdLineArgs * args) {
    if ( args->isSet("country-code") )
        setLocalCountryCode( args->getOption("country-code") );
    if ( args->isSet("area-code") )
        setLocalAreaCode( args->getOption("area-code") );
    if ( args->isSet("phonebook-name") )
        setPhoneBookName( args->getOption("phonebook-name" ) );
    if ( args->isSet("output-file") )
        setOutputFileName( args->getOption("output-file") );
    if ( args->isSet("netnumbers-file") )
        setNetNumbersFile( args->getOption("netnumbers-file") );
    m_WriteToConfig =  args->isSet("write-to-config");

}

void Environment::writeConfig() {
    KConfigGroup confGroup = m_Config->group("kfffeed");
    if ( ! m_OutputFileName.isEmpty() )
        confGroup.writeEntry("output-file",m_OutputFileName);
    if ( ! m_NetNumbersFile.isEmpty() )
        confGroup.writeEntry("netnumbers-file",m_NetNumbersFile);
    if ( ! m_LocalAreaCode.isEmpty() )
        confGroup.writeEntry("area-code",m_LocalAreaCode);
    if ( ! m_LocalCountryCode.isEmpty() )
        confGroup.writeEntry("country-code",m_LocalCountryCode);
    if ( ! m_PhoneBookName.isEmpty() )
    confGroup.writeEntry("phonebook-name", m_PhoneBookName);
    m_Config->sync();
}

void Environment::print() {
    kDebug() << "OutputFile     : " << m_OutputFileName;
    kDebug() << "NetNumbersFile : " << m_NetNumbersFile;
    kDebug() << "CountryCode    : " << m_LocalCountryCode;
    kDebug() << "AreaCode       : " << m_LocalAreaCode;
    kDebug() << "PhonebookName  : " << m_PhoneBookName;
}

bool Environment::isValid() {
    // is valid if Netnumbersname is found and Outputfile ist set.
    if (m_OutputFileName.isEmpty()) {
        kDebug() << "Outputfilename ist empty";
        return false;
    }
    if ( ! QFile(m_NetNumbersFile).exists()) {
        kDebug() << "netnumbers-XML-file does not exist" << m_NetNumbersFile;
        return false;
    }
    return true;
}
