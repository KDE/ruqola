/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QDialog>
class LibreTranslateEngineConfigureWidget;
class LibreTranslateEngineConfigureDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LibreTranslateEngineConfigureDialog(QWidget *parent = nullptr);
    ~LibreTranslateEngineConfigureDialog() override;

    Q_REQUIRED_RESULT QString serverUrl() const;

    void setServerUrl(const QString &serverUrl);

private:
    LibreTranslateEngineConfigureWidget *const mConfigureWidget;
};
