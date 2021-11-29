/*
   SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ConverterTextAbstractJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ConverterTextAbstractJobTest(QObject *parent = nullptr);
    ~ConverterTextAbstractJobTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValue();
};

