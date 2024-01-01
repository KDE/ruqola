/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class ShowAttachmentComboBoxTest : public QObject
{
    Q_OBJECT
public:
    explicit ShowAttachmentComboBoxTest(QObject *parent = nullptr);
    ~ShowAttachmentComboBoxTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};
