#ifndef MEEGOHANDSETNOTIFICATIONPROVIDER_H
#define MEEGOHANDSETNOTIFICATIONPROVIDER_H

#include <sowatch.h>

class MNotificationManagerInterface;
class WatchNotificationSink;

namespace sowatch
{

class MeegoHandsetNotificationProvider : public NotificationProvider
{
    Q_OBJECT
public:
    explicit MeegoHandsetNotificationProvider(QObject *parent = 0);
	~MeegoHandsetNotificationProvider();

protected:
	MNotificationManagerInterface* _manager;
	WatchNotificationSink* _sink;

protected slots:
	void newNotification(sowatch::Notification *n);

friend class WatchNoficationSink;
};

}

#endif // MEEGOHANDSETNOTIFICATIONPROVIDER_H
