/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class DownloadAppsLanguagesManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit DownloadAppsLanguagesManagerTest(QObject *parent = nullptr);
    ~DownloadAppsLanguagesManagerTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
