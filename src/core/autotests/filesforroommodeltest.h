/*
   SPDX-FileCopyrightText: 2018-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class FilesForRoomModelTest : public QObject
{
    Q_OBJECT
public:
    explicit FilesForRoomModelTest(QObject *parent = nullptr);
    ~FilesForRoomModelTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAddFiles();
    void shouldVerifyData();
};

