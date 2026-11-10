/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SettingsAddCustomOAuthJobTest : public QObject
{
    Q_OBJECT
public:
    explicit SettingsAddCustomOAuthJobTest(QObject *parent = nullptr);
    ~SettingsAddCustomOAuthJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
    void shouldNotStarting();
};
