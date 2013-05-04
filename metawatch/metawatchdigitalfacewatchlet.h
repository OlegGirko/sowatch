#ifndef METAWATCHDIGITALFACEWATCHLET_H
#define METAWATCHDIGITALFACEWATCHLET_H

#include <sowatch.h>

namespace sowatch
{

class MetaWatchDigitalFaceWatchlet : public DeclarativeWatchlet
{
    Q_OBJECT
public:
	explicit MetaWatchDigitalFaceWatchlet(Watch* watch);
};

}

#endif // METAWATCHDIGITALFACEWATCHLET_H
