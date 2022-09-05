/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class TranslationConfigureLanguageListWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit TranslationConfigureLanguageListWidgetTest(QObject *parent = nullptr);
    ~TranslationConfigureLanguageListWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldAssignLabel();
};
