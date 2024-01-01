/*
   SPDX-FileCopyrightText: 2018-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class FileTest : public QObject
{
    Q_OBJECT
public:
    explicit FileTest(QObject *parent = nullptr);
    ~FileTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldAssignValue();
    void shouldCopyValue();

    void shouldParseFile_data();
    void shouldParseFile();
};
