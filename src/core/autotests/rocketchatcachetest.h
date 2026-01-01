/*
   SPDX-FileCopyrightText: 2018-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RocketChatCacheTest : public QObject
{
    Q_OBJECT
public:
    explicit RocketChatCacheTest(QObject *parent = nullptr);
    ~RocketChatCacheTest() override = default;
private Q_SLOTS:
    void shouldGenerateFileCachePath();
    void shouldGenerateFileCachePath_data();
};
