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
class KPasswordLineEdit;
class QPlainTextEdit;
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
    void addPasswordEdit(const QString &labelStr, KPasswordLineEdit *lineEdit, const QString &variable);
    void addLabel(const QString &labelStr, QLabel *labelElement, const QString &variable);
    void addPlainTextEdit(const QString &labelStr, QPlainTextEdit *lineEdit, const QString &variable);

    QWidget *const mCurrentWidget;
    QFormLayout *mMainLayout = nullptr;

    void initializeWidget(QPlainTextEdit *lineEdit, const QMap<QString, QVariant> &mapSettings);
    void initializeWidget(KPasswordLineEdit *lineEdit, const QMap<QString, QVariant> &mapSettings);
    void initializeWidget(QLineEdit *lineEdit, const QMap<QString, QVariant> &mapSettings, const QString &defaultValue = QString());
    void initializeWidget(QCheckBox *checkbox, const QMap<QString, QVariant> &mapSettings, bool defaultValue = false);
    void initializeWidget(QSpinBox *spinbox, const QMap<QString, QVariant> &mapSettings, int defaultValue = -1);
    void initializeWidget(QComboBox *comboBox, const QMap<QString, QVariant> &mapSettings, const QString &defaultValue = QString());
    void initializeWidget(QLabel *label, const QMap<QString, QVariant> &mapSettings, const QString &defaultValue);

    Q_REQUIRED_RESULT QLabel *createBoldLabel(const QString &text);

private:
    void
    updateSettings(const QString &settingName, const QVariant &value, RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo::ValueType typeValue);
    void slotAdminSettingsDone(const QJsonObject &obj);
    RocketChatAccount *const mAccount;
};
