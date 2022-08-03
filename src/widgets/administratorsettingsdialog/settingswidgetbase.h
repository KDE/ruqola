/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include "settings/updateadminsettingsjob.h"
#include <QScrollArea>
#include <QVariant>
class RocketChatAccount;
class QCheckBox;
class QSpinBox;
class QFormLayout;
class QLineEdit;
class QComboBox;
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SettingsWidgetBase : public QScrollArea
{
    Q_OBJECT
public:
    explicit SettingsWidgetBase(RocketChatAccount *account, QWidget *parent = nullptr);
    ~SettingsWidgetBase() override;

    virtual void initialize(const QMap<QString, QVariant> &mapSettings) = 0;

protected:
    void connectCheckBox(QCheckBox *checkBox, const QString &variable);
    void addSpinbox(const QString &labelStr, QSpinBox *spinBox, const QString &variable);
    void addLineEdit(const QString &labelStr, QLineEdit *lineEdit, const QString &variable);
    void addComboBox(const QString &labelStr, const QMap<QString, QString> &items, QComboBox *comboBox, const QString &variable);

    QWidget *const mCurrentWidget;
    QFormLayout *mMainLayout = nullptr;

    void initializeWidget(QLineEdit *lineEdit, const QMap<QString, QVariant> &mapSettings);
    void initializeWidget(QCheckBox *checkbox, const QMap<QString, QVariant> &mapSettings);
    void initializeWidget(QSpinBox *spinbox, const QMap<QString, QVariant> &mapSettings);
    void initializeWidget(QComboBox *comboBox, const QMap<QString, QVariant> &mapSettings);

    Q_REQUIRED_RESULT QLabel *createBoldLabel(const QString &text);

private:
    void
    updateSettings(const QString &settingName, const QVariant &value, RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo::ValueType typeValue);
    void slotAdminSettingsDone(const QJsonObject &obj);
    RocketChatAccount *const mAccount;
};
