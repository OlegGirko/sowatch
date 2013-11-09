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

	void displayIdleScreen();
	void displayNotification(Notification *notification);
	void displayApplication();

	void clear(Mode mode, bool black = false);
	void update(Mode mode, const QList<QRect>& rects = QList<QRect>());

protected:
	void setupBluetoothWatch();
};

}

#endif // METAWATCHDIGITAL_H
