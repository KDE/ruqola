/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RoomWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit RoomWidgetTest(QObject *parent = nullptr);
    ~RoomWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldStorePendingTextPerRoom();
    void shouldShowNoticeWhenReplyingToThread();
};

