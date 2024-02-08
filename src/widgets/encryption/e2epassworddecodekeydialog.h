/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QDialog>
class E2ePasswordDecodeKeyWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT E2ePasswordDecodeKeyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit E2ePasswordDecodeKeyDialog(QWidget *parent = nullptr);
    ~E2ePasswordDecodeKeyDialog() override;

    [[nodiscard]] QString password() const;

private:
    E2ePasswordDecodeKeyWidget *const mE2ePasswordDecodeKeyWidget;
};
