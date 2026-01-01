/*
   SPDX-FileCopyrightText: 2024-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include "ruqolaserverconfig.h"
#include <QMap>
#include <QWidget>
class QGridLayout;
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT PasswordValidateLabel : public QWidget
{
    Q_OBJECT
public:
    explicit PasswordValidateLabel(QWidget *parent = nullptr);
    ~PasswordValidateLabel() override;
    void setText(RuqolaServerConfig::PasswordSettings::PasswordSettingCheck c, int value = -1);
    void setValidStatus(bool b);

private:
    QLabel *const mIconLabel;
    QLabel *const mLabel;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT PasswordValidateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PasswordValidateWidget(QWidget *parent = nullptr);
    ~PasswordValidateWidget() override;

    void setPasswordValidChecks(const RuqolaServerConfig::PasswordSettings &passwordSettings);

    void validatePassword(const QString &password);

Q_SIGNALS:
    void passwordIsValid(bool valid);

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void addLabel(RuqolaServerConfig::PasswordSettings::PasswordSettingCheck c, int row, int col, int value = -1);
    LIBRUQOLAWIDGETS_NO_EXPORT void initializeWidget();
    LIBRUQOLAWIDGETS_NO_EXPORT void setValidStatus(RuqolaServerConfig::PasswordSettings::PasswordSettingCheck c, bool b);
    LIBRUQOLAWIDGETS_NO_EXPORT void updateGridPosition(int &row, int &col);
    RuqolaServerConfig::PasswordSettings mPasswordSettings;
    QMap<RuqolaServerConfig::PasswordSettings::PasswordSettingCheck, PasswordValidateLabel *> mWidgetMap;
    QGridLayout *const mGridLayout;
};
