/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QObject>

class E2eSaveEncryptionKeyWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit E2eSaveEncryptionKeyWidgetTest(QObject *parent = nullptr);
    ~E2eSaveEncryptionKeyWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldEmitSaveSignal();
};
