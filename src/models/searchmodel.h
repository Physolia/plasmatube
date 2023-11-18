// SPDX-FileCopyrightText: 2023 Joshua Goins <josh@redstrate.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "abstractapi.h"
#include "api/searchparameters.h"
#include "api/videobasicinfo.h"
#include <QAbstractListModel>
#include <QFutureSynchronizer>
#include <QtQml/qqmlregistration.h>

#include "abstractlistmodel.h"

class InvidiousManager;
class QNetworkReply;

class SearchModel : public AbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit SearchModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void fetchMore(const QModelIndex &parent) override;
    bool canFetchMore(const QModelIndex &parent) const override;

    bool isLoading() const;

public Q_SLOTS:
    void request(const SearchParameters *searchParameters);
    void refresh() override;
    void markAsWatched(int index) override;
    void markAsUnwatched(int index) override;
    void removeFromPlaylist(const QString &plid, int index) override;

private:
    void performSearch();

    qint32 m_currentPage = 1;
    SearchParameters m_searchParameters;
    QFutureWatcher<QInvidious::SearchListResult> *m_futureWatcher = nullptr;

    QList<QInvidious::SearchResult> m_results;
};
