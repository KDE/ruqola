/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class VideoConferenceListJobTest : public QObject
{
    Q_OBJECT
public:
    explicit VideoConferenceListJobTest(QObject *parent = nullptr);
    ~VideoConferenceListJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};
