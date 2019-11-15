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
    QTime t;
    t.start();
    //examples
    auto path = RegistryManager::instance().read("HKEY_CURRENT_USER\\Environment\\Path"); // Test1 //Test 4 - ok
    qDebug("Time elapsed: %d sec; %d in ms", t.elapsed()/1000,t.elapsed());
    //RegistryManager::instance().read("HKEY_CURRENT_USER\\Control Panel\\Cursors\\Cursors"); // Test2 - ok
    //RegistryManager::instance().write("HKEY_CURRENT_USER\\Control Panel\\Cursors\\Cursors","2333"); // Test3 - ok //Обратно чтению - двойной путь - подкталог, одинарный - файл
    // RegistryManager::instance().read("HKEY_CURRENT_USER\\Control Panel\\Colors\\ButtonFace");//Test4 - ok
    //QString path;
    //QTextStream stream(stdin);
    //path = stream.readLine();
    //RegistryManager::instance().read(path);
    //RegistryManager::instance().printErrorslist();
    //return a.exec();
}
