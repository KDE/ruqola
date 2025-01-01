/*
  SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWidget>

class KPasswordLineEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT E2ePasswordDecodeKeyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit E2ePasswordDecodeKeyWidget(QWidget *parent = nullptr);
    ~E2ePasswordDecodeKeyWidget() override;

    [[nodiscard]] QString password() const;

private:
    KPasswordLineEdit *const mPassword;
};
