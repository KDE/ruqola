/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class QLineEdit;
class LibreTranslateEngineConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LibreTranslateEngineConfigureWidget(QWidget *parent = nullptr);
    ~LibreTranslateEngineConfigureWidget() override;

    Q_REQUIRED_RESULT QString serverUrl() const;

private:
    QLineEdit *const mServerUrl;
};
