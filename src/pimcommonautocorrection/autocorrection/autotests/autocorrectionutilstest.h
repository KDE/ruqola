/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class AutoCorrectionUtilsTest : public QObject
{
    Q_OBJECT
public:
    explicit AutoCorrectionUtilsTest(QObject *parent = nullptr);
    ~AutoCorrectionUtilsTest() override = default;
private Q_SLOTS:
    void shouldConvertToLibreOfficeFilename();
    void shouldSplitString();
    void shouldSplitString_data();
};
