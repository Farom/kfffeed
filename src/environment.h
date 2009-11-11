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
    void setConfig(KConfig * config) { m_Config = config; }

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
    KConfig * m_Config;

};
/*
QMap<QChar,int> vanityChars;
vanityChars.insert(' ',1);
vanityChars.insert('A',2);
vanityChars.insert('B',2);
vanityChars.insert('C',2);
vanityChars.insert('D',3);
vanityChars.insert('E',3);
vanityChars.insert('F',3);
vanityChars.insert('G',4);
vanityChars.insert('H',4);
vanityChars.insert('I',4);
vanityChars.insert('J',5);
vanityChars.insert('K',5);
vanityChars.insert('L',5);
vanityChars.insert('M',6);
vanityChars.insert('N',6);
vanityChars.insert('O',6);
vanityChars.insert('P',7);
vanityChars.insert('Q',7);
vanityChars.insert('R',7);
vanityChars.insert('S',7);
vanityChars.insert('T',8);
vanityChars.insert('U',8);
vanityChars.insert('V',8);
vanityChars.insert('W',9);
vanityChars.insert('X',9);
vanityChars.insert('Y',9);
vanityChars.insert('Z',9);
*/
#endif // ENVIRONMENT_H
