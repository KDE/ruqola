/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AdministratorCustomEmojiCreateWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit AdministratorCustomEmojiCreateWidgetTest(QObject *parent = nullptr);
    ~AdministratorCustomEmojiCreateWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
