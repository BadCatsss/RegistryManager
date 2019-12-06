#include <QCoreApplication>
#include "RegistryManager.h"
#include <QTime>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSystemSemaphore semaphore("RegistryManager", 1);
    semaphore.acquire();
    QSharedMemory sharedMemory("RegistryManager sharedMemory");
    bool is_running;
    if ( sharedMemory.attach() )
    {
        is_running = true;
    }
    else
    {
        sharedMemory.create(1);
        is_running = false;
    }
    semaphore.release();
    if(is_running)
    {
        QTextStream(stdout) << "application is already run";
        a.exit(-1);
    }

    //examples
    QString path;

    //    QTextStream stream(stdin);
    //    path = stream.readLine();
    //    RegistryManager::instance().read(path);

    //TIME TESTS
    //QTime t;
    //t.start();

    //READ

    //path =RegistryManager::instance().read("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control");
    //path = RegistryManager::instance().read("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\OS");
    //path = RegistryManager::instance().read("HKEY_CURRENT_USER\\Environment\\Path"); //!!!!!!!!!!!!!!!!!!!!
    //path = RegistryManager::instance().read("HKEY_CURRENT_USER\\Control Panel\\Colors\\ButtonFace");
    //path = RegistryManager::instance().read("HKEY_CURRENT_USER\\Control Panel\\Cursors\\Cursors");

    //WRITE
    //RegistryManager::instance().write("HKEY_CURRENT_USER\\Control Panel\\Cursors\\Cursors","11111");
    //RegistryManager::instance().write("HKEY_CURRENT_USER\\Environment\\Path","111111111");
    //RegistryManager::instance().write("HKEY_CURRENT_USER\\Environment\\TEST1", "22222222");

    //qDebug("Time elapsed: %d sec; %d in ms", t.elapsed()/1000,t.elapsed());
    RegistryManager::instance().printErrorslist();
    return a.exec();
}
