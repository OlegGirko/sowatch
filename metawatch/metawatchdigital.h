#ifndef METAWATCHDIGITAL_H
#define METAWATCHDIGITAL_H

#include "metawatch.h"

namespace sowatch
{

class MetaWatchDigital : public MetaWatch
{
    Q_OBJECT
public:
	explicit MetaWatchDigital(ConfigKey *settings, QObject *parent = 0);

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
	// Idle screen: notifications unread count
	ushort _nMails, _nCalls, _nIms, _nSms, _nMms;
	// Idle screen: weather information
	WeatherNotification::WeatherType _wForecast;
	QString _wBody;
	short _wTemperature;
	bool _wMetric;

	void handleWatchConnected();

	void renderIdleScreen();
	void renderIdleWeather();
	QImage iconForWeather(WeatherNotification::WeatherType w);
	void renderIdleCounts();

	void renderNotification(Notification *n);
	QImage iconForNotification(const Notification *n);
};

}

#endif // METAWATCHDIGITAL_H
