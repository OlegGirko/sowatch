#ifndef METAWATCHANALOG_H
#define METAWATCHANALOG_H

#include "metawatch.h"

namespace sowatch
{

class MetaWatchAnalog : public MetaWatch
{
    Q_OBJECT
public:
	explicit MetaWatchAnalog(const QBluetoothAddress& address, QSettings* settings = 0, QObject *parent = 0);

	static const int screenWidth = 80;
	static const int screenHeight = 16*2;

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
	void handleWatchConnected();
};

}

#endif // METAWATCHANALOG_H
