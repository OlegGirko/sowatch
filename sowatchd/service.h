#ifndef SERVICE_H
#define SERVICE_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QSettings>

#include <sowatch.h>

namespace sowatch
{

class Service : public QObject
{
    Q_OBJECT
public:
    explicit Service(QObject *parent = 0);

public slots:
	void terminate();

};

}

#endif // SERVICE_H
