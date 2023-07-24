/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ManageLoadHistoryParseSyncMessagesUtilsTest : public QObject
{
    Q_OBJECT
public:
    explicit ManageLoadHistoryParseSyncMessagesUtilsTest(QObject *parent = nullptr);
    ~ManageLoadHistoryParseSyncMessagesUtilsTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldParseSyncMessages_data();
    void shouldParseSyncMessages();
};
