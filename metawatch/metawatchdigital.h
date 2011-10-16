#ifndef METAWATCHDIGITAL_H
#define METAWATCHDIGITAL_H

#include "metawatch.h"

namespace sowatch
{

class MetaWatchDigital : public MetaWatch
{
    Q_OBJECT
public:
	explicit MetaWatchDigital(const QBluetoothAddress& address, QSettings* settings = 0, QObject *parent = 0);

	static const int screenWidth = 96;
	static const int screenHeight = 96;
	static const int systemAreaHeight = 30;

	int metric(PaintDeviceMetric metric) const;

	QString model() const;

	void updateNotificationCount(Notification::Type type, int count);
	void updateWeather(WeatherNotification *weather);

	void displayIdleScreen();
	void displayNotification(Notification *notification);
	void displayApplication();

	void clear(Mode mode, bool black = false);
	void update(Mode mode, const QList<QRect>& rects = QList<QRect>());

protected:
	// Notifications: Unread count
	uint _nMails, _nCalls, _nIms, _nSms, _nMms;

	void handleWatchConnected();

	void renderIdleScreen();
	void renderIdleWeather(WeatherNotification *w = 0);
	QImage iconForWeather(WeatherNotification *w);
	void renderIdleCounts();

	void renderNotification(Notification *n);
	QImage iconForNotification(const Notification *n);
};

}

#endif // METAWATCHDIGITAL_H
