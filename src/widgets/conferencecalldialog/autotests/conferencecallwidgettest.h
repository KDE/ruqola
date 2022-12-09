/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ConferenceCallWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ConferenceCallWidgetTest(QObject *parent = nullptr);
    ~ConferenceCallWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
