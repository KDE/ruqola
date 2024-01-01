/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class SelectedMessageBackgroundAnimationTest : public QObject
{
    Q_OBJECT
public:
    explicit SelectedMessageBackgroundAnimationTest(QObject *parent = nullptr);
    ~SelectedMessageBackgroundAnimationTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
