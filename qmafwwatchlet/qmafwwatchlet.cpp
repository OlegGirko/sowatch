#include <MafwShared.h>

#include "qmafwwatchletplayer.h"
#include "qmafwwatchletvolumecontrol.h"
#include "qmafwwatchlet.h"

using namespace sowatch;

QMafwWatchlet::QMafwWatchlet(Watch* watch) :
	DeclarativeWatchlet(server, "com.javispedro.sowatch.qmafw"),
	_registry(MafwRegistry::instance()),
	_player(new QMafwWatchletPlayer(this)),
    _volumeControl(new QMafwWatchletVolumeControl(this))
{
	MafwShared* shared = MafwShared::instance();

	connect(_registry, SIGNAL(rendererAdded(QString, QString)), SLOT(handleRendererAdded(QString)));
	connect(_registry, SIGNAL(rendererRemoved(QString, QString)), SLOT(handleRendererRemoved(QString)));

	shared->initTracking(_registry);

	connect(this, SIGNAL(activated()), _player, SLOT(activate()));
	connect(this, SIGNAL(deactivated()), _player, SLOT(deactivate()));

	context()->setContextProperty("player", _player);
	context()->setContextProperty("volumeControl", _volumeControl);
	setSource(QUrl(SOWATCH_QML_DIR "/qmafwwatchlet/" + server->watch()->model() + ".qml"));
}

void QMafwWatchlet::handleRendererAdded(const QString &uuid)
{
	if (uuid == "mafw_gst_renderer") {
		_player->setRenderer(_registry->renderer(uuid));
	}
}

void QMafwWatchlet::handleRendererRemoved(const QString &uuid)
{
	if (uuid == "mafw_gst_renderer") {
		_player->setRenderer(0);
	}
}
