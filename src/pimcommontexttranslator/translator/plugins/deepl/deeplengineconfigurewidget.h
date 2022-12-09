/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QWidget>
class QCheckBox;
class DeeplEngineConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DeeplEngineConfigureWidget(QWidget *parent = nullptr);
    ~DeeplEngineConfigureWidget() override;
    void setUseFreeLicenceKey(bool b);
    Q_REQUIRED_RESULT bool useFreeLicenceKey() const;

private:
    QCheckBox *const mUseFreeLicense;
};
