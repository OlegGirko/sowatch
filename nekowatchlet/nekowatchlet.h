#ifndef NEKOWATCHLET_H
#define NEKOWATCHLET_H

#include <sowatch.h>

namespace sowatch
{

class NekoWatchlet : public DeclarativeWatchlet
{
    Q_OBJECT

public:
	explicit NekoWatchlet(Watch* watch);

	static const QLatin1String myId;
};

}

#endif // NEKOWATCHLET_H
