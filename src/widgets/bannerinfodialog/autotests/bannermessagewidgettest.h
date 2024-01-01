/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class BannerMessageWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit BannerMessageWidgetTest(QObject *parent = nullptr);
    ~BannerMessageWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
