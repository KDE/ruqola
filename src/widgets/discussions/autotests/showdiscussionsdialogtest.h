/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ShowDiscussionsDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit ShowDiscussionsDialogTest(QObject *parent = nullptr);
    ~ShowDiscussionsDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
