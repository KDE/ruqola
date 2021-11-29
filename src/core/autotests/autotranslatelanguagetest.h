/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AutotranslateLanguageTest : public QObject
{
    Q_OBJECT
public:
    explicit AutotranslateLanguageTest(QObject *parent = nullptr);
    ~AutotranslateLanguageTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValue();
};

