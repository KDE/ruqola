/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class TimeStampInMessagesDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit TimeStampInMessagesDialogTest(QObject *parent = nullptr);
    ~TimeStampInMessagesDialogTest() override = default;
};
