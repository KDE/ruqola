/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class QLineEdit;
class LingvaEngineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LingvaEngineWidget(QWidget *parent = nullptr);
    ~LingvaEngineWidget() override;
    Q_REQUIRED_RESULT QString serverUrl() const;

    void setServerUrl(const QString &serverUrl);

private:
    QLineEdit *const mServerUrl;
};
