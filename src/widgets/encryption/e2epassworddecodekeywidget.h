/*
  SPDX-FileCopyrightText: 2024 Laurent Montel <montel.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include <QWidget>
class KPasswordLineEdit;
class E2ePasswordDecodeKeyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit E2ePasswordDecodeKeyWidget(QWidget *parent = nullptr);
    ~E2ePasswordDecodeKeyWidget() override;

    [[nodiscard]] QString password() const;

private:
    KPasswordLineEdit *const mPassword;
};
