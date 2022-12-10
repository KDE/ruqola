/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class VideoConferenceInfoJobTest : public QObject
{
    Q_OBJECT
public:
    explicit VideoConferenceInfoJobTest(QObject *parent = nullptr);
    ~VideoConferenceInfoJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};
