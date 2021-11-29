/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class UploadFileProgressStatusWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit UploadFileProgressStatusWidgetTest(QObject *parent = nullptr);
    ~UploadFileProgressStatusWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldChangeText();
};

