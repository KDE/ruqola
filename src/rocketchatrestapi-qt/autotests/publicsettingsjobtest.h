/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class PublicSettingsJobTest : public QObject
{
    Q_OBJECT
public:
    explicit PublicSettingsJobTest(QObject *parent = nullptr);
    ~PublicSettingsJobTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};
