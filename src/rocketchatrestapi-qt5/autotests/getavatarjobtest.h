/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class GetAvatarJobTest : public QObject
{
    Q_OBJECT
public:
    explicit GetAvatarJobTest(QObject *parent = nullptr);
    ~GetAvatarJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};

