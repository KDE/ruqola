/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class InviteInfoTest : public QObject
{
    Q_OBJECT
public:
    explicit InviteInfoTest(QObject *parent = nullptr);
    ~InviteInfoTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldParseInviteInfo_data();
    void shouldParseInviteInfo();
};
