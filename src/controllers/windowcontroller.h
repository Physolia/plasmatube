// SPDX-FileCopyrightText: 2023 Joshua Goins <josh@redstrate.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
#include <QQuickWindow>
#include <QtQml/qqmlregistration.h>

class Identity;
class Post;

class WindowController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    void setWindow(QQuickWindow *window);

    void restoreGeometry();
    Q_INVOKABLE void saveGeometry();

private:
    QQuickWindow *m_window = nullptr;
};