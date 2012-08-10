#ifndef QMSGWATCHLET_H
#define QMSGWATCHLET_H

#include <QtMessaging/QMessageService>
#include <sowatch.h>

namespace sowatch
{

class QMsgWatchlet : public DeclarativeWatchlet
{
    Q_OBJECT
public:
	explicit QMsgWatchlet(WatchServer* server);

private:
	QTM_PREPEND_NAMESPACE(QMessageService) _qms;
};

}

#endif // QMSGWATCHLET_H
