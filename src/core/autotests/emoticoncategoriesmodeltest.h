/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include <QObject>

class EmoticonCategoriesModelTest : public QObject
{
    Q_OBJECT
public:
    explicit EmoticonCategoriesModelTest(QObject *parent = nullptr);
    ~EmoticonCategoriesModelTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

