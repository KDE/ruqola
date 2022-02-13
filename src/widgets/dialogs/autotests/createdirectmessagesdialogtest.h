/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class CreateDirectMessagesDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit CreateDirectMessagesDialogTest(QObject *parent = nullptr);
    ~CreateDirectMessagesDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

