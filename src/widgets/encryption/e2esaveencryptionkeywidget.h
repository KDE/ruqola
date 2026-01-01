/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <KMessageWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT E2eSaveEncryptionKeyWidget : public KMessageWidget
{
    Q_OBJECT
public:
    explicit E2eSaveEncryptionKeyWidget(QWidget *parent = nullptr);
    ~E2eSaveEncryptionKeyWidget() override;

Q_SIGNALS:
    void saveEncrytionKey();
};
