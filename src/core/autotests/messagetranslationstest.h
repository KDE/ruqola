/*
   SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class MessageTranslationsTest : public QObject
{
    Q_OBJECT
public:
    explicit MessageTranslationsTest(QObject *parent = nullptr);
    ~MessageTranslationsTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
