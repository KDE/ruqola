/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include <QObject>

class ChannelNameValidLineEditTest : public QObject
{
    Q_OBJECT
public:
    explicit ChannelNameValidLineEditTest(QObject *parent = nullptr);
    ~ChannelNameValidLineEditTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
};
