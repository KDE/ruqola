/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ThreadMessageDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit ThreadMessageDialogTest(QObject *parent = nullptr);
    ~ThreadMessageDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
