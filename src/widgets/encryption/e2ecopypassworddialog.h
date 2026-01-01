/*
  SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once

#include "libruqolawidgets_private_export.h"
#include <QDialog>
class E2eCopyPasswordWidget;
class RocketChatAccount;
class LIBRUQOLAWIDGETS_TESTS_EXPORT E2eCopyPasswordDialog : public QDialog
{
    Q_OBJECT
public:
    explicit E2eCopyPasswordDialog(RocketChatAccount *account, QWidget *parent = nullptr);
    ~E2eCopyPasswordDialog() override;

private:
    E2eCopyPasswordWidget *const mE2eCopyPasswordWidget;
};
