/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ExplorePermissionsDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit ExplorePermissionsDialogTest(QObject *parent = nullptr);
    ~ExplorePermissionsDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
