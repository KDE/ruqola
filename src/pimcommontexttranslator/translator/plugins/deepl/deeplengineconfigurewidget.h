/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class QCheckBox;
class QLineEdit;
class DeeplEngineConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DeeplEngineConfigureWidget(QWidget *parent = nullptr);
    ~DeeplEngineConfigureWidget() override;
    void setUseFreeLicenceKey(bool b);
    Q_REQUIRED_RESULT bool useFreeLicenceKey() const;

    Q_REQUIRED_RESULT QString apiKey() const;
    void setApiKey(const QString &key);

private:
    QCheckBox *const mUseFreeLicense;
    QLineEdit *const mApiKey;
};
