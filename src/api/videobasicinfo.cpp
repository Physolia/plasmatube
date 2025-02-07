// SPDX-FileCopyrightText: 2019 Linus Jahn <lnj@kaidan.im>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "videobasicinfo.h"

using namespace QInvidious;
using namespace Qt::StringLiterals;

VideoBasicInfo VideoBasicInfo::fromJson(const QJsonObject &obj, VideoBasicInfo &info)
{
    const bool isPeerTube = obj.contains("id"_L1);
    const bool isInvidious = obj.contains("videoId"_L1);
    if (isPeerTube) {
        info.setVideoId(obj.value("uuid"_L1).toString());
        info.setTitle(obj.value("name"_L1).toString());
        info.setLength(QTime(0, 0).addSecs(obj.value("duration"_L1).toInt()));
        info.setViewCount(obj.value("views"_L1).toInt());

        const auto channel = obj.value("channel"_L1).toObject();

        info.setAuthor(channel.value("displayName"_L1).toString());
        info.setAuthorId(channel.value("name"_L1).toString());
        info.setAuthorUrl(channel.value("url"_L1).toString());
        // FIXME: 2038 problem (timestamp is only 32 bit long)
        info.setPublished(QDateTime::fromSecsSinceEpoch(obj.value("publishedAt"_L1).toInt()));
        info.setPublishedText(info.published().toString());
        info.setDescription(obj.value("description"_L1).toString());
        info.setDescriptionHtml(obj.value("description"_L1).toString());
        info.setLiveNow(obj.value("isLive"_L1).toBool(false));
        info.setPaid(false);
        info.setPremium(false);
        info.setUpcoming(false);

        VideoThumbnail thumbnail;
        thumbnail.setUrl(QUrl::fromUserInput(obj.value("thumbnailPath"_L1).toString()));
        info.m_videoThumbnails.push_back(thumbnail);
    } else if (isInvidious) {
        info.setVideoId(obj.value("videoId"_L1).toString());
        info.setTitle(obj.value("title"_L1).toString());
        info.setLength(QTime(0, 0).addSecs(obj.value("lengthSeconds"_L1).toInt()));
        info.setViewCount(obj.value("viewCount"_L1).toInt());
        info.setAuthor(obj.value("author"_L1).toString());
        info.setAuthorId(obj.value("authorId"_L1).toString());
        info.setAuthorUrl(obj.value("authorUrl"_L1).toString());
        // FIXME: 2038 problem (timestamp is only 32 bit long)
        info.setPublished(QDateTime::fromSecsSinceEpoch(obj.value("published"_L1).toInt()));
        info.setPublishedText(obj.value("publishedText"_L1).toString());
        info.setDescription(obj.value("description"_L1).toString());
        info.setDescriptionHtml(obj.value("descriptionHtml"_L1).toString());
        info.setLiveNow(obj.value("liveNow"_L1).toBool(false));
        info.setPaid(obj.value("paid"_L1).toBool(false));
        info.setPremium(obj.value("premium"_L1).toBool(false));
        info.setUpcoming(obj.value("isUpcoming"_L1).toBool(false));
        parseArray(obj.value("videoThumbnails"_L1), info.m_videoThumbnails);
        if (obj.contains("indexId"_L1)) {
            info.setIndexId(obj.value("indexId"_L1).toString());
        }
    } else {
        info.setVideoId(obj.value("url"_L1).toString().remove(QStringLiteral("/watch?v=")));
        info.setTitle(obj.value("title"_L1).toString());
        info.setLength(QTime(0, 0).addSecs(obj.value("duration"_L1).toInt()));
        info.setViewCount(obj.value("views"_L1).toInt());
        info.setAuthor(obj.value("uploaderName"_L1).toString());
        info.setAuthorUrl(obj.value("uploaderUrl"_L1).toString());
        info.setAuthorId(obj.value("uploaderUrl"_L1).toString().remove(QStringLiteral("/channel/")));
        // FIXME: 2038 problem (timestamp is only 32 bit long)
        info.setPublished(QDateTime::fromSecsSinceEpoch(obj.value("uploaded"_L1).toInt()));
        info.setPublishedText(obj.value("uploadedDate"_L1).toString());
        if (obj.contains("description"_L1)) {
            info.setDescription(obj.value("description"_L1).toString());
            info.setDescriptionHtml(obj.value("description"_L1).toString());
        } else {
            info.setDescription(obj.value("shortDescription"_L1).toString());
            info.setDescriptionHtml(obj.value("shortDescription"_L1).toString());
        }
        info.setLiveNow(false);
        info.setPaid(false);
        info.setPremium(false);
        info.setUpcoming(false);

        VideoThumbnail thumbnail;
        thumbnail.setUrl(QUrl::fromUserInput(obj.value("thumbnail"_L1).toString()));
        info.m_videoThumbnails.push_back(thumbnail);
    }
    return info;
}

