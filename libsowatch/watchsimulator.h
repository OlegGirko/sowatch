#ifndef SOWATCH_WATCHSIMULATOR_H
#define SOWATCH_WATCHSIMULATOR_H

#include <QtGui/QImage>

#include "watch.h"

namespace sowatch
{

class WatchSimulator : public Watch
{
    Q_OBJECT
public:
	explicit WatchSimulator(QObject *parent = 0);
};

}

#endif // SOWATCH_WATCHSIMULATOR_H
