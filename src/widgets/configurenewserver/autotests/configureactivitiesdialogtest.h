/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ConfigureActivitiesDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit ConfigureActivitiesDialogTest(QObject *parent = nullptr);
    ~ConfigureActivitiesDialogTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
