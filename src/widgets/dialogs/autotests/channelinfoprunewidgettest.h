/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ChannelInfoPruneWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ChannelInfoPruneWidgetTest(QObject *parent = nullptr);
    ~ChannelInfoPruneWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
