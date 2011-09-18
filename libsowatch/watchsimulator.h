#ifndef WATCHSIMULATOR_H
#define WATCHSIMULATOR_H

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

#endif // WATCHSIMULATOR_H
