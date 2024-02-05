/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include <QObject>

class E2eDecodeEncryptionKeyFailedWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit E2eDecodeEncryptionKeyFailedWidgetTest(QObject *parent = nullptr);
    ~E2eDecodeEncryptionKeyFailedWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
