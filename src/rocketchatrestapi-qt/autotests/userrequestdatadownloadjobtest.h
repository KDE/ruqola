/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class UserRequestDataDownloadJobTest : public QObject
{
    Q_OBJECT
public:
    explicit UserRequestDataDownloadJobTest(QObject *parent = nullptr);
    ~UserRequestDataDownloadJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};
