#pragma once
#include <qstring.h>

#include <QMutex>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QSettings>
#include<QDir>
#include <QtDebug>

class RegistryManagerDestroyer;
class RegistryManager
{
private:
    RegistryManager(const RegistryManager& param) = delete;
    RegistryManager();
    RegistryManager& operator = (RegistryManager& param) = delete ;

    bool checkHiveKeyCorrectnes(const QString& path);
    bool checkKeyCorrectnes(const QString& path);

    void addErrorToList(const QString& param);
    const QStringList& getErrorsList();

    QVector<QString> hiveKeys;
    QStringList errorsList;
    QStringList pathParts;

public:
    static RegistryManager& instance();
    bool write(const QString& key, const QString& value);
    const QString read(const QString& key);
    void printErrorslist();
};
