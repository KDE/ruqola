/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class ApplicationsSettingsDescriptionDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationsSettingsDescriptionDialogTest(QObject *parent = nullptr);
    ~ApplicationsSettingsDescriptionDialogTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
