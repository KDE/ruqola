/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class DownloadAvartarUrlManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit DownloadAvartarUrlManagerTest(QObject *parent = nullptr);
    ~DownloadAvartarUrlManagerTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
