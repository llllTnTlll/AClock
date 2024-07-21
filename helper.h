#ifndef HELPER_H
#define HELPER_H

#include <QObject>
#include <QTimer>
#include <QQmlEngine>
#include <random>

class CpuWatcher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int currentCpuUsage READ currentCpuUsage WRITE setCurrentCpuUsage NOTIFY currentCpuUsageChanged)
    QML_ELEMENT

public slots:
    void updateCpuProperty();
public:
    explicit CpuWatcher(QObject *parent = nullptr): QObject(parent){
        // 启动一个定时器
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &CpuWatcher::updateCpuProperty);
        timer->start(2000);
    }

    int currentCpuUsage(){
        return currentUsage;
    }

    void setCurrentCpuUsage(int value){
        currentUsage = value;
        emit currentCpuUsageChanged();
    }

private:
    int currentUsage=0;
signals:
    void currentCpuUsageChanged();
    // void cpuValueChanged(int cpuValue);
};

#endif // HELPER_H
