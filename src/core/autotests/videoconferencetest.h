/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class VideoConferenceTest : public QObject
{
    Q_OBJECT
public:
    explicit VideoConferenceTest(QObject *parent = nullptr);
    ~VideoConferenceTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldExtractVideoConference();
    void shouldExtractVideoConference_data();
};
