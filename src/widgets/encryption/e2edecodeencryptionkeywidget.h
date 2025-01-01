/*
  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <KMessageWidget>

class LIBRUQOLAWIDGETS_TESTS_EXPORT E2eDecodeEncryptionKeyWidget : public KMessageWidget
{
    Q_OBJECT
public:
    explicit E2eDecodeEncryptionKeyWidget(QWidget *parent = nullptr);
    ~E2eDecodeEncryptionKeyWidget() override;

Q_SIGNALS:
    void decodeEncrytionKey();
};
