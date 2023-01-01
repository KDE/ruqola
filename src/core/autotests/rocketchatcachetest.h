/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

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
};
