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

	int getCount(Notification::Type type);
protected:
	MNotificationManagerInterface* _manager;
	WatchNotificationSink* _sink;

protected slots:
	void sinkNotification(const sowatch::Notification &n);
	void sinkUnreadCountChanged(sowatch::Notification::Type type);

friend class WatchNoficationSink;
};

}

#endif // MEEGOHANDSETNOTIFICATIONPROVIDER_H
