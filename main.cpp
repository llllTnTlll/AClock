#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "helper.h"

// #include <QQmlEngine>
// #include <QQmlComponent>
// #include <QQuickWindow>
// #include <auto_ptr.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<CpuWatcher>("helper", 1, 0, "CpuWatcher");

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/AClock/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl){
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);
    engine.load(url);

    // QObject* rootObject = engine.rootObjects().first();
    // QObject* cpuGauges = rootObject->findChild<QObject*>("cpuGauges");
    // if(cpuGauges!=nullptr){
    //     cpuGauges->setProperty("progressPercent", 30);
    // }


    // QGuiApplication app(argc, argv);
    // QQmlEngine engine;
    // QQmlComponent com(&engine);
    // com.loadUrl(QUrl("qrc:/AClock/Main.qml"));
    // if(com.isError()){
    //     qDebug()<<com.errorString();
    // }

    // std::unique_ptr<QQuickWindow> ct(static_cast<QQuickWindow*>(com.create()));
    // ct->show();

    return app.exec();
}
