/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class BannerInfoTest : public QObject
{
    Q_OBJECT
public:
    explicit BannerInfoTest(QObject *parent = nullptr);
    ~BannerInfoTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldBannerInfo();
    void shouldBannerInfo_data();
};
