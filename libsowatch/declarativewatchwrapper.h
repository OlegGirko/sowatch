#ifndef SOWATCH_DECLARATIVEWATCHWRAPPER_H
#define SOWATCH_DECLARATIVEWATCHWRAPPER_H

#include <QtDeclarative/QtDeclarative>
#include "sowatch_global.h"

namespace sowatch
{

class WatchServer;
class Watch;
class DeclarativeWatchlet;
class Notification;

class SOWATCH_EXPORT DeclarativeWatchWrapper : public QObject
{
    Q_OBJECT
	Q_PROPERTY(QString model READ model CONSTANT)
	Q_PROPERTY(bool active READ active NOTIFY activeChanged)

public:
	explicit DeclarativeWatchWrapper(WatchServer *server, Watch *watch, QObject *parent = 0);

	QString model() const;
	bool active() const;

public slots:
	void vibrate(int msecs);

signals:
	void buttonPressed(int button);
	void buttonReleased(int button);

	void activeChanged();
	void notificationsChanged();

private:
	WatchServer *_server;
	Watch* _watch;
	bool _active;

	void activate();
	void deactivate();

friend class DeclarativeWatchlet;
};

}

QML_DECLARE_TYPE(sowatch::DeclarativeWatchWrapper)

#endif // SOWATCH_DECLARATIVEWATCHWRAPPER_H
