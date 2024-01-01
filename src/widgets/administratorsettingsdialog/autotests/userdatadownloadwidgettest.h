/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class UserDataDownloadWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit UserDataDownloadWidgetTest(QObject *parent = nullptr);
    ~UserDataDownloadWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
