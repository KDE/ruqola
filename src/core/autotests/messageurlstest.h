/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MessageUrlsTest : public QObject
{
    Q_OBJECT
public:
    explicit MessageUrlsTest(QObject *parent = nullptr);
    ~MessageUrlsTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValue();
};
