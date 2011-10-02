#ifndef SOWATCH_WATCHSIMULATOR_H
#define SOWATCH_WATCHSIMULATOR_H

#include "watch.h"
#include "sowatch_global.h"

namespace sowatch
{

class SOWATCH_EXPORT WatchSimulator : public Watch
{
    Q_OBJECT
public:
	explicit WatchSimulator(QObject *parent = 0);
};

}

#endif // SOWATCH_WATCHSIMULATOR_H
