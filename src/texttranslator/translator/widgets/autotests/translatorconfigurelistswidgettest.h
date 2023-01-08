/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class TranslatorConfigureListsWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit TranslatorConfigureListsWidgetTest(QObject *parent = nullptr);
    ~TranslatorConfigureListsWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
