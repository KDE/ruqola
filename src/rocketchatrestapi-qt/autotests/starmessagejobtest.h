/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class StarMessageJobTest : public QObject
{
    Q_OBJECT
public:
    explicit StarMessageJobTest(QObject *parent = nullptr);
    ~StarMessageJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldHaveMessageId();
    void shouldGenerateStarMessageRequest();
    void shouldGenerateUnStarMessageRequest();
    void shouldGenerateJson();
};
