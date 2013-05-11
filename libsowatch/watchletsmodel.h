#ifndef SOWATCH_WATCHLETSMODEL_H
#define SOWATCH_WATCHLETSMODEL_H

#include <QtCore/QAbstractListModel>

#include "watchlet.h"
#include "watchletplugininterface.h"

namespace sowatch
{

class WatchletsModel : public QAbstractListModel
{
	Q_OBJECT

public:
	explicit WatchletsModel(QObject *parent = 0);

	enum DataRoles {
		ObjectRole = Qt::UserRole,
		TitleRole = Qt::DisplayRole
	};

	int rowCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;

	int size() const;
	Watchlet * at(int position) const;

	void add(Watchlet *w);
	void insert(int position, Watchlet *w);
	void move(const Watchlet *w, int to);
	void move(int position, int to);
	void remove(const Watchlet *w);
	void remove(int position);

signals:
	void modelChanged();

protected:
	typedef WatchletPluginInterface::WatchletInfo WatchletInfo;

	static WatchletInfo getInfoForWatchlet(const Watchlet *w);

private:
	QList<Watchlet*> _list;
	QList<WatchletInfo> _info;

};

}

#endif // WATCHLETSMODEL_H
