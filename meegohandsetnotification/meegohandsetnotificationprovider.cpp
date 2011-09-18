#include <QtDBus/QDBusConnection>
#include <notificationsystem/metatypedeclarations.h>
#include <notificationsystem/notificationsinkadaptor.h>
#include "watchnotificationsink.h"
#include "mnotificationmanagerinterface.h"
#include "meegohandsetnotificationprovider.h"

using namespace sowatch;

MeegoHandsetNotificationProvider::MeegoHandsetNotificationProvider(QObject *parent) :
	sowatch::NotificationProvider(parent),
	_manager(new MNotificationManagerInterface("com.meego.core.MNotificationManager", "/notificationsinkmanager", QDBusConnection::sessionBus(), this)),
	_sink(new WatchNotificationSink(this))
{
	qDBusRegisterMetaType< ::Notification >(); // Avoid a name collision with sowatch::Notification
	qDBusRegisterMetaType<QList< ::Notification > >();
	qDBusRegisterMetaType<NotificationGroup>();
	qDBusRegisterMetaType<QList<NotificationGroup> >();
	qDBusRegisterMetaType<NotificationParameters>();

	new NotificationSinkAdaptor(_sink);
	QDBusConnection::sessionBus().registerService("com.javispedro.sowatch.MeegoHandsetNotificationSink");
	QDBusConnection::sessionBus().registerObject("/meegohandsetnotificationsink", _sink);

	connect(_sink, SIGNAL(incomingNotification(sowatch::Notification)),
			SLOT(sinkNotification(sowatch::Notification)));
	connect(_sink, SIGNAL(countsChanged(sowatch::Notification::Type)),
			SLOT(sinkUnreadCountChanged(sowatch::Notification::Type)));

	_manager->registerSink("com.javispedro.sowatch.MeegoHandsetNotificationSink", "/meegohandsetnotificationsink");
}

MeegoHandsetNotificationProvider::~MeegoHandsetNotificationProvider()
{

}

int MeegoHandsetNotificationProvider::getCount(sowatch::Notification::Type type)
{
	return _sink->getCount(type);
}

void MeegoHandsetNotificationProvider::sinkNotification(const Notification &n)
{
	emit notification(n);
}

void MeegoHandsetNotificationProvider::sinkUnreadCountChanged(Notification::Type type)
{
	emit unreadCountChanged(type);
}
