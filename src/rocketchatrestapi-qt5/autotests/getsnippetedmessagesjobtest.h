/*
   SPDX-FileCopyrightText: 2019-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class GetSnippetedMessagesJobTest : public QObject
{
    Q_OBJECT
public:
    explicit GetSnippetedMessagesJobTest(QObject *parent = nullptr);
    ~GetSnippetedMessagesJobTest() override = default;

private Q_SLOTS:
    void shouldGenerateRequest();
    void shouldNotStarting();
    void shouldHaveDefaultValue();
};
