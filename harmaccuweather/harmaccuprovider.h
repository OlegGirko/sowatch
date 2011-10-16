#ifndef CKITCALLPROVIDER_H
#define CKITCALLPROVIDER_H

#include <QtCore/QTimer>
#include <sowatch.h>

namespace sowatch
{

class HarmAccuWeather;

class HarmAccuProvider : public NotificationProvider
{
    Q_OBJECT

public:
	explicit HarmAccuProvider(int updateTime, QObject *parent = 0);
	~HarmAccuProvider();

public slots:
	void generateNotification();

private:
	int _updateTime;
};

}

#endif // CKITCALLPROVIDER_H
