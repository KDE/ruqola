/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class GetMentionedMessagesJobTest : public QObject
{
    Q_OBJECT
public:
    explicit GetMentionedMessagesJobTest(QObject *parent = nullptr);
    ~GetMentionedMessagesJobTest() override = default;

private Q_SLOTS:
    void shouldGenerateRequest();
    void shouldNotStarting();
    void shouldHaveDefaultValue();
};
