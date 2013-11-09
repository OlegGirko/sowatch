#ifndef NOTIFICATIONSWATCHLET_H
#define NOTIFICATIONSWATCHLET_H

#include <sowatch.h>

namespace sowatch
{

class NotificationsWatchlet : public DeclarativeWatchlet
{
    Q_OBJECT
public:
	explicit NotificationsWatchlet(Watch* watch);
};

}

#endif // NOTIFICATIONSWATCHLET_H
