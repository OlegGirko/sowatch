#ifndef LIVEVIEW_H
#define LIVEVIEW_H

#include <sowatch.h>
#include <sowatchbt.h>

namespace sowatch
{

class LiveView : public BluetoothWatch
{
	Q_OBJECT

public:
	explicit LiveView(ConfigKey *settings, QObject *parent = 0);
	~LiveView();

	QPaintEngine* paintEngine() const;
	int metric(PaintDeviceMetric metric) const;

	QString model() const;
	QStringList buttons() const;
	bool isConnected() const;
	bool busy() const;

	void setDateTime(const QDateTime& dateTime);
	void queryDateTime();
	QDateTime dateTime() const;

	void queryBatteryLevel();
	int batteryLevel() const;

	void queryCharging();
	bool charging() const;

	void displayIdleScreen();
	void displayNotification(Notification *notification);
	void displayApplication();

	void vibrate(int msecs);

protected:
	void setupBluetoothWatch();
	void desetupBluetoothWatch();

private:
	ConfigKey *_settings;
};

}

#endif // LIVEVIEW_H
