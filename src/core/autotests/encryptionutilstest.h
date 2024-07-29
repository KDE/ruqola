/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class EncryptionUtilsTest : public QObject
{
    Q_OBJECT
public:
    explicit EncryptionUtilsTest(QObject *parent = nullptr);
    ~EncryptionUtilsTest() override = default;

private Q_SLOTS:
    void shouldExportJWKKey();

    void shouldSplitVectorAndEcryptedData_data();
    void shouldSplitVectorAndEcryptedData();
    void shouldJoinVectorAndEcryptedData_data();
    void shouldJoinVectorAndEcryptedData();
};
