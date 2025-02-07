// SPDX-FileCopyrightText: 2023 Joshua Goins <josh@redstrate.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "peertubeinstancesmodel.h"
#include <QUrlQuery>

#include "controllers/plasmatube.h"

using namespace Qt::Literals::StringLiterals;

PeerTubeInstancesModel::PeerTubeInstancesModel(QObject *parent)
    : QAbstractListModel(parent)
{
    fill();
}

QVariant PeerTubeInstancesModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(checkIndex(index, QAbstractItemModel::CheckIndexOption::IndexIsValid));

    const auto &instance = m_instances[index.row()];

    switch (role) {
    case URLRole:
        return instance.url;
    default:
        return {};
    }
}

bool PeerTubeInstancesModel::loading() const
{
    return m_loading;
}

void PeerTubeInstancesModel::setLoading(bool loading)
{
    if (m_loading == loading) {
        return;
    }
    m_loading = loading;
    Q_EMIT loadingChanged();
}

int PeerTubeInstancesModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_instances.size();
}

QHash<int, QByteArray> PeerTubeInstancesModel::roleNames() const
{
    return {{URLRole, "url"}};
}

void PeerTubeInstancesModel::setFilterString(const QString &filterString)
{
    if (filterString != m_filterString) {
        m_filterString = filterString;
        Q_EMIT filterStringChanged();

        beginResetModel();
        m_instances.clear();
        endResetModel();

        fill();
    }
}

QString PeerTubeInstancesModel::filterString() const
{
    return m_filterString;
}

void PeerTubeInstancesModel::fill()
{
    if (m_loading) {
        return;
    }
    setLoading(true);

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("count"), QString::number(10));
    if (!m_filterString.isEmpty()) {
        query.addQueryItem(QStringLiteral("search"), m_filterString);
    }

    QUrl url(QStringLiteral("https://instances.joinpeertube.org/api/v1/instances"));
    url.setQuery(query);

    auto reply = m_netManager.get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        const auto doc = QJsonDocument::fromJson(reply->readAll());
        auto instances = doc["data"_L1].toArray().toVariantList();

        if (!instances.isEmpty()) {
            QList<PeerTubeInstance> fetchedInstances;

            std::transform(instances.cbegin(), instances.cend(), std::back_inserter(fetchedInstances), [=](const QVariant &value) -> auto {
                return fromSourceData(value.toJsonObject());
            });
            beginInsertRows({}, m_instances.size(), m_instances.size() + fetchedInstances.size() - 1);
            m_instances += fetchedInstances;
            endInsertRows();
        }

        setLoading(false);
    });
}

PeerTubeInstancesModel::PeerTubeInstance PeerTubeInstancesModel::fromSourceData(const QJsonObject &obj) const
{
    PeerTubeInstance instance;
    instance.url = obj["host"_L1].toString();

    return instance;
}

#include "moc_peertubeinstancesmodel.cpp"