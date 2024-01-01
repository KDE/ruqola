/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class FileUploadSettingsWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit FileUploadSettingsWidgetTest(QObject *parent = nullptr);
    ~FileUploadSettingsWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
