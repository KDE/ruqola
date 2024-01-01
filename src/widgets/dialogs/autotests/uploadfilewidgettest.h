/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class UploadFileWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit UploadFileWidgetTest(QObject *parent = nullptr);
    ~UploadFileWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldReturnEmptyResult();
};
