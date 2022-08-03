/*
   SPDX-FileCopyrightText: 2018-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ChangeGroupsNameJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ChangeGroupsNameJobTest(QObject *parent = nullptr);
    ~ChangeGroupsNameJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
};
