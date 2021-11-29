/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SearchMessageFilterProxyModelTest : public QObject
{
    Q_OBJECT
public:
    explicit SearchMessageFilterProxyModelTest(QObject *parent = nullptr);
    ~SearchMessageFilterProxyModelTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
};

