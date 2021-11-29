/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class TeamChannelsWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit TeamChannelsWidgetTest(QObject *parent = nullptr);
    ~TeamChannelsWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
