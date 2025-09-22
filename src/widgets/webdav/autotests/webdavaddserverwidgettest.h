/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include <QObject>

class WebDavAddServerWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit WebDavAddServerWidgetTest(QObject *parent = nullptr);
    ~WebDavAddServerWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldChangeEnableButtonStatus();
};
