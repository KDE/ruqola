/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class WhatsNewInfoTest : public QObject
{
    Q_OBJECT
public:
    explicit WhatsNewInfoTest(QObject *parent = nullptr);
    ~WhatsNewInfoTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
