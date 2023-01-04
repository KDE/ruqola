/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

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

    Q_REQUIRED_RESULT QString apiKey() const;
    void setApiKey(const QString &key);

    Q_REQUIRED_RESULT bool serverRequiredApiKey() const;
    void setServerRequiredApiKey(bool state);

private:
    LibreTranslateEngineConfigureWidget *const mConfigureWidget;
};
