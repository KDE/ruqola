/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class UnicodeEmoticonManagerTest : public QObject
{
    Q_OBJECT
public:
    explicit UnicodeEmoticonManagerTest(QObject *parent = nullptr);
    ~UnicodeEmoticonManagerTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
