/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class BannerInfoDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit BannerInfoDialogTest(QObject *parent = nullptr);
    ~BannerInfoDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
