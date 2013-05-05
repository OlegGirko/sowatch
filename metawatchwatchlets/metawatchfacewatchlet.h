#ifndef METAWATCHFACEWATCHLET_H
#define METAWATCHFACEWATCHLET_H

#include <sowatch.h>

namespace sowatch
{

class MetaWatchFaceWatchlet : public DeclarativeWatchlet
{
    Q_OBJECT
public:
	explicit MetaWatchFaceWatchlet(Watch* watch);

	static const QLatin1String myId;
};

}

#endif // METAWATCHFACEWATCHLET_H
