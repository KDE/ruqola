/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RemoveAccountDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit RemoveAccountDialogTest(QObject *parent = nullptr);
    ~RemoveAccountDialogTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
