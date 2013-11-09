#ifndef QMAFWWATCHLET_H
#define QMAFWWATCHLET_H

#include <sowatch.h>

#include <MafwRegistry.h>

namespace sowatch
{

class QMafwWatchletPlayer;
class QMafwWatchletVolumeControl;

class QMafwWatchlet : public DeclarativeWatchlet
{
    Q_OBJECT
public:
	explicit QMafwWatchlet(Watch* watch);

private:
	MafwRegistry *_registry;
	QMafwWatchletPlayer *_player;
	QMafwWatchletVolumeControl *_volumeControl;

private slots:
	void handleRendererAdded(const QString & uuid);
	void handleRendererRemoved(const QString & uuid);
};

}

#endif // QMAFWWATCHLET_H
