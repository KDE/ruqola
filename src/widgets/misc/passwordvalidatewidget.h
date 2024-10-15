/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include "ruqolaserverconfig.h"
#include <QWidget>
class QGridLayout;

class LIBRUQOLAWIDGETS_TESTS_EXPORT PasswordValidateLabel : public QWidget
{
    Q_OBJECT
public:
    explicit PasswordValidateLabel(QWidget *parent = nullptr);
    ~PasswordValidateLabel() override;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT PasswordValidateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PasswordValidateWidget(QWidget *parent = nullptr);
    ~PasswordValidateWidget() override;

    void setPasswordValidChecks(const RuqolaServerConfig::PasswordSettings &passwordSettings);

    void validatePassword(const QString &password);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void initializeWidget();
    RuqolaServerConfig::PasswordSettings mPasswordSettings;
    QGridLayout *const mGridLayout;
    // QMap<RuqolaServerConfig::PasswordSettings::PasswordSettingCheck, QWidget *> mWidgetMap;
};
