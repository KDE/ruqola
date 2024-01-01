/*
   SPDX-FileCopyrightText: 2019-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AutotranslateLanguagesTest : public QObject
{
    Q_OBJECT
public:
    explicit AutotranslateLanguagesTest(QObject *parent = nullptr);
    ~AutotranslateLanguagesTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
