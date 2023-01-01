/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class SearchTeamWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit SearchTeamWidgetTest(QObject *parent = nullptr);
    ~SearchTeamWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
