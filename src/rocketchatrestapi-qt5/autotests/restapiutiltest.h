/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RestApiUtilTest : public QObject
{
    Q_OBJECT
public:
    explicit RestApiUtilTest(QObject *parent = nullptr);
    ~RestApiUtilTest() override = default;
private Q_SLOTS:
    void shouldAdaptUrl_data();
    void shouldAdaptUrl();

    void shouldUseV1Version();
};
