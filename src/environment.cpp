#include "environment.h"

Environment::Environment():
    m_LocalCountryCode("49"),
    m_LocalAreaCode(),
    m_PhoneBookName("Telefonbuch"),
    m_NetNumbersFile("/src/foo/netnumbers.xml"),
    m_OutputFileName("export.xml"),
    m_Config("kfffeedrc", KConfig::SimpleConfig)

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
    setLocalCountryCode( m_Config.group("kfffeed").readEntry("country-code") );
    setLocalAreaCode( m_Config.group("kfffeed").readEntry("area-code") );
    setPhoneBookName( m_Config.group("kfffeed").readEntry("phonebook-name") );
    setOutputFileName( m_Config.group("kfffeed").readEntry("output-file") );
    setNetNumbersFile( m_Config.group("kfffeed").readEntry("netnumbers-file") );
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
    KConfigGroup confGroup = m_Config.group("kfffeed");
    confGroup.writeEntry("output-file",m_OutputFileName);
    confGroup.writeEntry("netnumbers-file",m_NetNumbersFile);
    confGroup.writeEntry("area-code",m_LocalAreaCode);
    confGroup.writeEntry("country-code",m_LocalCountryCode);
    confGroup.writeEntry("phonebook-name", m_PhoneBookName);
    m_Config.sync();
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
