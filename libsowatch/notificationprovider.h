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

public:
	virtual int getCount(Notification::Type type) = 0;

signals:
	void notification(const Notification& n);
	void unreadCountChanged(Notification::Type type);

};

}

#endif // SOWATCH_NOTIFICATIONPROVIDER_H
