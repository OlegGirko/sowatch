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
};

}

#endif // METAWATCHFACEWATCHLET_H
