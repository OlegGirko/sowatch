#ifndef WATCHMANAGER_H
#define WATCHMANAGER_H

#include <QObject>

namespace sowatch
{

class WatchManager : public QObject
{
    Q_OBJECT
public:
    explicit WatchManager(QObject *parent = 0);

signals:

public slots:

};

}

#endif // WATCHMANAGER_H
