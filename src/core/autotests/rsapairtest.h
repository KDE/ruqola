
/*
   SPDX-FileCopyrightText: 2025 Andro Ranogajec <ranogaet@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RsaPairTest : public QObject
{
    Q_OBJECT
public:
    explicit RsaPairTest(QObject *parent = nullptr);
    ~RsaPairTest() override = default;

private Q_SLOTS:
    void rsaPairGenerationNonDeterminismTest();
    void encryptDecryptDeterminismTest();
};
