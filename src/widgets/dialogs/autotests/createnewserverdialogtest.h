/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class CreateNewServerDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit CreateNewServerDialogTest(QObject *parent = nullptr);
    ~CreateNewServerDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

