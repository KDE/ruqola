/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include <QObject>

class AdministratorInvitesWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit AdministratorInvitesWidgetTest(QObject *parent = nullptr);
    ~AdministratorInvitesWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
