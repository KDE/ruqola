/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SearchTeamDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit SearchTeamDialogTest(QObject *parent = nullptr);
    ~SearchTeamDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
