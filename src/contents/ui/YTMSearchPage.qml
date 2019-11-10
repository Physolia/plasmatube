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

import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12 as Controls
import org.kde.kirigami 2.10 as Kirigami

import org.kde.plasmatube.accountmanager 1.0
import org.kde.plasmatube.models 1.0
import org.kde.plasmatube.invidious 1.0
import org.kde.plasmatube.ytmusic 1.0 as YTM
import "utils.js" as Utils

Kirigami.ScrollablePage {
    id: root
    title: qsTr("YouTube Music")
    supportsRefreshing: true
    onRefreshingChanged: {
        if (refreshing)
            searchModel.query = searchField.text;
    }

    Kirigami.Theme.colorSet: Kirigami.Theme.View

    header: Rectangle {
        color: Kirigami.Theme.backgroundColor
        height: searchField.implicitHeight + 2 * Kirigami.Units.largeSpacing
        width: root.width

        Kirigami.SearchField {
            id: searchField
            anchors.centerIn: parent
            anchors.margins: Kirigami.Units.largeSpacing
            width: parent.width - 2 * Kirigami.Units.largeSpacing

            selectByMouse: true
            rightActions: [
                Kirigami.Action {
                    iconName: "search"
                    onTriggered: searchField.accepted()
                }
            ]

            onAccepted: root.refreshing = true
        }
    }

    ListView {
        clip: true
        model: YTM.SearchModel {
            id: searchModel
            onIsLoadingChanged: {
                if (!isLoading)
                    root.refreshing = false;
            }
        }
`
        section.property: "shelfTitle"
        section.delegate: Kirigami.ListSectionHeader {
            text: section
        }

        delegate: Kirigami.BasicListItem {
            reserveSpaceForIcon: false
            reserveSpaceForLabel: false

            RowLayout {
                spacing: Kirigami.Units.smallSpacing

                Image {
                    Layout.preferredWidth: 42
                    Layout.preferredHeight: 42

                    source: thumbnailUrl
                    fillMode: Image.PreserveAspectCrop
                    mipmap: true
                }

                ColumnLayout {
                    Controls.Label {
                        text: model.title
                    }

                    Controls.Label {
                        text: model.attributes.join(" \u2022 ")
                    }
                }
            }
        }
    }
}
