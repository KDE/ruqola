/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class CreateSoundMessageDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit CreateSoundMessageDialogTest(QObject *parent = nullptr);
    ~CreateSoundMessageDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
