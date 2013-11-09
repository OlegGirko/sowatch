#include <QtCore/QDebug>

#include "qmsgwatchlet.h"

QTM_USE_NAMESPACE
using namespace sowatch;

QMsgWatchlet::QMsgWatchlet(Watch* watch) :
	DeclarativeWatchlet(watch, "com.javispedro.sowatch.qmsg"),
    _qms(new QMessageService(this))
{
	setSource(QUrl(SOWATCH_QML_DIR "/qmsgwatchlet/" + watch->model() + ".qml"));
}
