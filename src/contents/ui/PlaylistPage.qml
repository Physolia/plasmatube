// SPDX-FileCopyrightText: 2023 Joshua Goins <josh@redstrate.com
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4 as Controls
import org.kde.kirigami 2.19 as Kirigami

import org.kde.plasmatube 1.0
import org.kde.plasmatube.models 1.0
import "utils.js" as Utils

Kirigami.ScrollablePage {
    required property string playlistId
    property string currentVideoId
    property int currentVideoIndex

    id: root
    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0

    supportsRefreshing: true
    onRefreshingChanged: {
        if (refreshing && !videoModel.isLoading) {
            videoModel.refresh();
        }
    }

    Kirigami.Theme.colorSet: Kirigami.Theme.View

    GridView {
        id: gridView
        topMargin: root.width > 900 ? Kirigami.Units.gridUnit * 2 : Kirigami.Units.largeSpacing
        bottomMargin: root.width > 900 ? Kirigami.Units.gridUnit * 2 : Kirigami.Units.largeSpacing
        leftMargin: root.width > 900 ? Kirigami.Units.gridUnit * 4 : Kirigami.Units.largeSpacing
        rightMargin: root.width > 900 ? Kirigami.Units.gridUnit * 4 : Kirigami.Units.largeSpacing

        readonly property real effectiveWidth: width - leftMargin - rightMargin
        readonly property real targetDelegateWidth: Kirigami.Units.gridUnit * 14 + Kirigami.Units.largeSpacing * 2
        readonly property int columns: Math.floor(effectiveWidth / targetDelegateWidth)

        cellWidth: effectiveWidth / columns
        cellHeight: (cellWidth / 16 * 9) + Kirigami.Units.gridUnit * 4

        currentIndex: -1
        model: VideoListModel {
            id: videoModel
            onIsLoadingChanged: {
                root.refreshing = isLoading
            }
            onErrorOccured: (errorText) => {
                applicationWindow().showPassiveNotification(errorText)
            }
        }
        delegate: VideoGridItem {
            width: gridView.cellWidth
            height: gridView.cellHeight

            vid: model.id
            thumbnail: model.thumbnail
            liveNow: model.liveNow
            length: model.length
            title: model.title
            author: model.author
            authorId: model.authorId
            description: model.description
            viewCount: model.viewCount
            publishedText: model.publishedText
            watched: model.watched

            onClicked: (mouse) => {
                if (mouse.button === Qt.LeftButton) {
                    applicationWindow().switchVideo(vid);
                    applicationWindow().openPlayer();
                } else {
                    currentVideoId = vid;
                    currentVideoIndex = index;
                    videoMenu.isWatched = PlasmaTube.isVideoWatched(vid);
                    videoMenu.popup();
                }
            }
        }

        Kirigami.PlaceholderMessage {
            anchors.centerIn: parent
            visible: gridView.count === 0 && !root.refreshing
            text: i18nc("@info:status", "Loading…")
        }
    }

    Controls.Menu {
        id: videoMenu

        modal: true

        property bool isWatched

        Controls.MenuItem {
            text: videoMenu.isWatched ? i18n("Mark as unwatched") : i18n("Mark as watched")
            icon.name: videoMenu.isWatched ? "view-hidden" : "view-visible"
            onTriggered: {
                if (videoMenu.isWatched) {
                    videoModel.markAsUnwatched(currentVideoIndex);
                } else {
                    videoModel.markAsWatched(currentVideoIndex);
                }
            }
        }

        Controls.MenuItem {
            text: i18n("Remove from playlist")
            icon.name: "media-playlist-append"
            onTriggered: videoModel.removeFromPlaylist(root.playlistId, root.currentVideoIndex)
        }
    }

    Component.onCompleted: videoModel.requestPlaylist(playlistId)
}