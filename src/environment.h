#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QtCore>
#include <KConfig>
#include <KConfigGroup>
#include <KDebug>
#include <kcmdlineargs.h>

class Environment
{
public:
    Environment();

    QString localCountryCode() const {return m_LocalCountryCode; }
    void setLocalCountryCode(const QString countryCode);
    QString localAreaCode() const { return m_LocalAreaCode; }
    void setLocalAreaCode(const QString areaCode);
    QString phoneBookName() const { return m_PhoneBookName; }
    void setPhoneBookName(const QString phoneBookName);
    QString netNumbersFile() const { return m_NetNumbersFile; }
    void setNetNumbersFile(const QString netNumbersFile);
        QString outputFileName() const { return m_OutputFileName; }
    void setOutputFileName(const QString outputFileName);
    bool getWriteConfig() const { return m_WriteToConfig; }
    void setWriteConfig(bool DoWriteConfig);

    void readFromConfigFile();
    void readCommandLineArgs(const KCmdLineArgs * args);
    void writeConfig();
    void print();
    bool isValid();

private:
    QString m_LocalCountryCode;
    QString m_LocalAreaCode;
    QString m_PhoneBookName;
    QString m_NetNumbersFile;
    QString m_OutputFileName;
    bool m_WriteToConfig;
    KConfig m_Config;

};

#endif // ENVIRONMENT_H
