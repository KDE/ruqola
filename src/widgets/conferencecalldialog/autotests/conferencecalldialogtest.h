/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ConferenceCallDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit ConferenceCallDialogTest(QObject *parent = nullptr);
    ~ConferenceCallDialogTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
