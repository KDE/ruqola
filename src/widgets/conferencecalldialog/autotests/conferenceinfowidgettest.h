/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ConferenceInfoWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit ConferenceInfoWidgetTest(QObject *parent = nullptr);
    ~ConferenceInfoWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
