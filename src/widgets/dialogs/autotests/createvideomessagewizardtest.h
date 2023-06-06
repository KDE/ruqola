/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class CreateVideoMessageWizardTest : public QObject
{
    Q_OBJECT
public:
    explicit CreateVideoMessageWizardTest(QObject *parent = nullptr);
    ~CreateVideoMessageWizardTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
