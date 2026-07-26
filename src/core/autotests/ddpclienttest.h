/*
   SPDX-FileCopyrightText: 2026 Till Adam <till.adam@kdab.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class DDPClientTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void closeNotificationSurvivesAuthenticationCleanup_data();
    void closeNotificationSurvivesAuthenticationCleanup();
};
