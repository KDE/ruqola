/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RoomsAutocompleteAvailableForTeamsJobTest : public QObject
{
    Q_OBJECT
public:
    explicit RoomsAutocompleteAvailableForTeamsJobTest(QObject *parent = nullptr);
    ~RoomsAutocompleteAvailableForTeamsJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};
