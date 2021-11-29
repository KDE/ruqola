/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RoomUtilTest : public QObject
{
    Q_OBJECT
public:
    explicit RoomUtilTest(QObject *parent = nullptr);
    ~RoomUtilTest() override = default;
private Q_SLOTS:
    void shouldGeneratePermalink();
    void shouldGeneratePermalink_data();
};

