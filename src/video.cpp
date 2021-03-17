/*
 * Copyright 2019  Linus Jahn <lnj@kaidan.im>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "video.h"
#include <QJsonArray>

void Video::parseFromJson(const QJsonObject &obj)
{
    VideoBasicInfo::parseFromJson(obj);

    auto jsonValueToString = [](const QJsonValue &val) {
        return val.toString();
    };

    const auto keywords = obj.value("keywords").toArray();
    std::transform(keywords.cbegin(), keywords.cend(), std::back_inserter(m_keywords), jsonValueToString);

    setLikeCount(obj.value("likeCount").toInt());
    setDislikeCount(obj.value("dislikeCount").toInt());
    setIsFamilyFriendly(obj.value("isFamilyFriendly").toBool(true));

    const auto allowedRegions = obj.value("allowedRegions").toArray();
    std::transform(allowedRegions.cbegin(), allowedRegions.cend(), std::back_inserter(m_allowedRegions), jsonValueToString);

    setGenre(obj.value("genre").toString());
    setGenreUrl(obj.value("genreUrl").toString());

    const auto authorThumbnails = obj.value("authorThumbnails").toArray();
    std::transform(authorThumbnails.cbegin(), authorThumbnails.cend(), std::back_inserter(m_authorThumbnails),
                   [](const QJsonValue &val) {
        VideoThumbnail thumb;
        thumb.parseFromJson(val.toObject());
        return thumb;
    });

    setSubCountText(obj.value("subCountText").toString());
    setAllowRatings(obj.value("allowRatings").toBool(true));
    setRating(obj.value("rating").toDouble(5.0));
    setIsListed(obj.value("isListed").toBool(true));

    const auto recommendedVideos = obj.value("recommendedVideos").toArray();
    std::transform(recommendedVideos.cbegin(), recommendedVideos.cend(), std::back_inserter(m_recommendedVideos),
                   [](const QJsonValue &val) {
        VideoBasicInfo vid;
        vid.parseFromJson(val.toObject());
        return vid;
    });
}

QStringList Video::keywords() const
{
    return m_keywords;
}

void Video::setKeywords(const QStringList &keywords)
{
    m_keywords = keywords;
}

qint32 Video::likeCount() const
{
    return m_likeCount;
}

void Video::setLikeCount(qint32 likeCount)
{
    m_likeCount = likeCount;
}

qint32 Video::dislikeCount() const
{
    return m_dislikeCount;
}

void Video::setDislikeCount(qint32 dislikeCount)
{
    m_dislikeCount = dislikeCount;
}

bool Video::isFamilyFriendly() const
{
    return m_isFamilyFriendly;
}

void Video::setIsFamilyFriendly(bool isFamilyFriendly)
{
    m_isFamilyFriendly = isFamilyFriendly;
}

QStringList Video::allowedRegions() const
{
    return m_allowedRegions;
}

void Video::setAllowedRegions(const QStringList &allowedRegions)
{
    m_allowedRegions = allowedRegions;
}

QString Video::genre() const
{
    return m_genre;
}

void Video::setGenre(const QString &genre)
{
    m_genre = genre;
}

QString Video::genreUrl() const
{
    return m_genreUrl;
}

void Video::setGenreUrl(const QString &genreUrl)
{
    m_genreUrl = genreUrl;
}

QList<VideoThumbnail> Video::authorThumbnails() const
{
    return m_authorThumbnails;
}

void Video::setAuthorThumbnails(const QList<VideoThumbnail> &authorThumbnails)
{
    m_authorThumbnails = authorThumbnails;
}

QString Video::subCountText() const
{
    return m_subCountText;
}

void Video::setSubCountText(const QString &subCountText)
{
    m_subCountText = subCountText;
}

bool Video::allowRatings() const
{
    return m_allowRatings;
}

void Video::setAllowRatings(bool allowRatings)
{
    m_allowRatings = allowRatings;
}

double Video::rating() const
{
    return m_rating;
}

void Video::setRating(double rating)
{
    m_rating = rating;
}

bool Video::isListed() const
{
    return m_isListed;
}

void Video::setIsListed(bool isListed)
{
    m_isListed = isListed;
}

QList<VideoBasicInfo> Video::recommendedVideos() const
{
    return m_recommendedVideos;
}

void Video::setRecommendedVideos(const QList<VideoBasicInfo> &recommendedVideos)
{
    m_recommendedVideos = recommendedVideos;
}
