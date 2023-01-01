/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class CustomUserStatusDeleteTestJob : public QObject
{
    Q_OBJECT
public:
    explicit CustomUserStatusDeleteTestJob(QObject *parent = nullptr);
    ~CustomUserStatusDeleteTestJob() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
    void shouldGenerateJson();
    void shouldNotStarting();
};
