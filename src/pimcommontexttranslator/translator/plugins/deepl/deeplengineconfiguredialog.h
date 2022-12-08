/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

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

private:
    DeeplEngineConfigureWidget *const mConfigureWidget;
};