VideoBasicInfo::VideoBasicInfo()
    : m_viewCount(0)
    , m_isNotification(false)
    , m_liveNow(false)
    , m_paid(false)
    , m_premium(false)
    , m_upcoming(false)
{
}

bool VideoBasicInfo::isNotification() const
{
    return m_isNotification;
}

void VideoBasicInfo::setIsNotification(bool isNotification)
{
    m_isNotification = isNotification;
}

QString VideoBasicInfo::videoId() const
{
    return m_videoId;
}

void VideoBasicInfo::setVideoId(const QString &videoId)
{
    m_videoId = videoId;
}

QString VideoBasicInfo::title() const
{
    return m_title;
}

void VideoBasicInfo::setTitle(const QString &title)
{
    m_title = title;
}

QList<VideoThumbnail> VideoBasicInfo::videoThumbnails() const
{
    return m_videoThumbnails;
}

void VideoBasicInfo::setVideoThumbnails(const QList<VideoThumbnail> &videoThumbnails)
{
    m_videoThumbnails = videoThumbnails;
}

VideoThumbnail VideoBasicInfo::thumbnail(const QString &quality) const
{
    for (auto &thumb : m_videoThumbnails) {
        if (thumb.quality() == quality)
            return thumb;
    }
    // didn't found requested quality
    if (!m_videoThumbnails.isEmpty())
        return m_videoThumbnails.last();
    return VideoThumbnail();
}

QTime VideoBasicInfo::length() const
{
    return m_length;
}

void VideoBasicInfo::setLength(const QTime &length)
{
    m_length = length;
}

qint64 VideoBasicInfo::viewCount() const
{
    return m_viewCount;
}

void VideoBasicInfo::setViewCount(qint64 viewCount)
{
    m_viewCount = viewCount;
}

QString VideoBasicInfo::author() const
{
    return m_author;
}

void VideoBasicInfo::setAuthor(const QString &author)
{
    m_author = author;
}

QString VideoBasicInfo::authorId() const
{
    return m_authorId;
}

void VideoBasicInfo::setAuthorId(const QString &authorId)
{
    m_authorId = authorId;
}

QString VideoBasicInfo::authorUrl() const
{
    return m_authorUrl;
}

void VideoBasicInfo::setAuthorUrl(const QString &authorUrl)
{
    m_authorUrl = authorUrl;
}

QDateTime VideoBasicInfo::published() const
{
    return m_published;
}

void VideoBasicInfo::setPublished(const QDateTime &published)
{
    m_published = published;
}

QString VideoBasicInfo::publishedText() const
{
    return m_publishedText;
}

void VideoBasicInfo::setPublishedText(const QString &publishedText)
{
    m_publishedText = publishedText;
}

QString VideoBasicInfo::description() const
{
    return m_description;
}

void VideoBasicInfo::setDescription(const QString &description)
{
    m_description = description;
}

QString VideoBasicInfo::descriptionHtml() const
{
    return m_descriptionHtml;
}

void VideoBasicInfo::setDescriptionHtml(const QString &descriptionHtml)
{
    m_descriptionHtml = descriptionHtml;
}

bool VideoBasicInfo::liveNow() const
{
    return m_liveNow;
}

void VideoBasicInfo::setLiveNow(bool liveNow)
{
    m_liveNow = liveNow;
}

bool VideoBasicInfo::paid() const
{
    return m_paid;
}

void VideoBasicInfo::setPaid(bool paid)
{
    m_paid = paid;
}

bool VideoBasicInfo::premium() const
{
    return m_premium;
}

void VideoBasicInfo::setPremium(bool premium)
{
    m_premium = premium;
}

bool VideoBasicInfo::upcoming() const
{
    return m_upcoming;
}

void VideoBasicInfo::setUpcoming(bool upcoming)
{
    m_upcoming = upcoming;
}

QString VideoBasicInfo::indexId() const
{
    return m_indexId;
}

void VideoBasicInfo::setIndexId(const QString &id)
{
    m_indexId = id;
}
