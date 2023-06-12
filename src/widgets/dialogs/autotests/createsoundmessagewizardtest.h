/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class CreateSoundMessageWizardTest : public QObject
{
    Q_OBJECT
public:
    explicit CreateSoundMessageWizardTest(QObject *parent = nullptr);
    ~CreateSoundMessageWizardTest() override = default;
};
