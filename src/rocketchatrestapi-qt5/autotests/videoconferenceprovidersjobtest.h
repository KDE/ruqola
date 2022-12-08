/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class VideoConferenceProvidersJobTest : public QObject
{
    Q_OBJECT
public:
    explicit VideoConferenceProvidersJobTest(QObject *parent = nullptr);
    ~VideoConferenceProvidersJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};
