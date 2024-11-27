/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class AppsCountInfoTest : public QObject
{
    Q_OBJECT
public:
    explicit AppsCountInfoTest(QObject *parent = nullptr);
    ~AppsCountInfoTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldLoadAppsCountInfo();
    void shouldLoadAppsCountInfo_data();
};
