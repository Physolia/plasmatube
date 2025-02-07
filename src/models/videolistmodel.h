// SPDX-FileCopyrightText: 2019 Linus Jahn <lnj@kaidan.im>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "abstractapi.h"
#include "api/searchparameters.h"
#include "api/videobasicinfo.h"
#include <QAbstractListModel>
#include <QFutureSynchronizer>
#include <QtQml/qqmlregistration.h>

class InvidiousManager;
class QNetworkReply;

class VideoListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool isLoading READ isLoading NOTIFY isLoadingChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)

public:
    enum Roles : int {
        IdRole = Qt::UserRole + 1,
        TitleRole,
        ThumbnailRole,
        LengthRole,
        ViewCountRole,
        AuthorRole,
        AuthorIdRole,
        AuthorUrlRole,
        PublishedRole,
        PublishedTextRole,
        DescriptionRole,
        DescriptionHtmlRole,
        LiveNowRole,
        PaidRole,
        PremiumRole,
        WatchedRole
    };

    enum QueryType : quint8 {
        NoQuery,
        Feed,
        Search,
        Top,
        Trending,
        TrendingGaming,
        TrendingMovies,
        TrendingMusic,
        TrendingNews,
        RecommendedVideos,
        Channel,
        History,
        Playlist
    };
    Q_ENUM(QueryType)

    Q_INVOKABLE static QString queryTypeString(QueryType);
    Q_INVOKABLE static QString queryTypeIcon(QueryType);

    VideoListModel(QObject *parent = nullptr);
    VideoListModel(const QList<QInvidious::VideoBasicInfo> &, QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE void fetchMore(const QModelIndex &parent) override;
    Q_INVOKABLE bool canFetchMore(const QModelIndex &parent) const override;

    Q_INVOKABLE void requestSearchResults(const SearchParameters *searchParameters);
    Q_INVOKABLE void requestChannel(const QString &ucid);
    Q_INVOKABLE void requestQuery(QueryType type);
    Q_INVOKABLE void requestPlaylist(const QString &id);
    Q_INVOKABLE void refresh();

    bool isLoading() const;

    QString title() const;

    Q_INVOKABLE void markAsWatched(int index);
    Q_INVOKABLE void markAsUnwatched(int index);

    Q_INVOKABLE void removeFromPlaylist(const QString &plid, int index);

Q_SIGNALS:
    void isLoadingChanged();
    void titleChanged();
    void errorOccured(const QString &error);

private:
    void handleQuery(QFuture<QInvidious::VideoListResult> future, QueryType, bool reset = true);

    void setQueryType(QueryType);
    void clearAll();

    bool m_constant = false;

    QueryType m_queryType = NoQuery;
    qint32 m_currentPage = 1;
    SearchParameters m_searchParameters;
    QString m_channel;
    QFutureWatcher<QInvidious::VideoListResult> *m_futureWatcher = nullptr;

    // history
    QFutureWatcher<QInvidious::HistoryResult> *m_historyPageWatcher = nullptr;
    QFutureSynchronizer<QInvidious::VideoResult> m_historyFutureSync;
    QFutureWatcher<void> *m_historyFetchFinishWatcher = nullptr;

    // playlist
    QString m_playlist;

    QList<QInvidious::VideoBasicInfo> m_results;

    void requestHistory();
    void processHistoryResult(const QList<QString> &result);
};
