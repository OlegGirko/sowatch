#ifndef QMAFWWATCHLETVOLUMECONTROL_H
#define QMAFWWATCHLETVOLUMECONTROL_H

#include <QtCore/QObject>
#include <dbus/dbus.h>

class MainVolumeControlProxy;

namespace sowatch
{

class QMafwWatchletVolumeControl : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
	Q_PROPERTY(int min READ min NOTIFY minChanged)
	Q_PROPERTY(int max READ max NOTIFY maxChanged)

public:
	explicit QMafwWatchletVolumeControl(QObject *parent = 0);
	~QMafwWatchletVolumeControl();

	int volume() const;
	int min() const;
	int max() const;

	void setVolume(int vol);
	
signals:
	void volumeChanged();
	void minChanged();
	void maxChanged();
	
public slots:
	void up();
	void down();

private:
	void _openConnection();
	void _listenForSignal();
	void _fetchValues();
	static void handleFetchReply(DBusPendingCall *pending, void *user_data);
	static DBusHandlerResult handleDBusSignal(DBusConnection *connection, DBusMessage *message, void *user_data);

private:
	DBusConnection *_conn;
	uint _curStep;
	uint _maxStep;
};

}

#endif // QMAFWWATCHLETVOLUMECONTROL_H
