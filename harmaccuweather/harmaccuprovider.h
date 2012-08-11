#ifndef HARMACCUPROVIDER_H
#define HARMACCUPROVIDER_H

#include <sowatch.h>

namespace sowatch
{

class HarmAccuWeather;

class HarmAccuProvider : public NotificationProvider
{
    Q_OBJECT

public:
	explicit HarmAccuProvider(QObject *parent = 0);
	~HarmAccuProvider();

public slots:
	void generateNotification();

private:

};

}

#endif // HARMACCUPROVIDER_H
