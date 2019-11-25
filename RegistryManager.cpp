#include "RegistryManager.h"

RegistryManager::RegistryManager()
{
    this->hiveKeys.push_back("HKEY_CLASSES_ROOT");
    this->hiveKeys.push_back("HKEY_CURRENT_USER");
    this->hiveKeys.push_back("HKEY_LOCAL_MACHINE");
    this->hiveKeys.push_back("HKEY_USERS");
    this->hiveKeys.push_back("HKEY_CURRENT_CONFIG");
}
RegistryManager& RegistryManager::instance()
{
    static RegistryManager instance;
    return instance;
}
bool RegistryManager::checkHiveKeyCorrectnes(const QString& path )
{
    for (int i = 0; i < 5; ++i)
    {
        if ( path.toStdString().find( RegistryManager::hiveKeys[i].toStdString() ) != std::string::npos )
        {
            return true;
        }
    }
    addErrorToList("HiveKey is incorrect");
    return false;
}
bool RegistryManager::checkKeyCorrectnes(const QString& path)
{
    pathParts = path.split( QRegExp( "[/\\\\]" ) );
    QString pathh = pathParts[0];
    if (pathParts.size() >= 2)
    {
        for (int var = 1; var < pathParts.size(); ++var)
        {
            QSettings st(pathh, QSettings::NativeFormat);

            if ( st.childGroups().contains( pathParts[var] ) || st.childKeys().contains( pathParts[var] ) )
            {
                if ( ! ( st.childGroups().contains( pathParts[var] ) && st.childKeys().contains( pathParts[var] ) && var == pathParts.length() - 1 )   )
                {
                    //                     qDebug()<<"groups"<<endl<<st.childGroups()<<endl;
                    //если st.childGroups().contains(list[var]) && st.allKeys().contains(list[var])
                    //например: HKEY_CURRENT_USER\Control Panel\Cursors\Cursors\Cursors
                    // - иначе, если так, отдаем предпочтение каталогу и подкаталогу - не добавляя  "\\" - и не переходя к одноименной записи
                    //var == list.length() - 1 - указан файл - т.е далее, нет подразделов в пути
                    pathh = pathh + "\\" + pathParts[var];
                }

                else if(st.childGroups().contains( pathParts[var] ) || var == pathParts.length() -1 )
                {
                    if( st.allKeys().contains( pathParts[var] )  ||  st.allKeys().size() != 0)
                    {
                        return true;
                    }
                    return false;
                }

                else
                {
                    addErrorToList("Key or subkey is wrong. Cant write value");
                    return false;
                }
            }
            else return false;
        }
        return true;
    }

    addErrorToList("Too short path");
    return false;

}
const QStringList& RegistryManager::getErrorsList()
{
    return errorsList;
}
void RegistryManager::addErrorToList(const QString &param)
{
    errorsList.push_back(param);
}
void RegistryManager::printErrorslist()
{
    if ( RegistryManager::instance().getErrorsList().length() != 0 )
    {
        for ( auto& i : RegistryManager::instance().getErrorsList() )
        {
            qWarning() << i << endl;
        }
    }
}
bool RegistryManager::write(const QString& key, const QString& value)
{
    QSettings settings(key,QSettings::NativeFormat);
    if ( checkHiveKeyCorrectnes(key) )
    {
        read(key);
        if ( checkKeyCorrectnes(key) )
        {
            settings.setValue(QString::fromStdString(key.toStdString().erase(0, key.toStdString().find_last_of("\\") + 1) ), value);
        }
        else
        {
            addErrorToList("Key or subkey is wrong. Cant write value");
            return false;
        }
    }
    else
    {
        addErrorToList("Hive Key is wrong. Cant write value");
        return false;
    }
    return true;
}
const QString RegistryManager::read(const QString& key)
{
    QSettings settings(key, QSettings::NativeFormat);
    if ( checkHiveKeyCorrectnes( key ) )
    {
        if ( checkKeyCorrectnes( key ) )
        {
            QMap<QString, QString> registryBranch;
            QStringList keyList = settings.allKeys();
            for(int i = 0; i < keyList.size(); i++)
            {
                registryBranch.insert(keyList[i], settings.value(keyList[i]).toString());
            }

            for (int var = 0; var < registryBranch.size(); ++var)
            {
                qDebug() << "KEY: " << keyList[var] << "  " << "VALUE " << registryBranch.find( keyList[var] ).value() << endl;
            }

            if (keyList.length() == 0)
            {
                //////////////////////////// TEST 5///////////////////////////////
                QString slicePart = pathParts[0];
                for (int i = 1; i < pathParts.size() - 1; ++i)
                {
                    slicePart=  slicePart + "\\" + pathParts[i] ;
                }
                QSettings settings1(slicePart, QSettings::NativeFormat);
                QStringList keyList1 = settings1.allKeys();
                for(int i = 0; i < keyList1.size(); i++)
                {
                    registryBranch.insert( keyList1[i], settings.value( keyList1[i] ).toString() );
                }
                qDebug() << "KEY: " << key << "  " << "VALUE " << settings1.value(pathParts[ pathParts.size() - 1 ], QSettings::NativeFormat).toString()<< endl;
                return QString ( settings1.value(pathParts[ pathParts.size() - 1 ], QSettings::NativeFormat).toString() );

            }
            //////////////////////////// TEST 4///////////////////////////////
            //qDebug() << "KEY: " << keyList[0] << "  " << "VALUE " << registryBranch.find(keyList[0]).value()<< endl;
            return QString ( registryBranch.find( keyList[0] ).value() );
            //////////////////////////// TEST 4///////////////////////////////
        }
        else
        {
            addErrorToList("Key or subkey is wrong. Cant read value");
        }
    }
    else
    {
        addErrorToList("Hive Key is wrong. Cant write value");
    }
}



