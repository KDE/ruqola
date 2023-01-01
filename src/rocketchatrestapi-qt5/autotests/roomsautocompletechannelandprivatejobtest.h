/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RoomsAutocompleteChannelAndPrivateJobTest : public QObject
{
    Q_OBJECT
public:
    explicit RoomsAutocompleteChannelAndPrivateJobTest(QObject *parent = nullptr);
    ~RoomsAutocompleteChannelAndPrivateJobTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldGenerateRequest();
};
