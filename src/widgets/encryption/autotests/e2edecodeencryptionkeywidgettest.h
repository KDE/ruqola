/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class E2eDecodeEncryptionKeyWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit E2eDecodeEncryptionKeyWidgetTest(QObject *parent = nullptr);
    ~E2eDecodeEncryptionKeyWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldEmitSaveSignal();
};
