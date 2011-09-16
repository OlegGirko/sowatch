#ifndef METAWATCHSIMULATOR_H
#define METAWATCHSIMULATOR_H

#include "watchsimulator.h"
#include "metawatchsimulatorform.h"

namespace sowatch {

class MetaWatchSimulator : public WatchSimulator
{
    Q_OBJECT
public:
    explicit MetaWatchSimulator(QObject *parent = 0);
	~MetaWatchSimulator();

	QString model() const;
	bool isConnected() const;
	bool busy() const;

	void update(const QList<QRect> &rects);
	void vibrate(bool on);

protected:
	QPixmap _screen;
	MetaWatchSimulatorForm* _form;
};

}

#endif // METAWATCHSIMULATOR_H
