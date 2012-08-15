#ifndef MEECASTPROVIDER_H
#define MEECASTPROVIDER_H

#include <sowatch.h>

namespace sowatch
{

class MeeCastWeather;

class MeeCastProvider : public NotificationProvider
{
    Q_OBJECT

public:
	explicit MeeCastProvider(QObject *parent = 0);
	~MeeCastProvider();

	static const QLatin1String myId;

public slots:
	void generateNotification();

private:

};

}

#endif // HARMACCUPROVIDER_H
