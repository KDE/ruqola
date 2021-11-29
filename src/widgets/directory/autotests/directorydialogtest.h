/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class DirectoryDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit DirectoryDialogTest(QObject *parent = nullptr);
    ~DirectoryDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
