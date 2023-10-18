// SPDX-FileCopyrightText: 2023 Joshua Goins <josh@redstrate.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "qinvidiousglobal.h"

namespace QInvidious
{

class Channel
{
public:
    FROM_JSON_OVERLOADS(Channel)
    static Channel fromJson(const QJsonObject &, Channel &);

    QString name() const;
    void setName(const QString &name);

    QString banner() const;
    void setBanner(const QString &banner);

    QString avatar() const;
    void setAvatar(const QString &avatar);

    QString description() const;
    void setDescription(const QString &description);

    int subCount() const;
    void setSubCount(int count);

private:
    QString m_name;
    QString m_banner;
    QString m_avatar;
    QString m_description;
    int m_subCount;
};

}
