#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
//#include <pthread.h>

#include "wifi.h"



//void *recv_threadfuntion(void* arg)
//{
//    while(1)
//    {
//        w.receiveFromServer();
//    }
//}


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    //Wifi Init
    Wifi w;
    w.init();
    //pthread_t receive_thread;

    qmlRegisterType< Wifi > ("Wifi", 1 , 0 , "Wifi");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

//    ::pthread_create(&receive_thread, 0, recv_threadfuntion, NULL);
//    pthread_detach(receive_thread);

    return app.exec();
}
