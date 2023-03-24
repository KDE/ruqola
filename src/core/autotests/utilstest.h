/*
   SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class UtilsTest : public QObject
{
    Q_OBJECT
public:
    explicit UtilsTest(QObject *parent = nullptr);
    ~UtilsTest() override = default;
private Q_SLOTS:
    void shouldGenerateServerUrl_data();
    void shouldGenerateServerUrl();

    void shouldExtractRoomUserFromUrl_data();
    void shouldExtractRoomUserFromUrl();

    void shouldConvertTextWithUrl_data();
    void shouldConvertTextWithUrl();

    void shouldGenerateAvatarUrl_data();
    void shouldGenerateAvatarUrl();

    void shouldTestAvatarInfoValues();
};
