/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ModerationMessagesWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ModerationMessagesWidgetTest(QObject *parent = nullptr);
    ~ModerationMessagesWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
