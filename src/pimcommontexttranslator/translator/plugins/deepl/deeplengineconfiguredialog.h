/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QDialog>
class DeeplEngineConfigureWidget;
class DeeplEngineConfigureDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DeeplEngineConfigureDialog(QWidget *parent = nullptr);
    ~DeeplEngineConfigureDialog() override;

    void setUseFreeLicenceKey(bool b);
    Q_REQUIRED_RESULT bool useFreeLicenceKey() const;

private:
    DeeplEngineConfigureWidget *const mConfigureWidget;
};
