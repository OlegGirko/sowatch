#ifndef TRIPWATCHLET_H
#define TRIPWATCHLET_H

#include <sowatch.h>

namespace sowatch
{

class TripWatchlet : public DeclarativeWatchlet
{
    Q_OBJECT
public:
	explicit TripWatchlet(WatchServer* server);

	static const QLatin1String myId;
};

}

#endif // TRIPWATCHLET_H
