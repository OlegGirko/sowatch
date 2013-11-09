#ifndef SOWATCH_WATCHLET_H
#define SOWATCH_WATCHLET_H

#include <QtCore/QObject>
#include <QtDeclarative/QtDeclarative>
#include "sowatch_global.h"

namespace sowatch
{

class Watch;
class WatchServer;
class WatchletsModel;
class Notification;
class NotificationsModel;

class SOWATCH_EXPORT Watchlet : public QObject
{
    Q_OBJECT
	Q_PROPERTY(QString id READ id CONSTANT)
	Q_PROPERTY(bool active READ isActive NOTIFY activeChanged)

public:
	Watchlet();
	Watchlet(Watch *watch, const QString& id);
	~Watchlet();

	const Watch* watch() const;
	Watch* watch();

	QString id() const;
	bool isActive() const;

	// To be called by the WatchServer
	virtual void activate();
	virtual void deactivate();

	// Some properties
	virtual void setWatchletsModel(WatchletsModel *model);
	virtual void setNotificationsModel(NotificationsModel *model);

	virtual bool handlesNotification(Notification* notification) const;
	virtual void openNotification(Notification* notification);

signals:
	void activeChanged();
	void activated();
	void deactivated();

protected:
	const QString _id;
	bool _active;

private:
	Watch* _watch;
};

}

QML_DECLARE_TYPE(sowatch::Watchlet)

#endif // SOWATCH_WATCHLET_H
