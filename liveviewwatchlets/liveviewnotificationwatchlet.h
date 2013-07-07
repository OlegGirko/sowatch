#ifndef LIVEVIEWNOTIFICATIONWATCHLET_H
#define LIVEVIEWNOTIFICATIONWATCHLET_H

#include <sowatch.h>

namespace sowatch
{

class LiveViewNotificationWatchlet : public DeclarativeWatchlet
{
    Q_OBJECT
public:
	explicit LiveViewNotificationWatchlet(Watch* watch);

	static const QLatin1String myId;
};

}

#endif // LIVEVIEWNOTIFICATIONWATCHLET_H
