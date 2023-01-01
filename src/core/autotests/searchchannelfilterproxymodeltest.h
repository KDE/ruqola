/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class SearchChannelFilterProxyModelTest : public QObject
{
    Q_OBJECT
public:
    explicit SearchChannelFilterProxyModelTest(QObject *parent = nullptr);
    ~SearchChannelFilterProxyModelTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldLoadChannelJSon();
};
