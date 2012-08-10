#include "qmsgwatchlet.h"

QTM_USE_NAMESPACE
using namespace sowatch;

QMsgWatchlet::QMsgWatchlet(WatchServer* server) :
	DeclarativeWatchlet(server, "com.javispedro.sowatch.qmsg"),
    _qms(new QMessageService(this))
{
	setSource(QUrl(SOWATCH_QML_DIR "/qmsgwatchlet/" + server->watch()->model() + ".qml"));
}
