#ifndef QORGWATCHLET_H
#define QORGWATCHLET_H

#include <sowatch.h>

namespace sowatch
{

class QOrgWatchlet : public DeclarativeWatchlet
{
    Q_OBJECT
public:
	explicit QOrgWatchlet(Watch* watch);
};

}

#endif // QORGWATCHLET_H
