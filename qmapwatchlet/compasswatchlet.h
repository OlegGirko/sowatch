#ifndef COMPASSWATCHLET_H
#define COMPASSWATCHLET_H

#include <sowatch.h>

namespace sowatch
{

class CompassWatchlet : public DeclarativeWatchlet
{
    Q_OBJECT
public:
	explicit CompassWatchlet(Watch* watch);

	static const QLatin1String myId;
};

}

#endif // COMPASSWATCHLET_H
