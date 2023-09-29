/*
   SPDX-FileCopyrightText: 2018-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class ReportMessageJobTest : public QObject
{
    Q_OBJECT
public:
    explicit ReportMessageJobTest(QObject *parent = nullptr);
    ~ReportMessageJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldHaveMessageId();
    void shouldGenerateJobRequest();
    void shouldGenerateJson();
};
