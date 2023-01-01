/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class CreateVideoMessageDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit CreateVideoMessageDialogTest(QObject *parent = nullptr);
    ~CreateVideoMessageDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
