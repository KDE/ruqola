/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SetUserPublicAndPrivateKeysJobTest : public QObject
{
    Q_OBJECT
public:
    explicit SetUserPublicAndPrivateKeysJobTest(QObject *parent = nullptr);
    ~SetUserPublicAndPrivateKeysJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
    void shouldNotStarting();
};
