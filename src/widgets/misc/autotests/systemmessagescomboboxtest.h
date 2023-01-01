/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SystemMessagesComboBoxTest : public QObject
{
    Q_OBJECT
public:
    explicit SystemMessagesComboBoxTest(QObject *parent = nullptr);
    ~SystemMessagesComboBoxTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
};
