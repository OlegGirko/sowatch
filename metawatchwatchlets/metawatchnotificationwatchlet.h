#ifndef METAWATCHNOTIFICATIONWATCHLET_H
#define METAWATCHNOTIFICATIONWATCHLET_H

#include <sowatch.h>

namespace sowatch
{

class MetaWatchNotificationWatchlet : public DeclarativeWatchlet
{
    Q_OBJECT
public:
	explicit MetaWatchNotificationWatchlet(Watch* watch);

	static const QLatin1String myId;
};

}

#endif // METAWATCHNOTIFICATIONWATCHLET_H
