/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class FilesForRoomFilterProxyModelTest : public QObject
{
    Q_OBJECT
public:
    explicit FilesForRoomFilterProxyModelTest(QObject *parent = nullptr);
    ~FilesForRoomFilterProxyModelTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldHaveDefaultValues();
    void shouldAssignValue();
};

