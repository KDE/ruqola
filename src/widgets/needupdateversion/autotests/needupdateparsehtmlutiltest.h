/*
  SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class NeedUpdateParseHtmlUtilTest : public QObject
{
    Q_OBJECT
public:
    explicit NeedUpdateParseHtmlUtilTest(QObject *parent = nullptr);
    ~NeedUpdateParseHtmlUtilTest() override = default;

private Q_SLOTS:
    void shouldExtraDate();
    void shouldExtraDate_data();
};
