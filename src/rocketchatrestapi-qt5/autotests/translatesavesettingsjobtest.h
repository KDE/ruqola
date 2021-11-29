/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class TranslateSaveSettingsJobTest : public QObject
{
    Q_OBJECT
public:
    explicit TranslateSaveSettingsJobTest(QObject *parent = nullptr);
    ~TranslateSaveSettingsJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
    void shouldNotStarting();
};

