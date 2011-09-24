#ifndef SOWATCH_NOTIFICATIONPROVIDER_H
#define SOWATCH_NOTIFICATIONPROVIDER_H

#include <QtCore/QObject>
#include "notification.h"

namespace sowatch
{

class NotificationProvider : public QObject
{
    Q_OBJECT

protected:
	explicit NotificationProvider(QObject *parent = 0);
	virtual ~NotificationProvider();

signals:
	void incomingNotification(Notification* notification);
};

}

#endif // SOWATCH_NOTIFICATIONPROVIDER_H
