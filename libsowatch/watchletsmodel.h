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
	Q_PROPERTY(QString watchModel READ watchModel WRITE setWatchModel NOTIFY watchModelChanged)

public:
	explicit WatchletsModel(QObject *parent = 0);

	enum DataRoles {
		ObjectRole = Qt::UserRole,
		TitleRole = Qt::DisplayRole,
		IconRole = Qt::DecorationRole
	};

	QString watchModel() const;
	void setWatchModel(const QString& s);

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

    QHash<int, QByteArray> getRoleNames() { return _roles; }
signals:
	void watchModelChanged();
	void modelChanged();

protected:
	typedef WatchletPluginInterface::WatchletInfo WatchletInfo;

	WatchletInfo getInfoForWatchlet(const Watchlet *w);

private:
	QString _watchModel;
	QList<Watchlet*> _list;
	QList<WatchletInfo> _info;
    QHash<int, QByteArray> _roles;

};

}

#endif // WATCHLETSMODEL_H
