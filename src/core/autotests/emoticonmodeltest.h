/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class EmoticonModelTest : public QObject
{
    Q_OBJECT
public:
    explicit EmoticonModelTest(QObject *parent = nullptr);
    ~EmoticonModelTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldListEmojis();
    void shouldFilterCategory();
};
