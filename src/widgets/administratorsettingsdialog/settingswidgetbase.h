/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

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
class QToolButton;
class LIBRUQOLAWIDGETS_TESTS_EXPORT SettingsWidgetBase : public QScrollArea
{
    Q_OBJECT
public:
    explicit SettingsWidgetBase(RocketChatAccount *account, QWidget *parent = nullptr);
    ~SettingsWidgetBase() override;

    struct SettingsInfo {
        bool readOnly = false;
        bool enterprise = false;
        QStringList modules;
        QVariant value;
    };

    virtual void initialize(const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings) = 0;

Q_SIGNALS:
    void changedDone(const QString &buttonObjectName);
    void changedCanceled(const QString &variableName);

protected:
    void addCheckBox(QCheckBox *checkBox, const QString &variable);
    void addSpinbox(const QString &labelStr, QSpinBox *spinBox, const QString &variable);
    void addLineEdit(const QString &labelStr, QLineEdit *lineEdit, const QString &variable);

    void addComboBox(const QString &labelStr, const QMap<QString, QString> &items, QComboBox *comboBox, const QString &variable);
    void fillComboBox(QComboBox *comboBox, const QMap<QString, QString> &items);

    void addPasswordEdit(const QString &labelStr, KPasswordLineEdit *lineEdit, const QString &variable);
    void addLabel(const QString &labelStr, QLabel *labelElement, const QString &variable);
    void addPlainTextEdit(const QString &labelStr, QPlainTextEdit *lineEdit, const QString &variable);

    QWidget *const mCurrentWidget;
    QFormLayout *mMainLayout = nullptr;

    void
    initializeWidget(QPlainTextEdit *lineEdit, const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings, const QString &defaultValue = QString());
    void initializeWidget(KPasswordLineEdit *lineEdit, const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings);
    void initializeWidget(QLineEdit *lineEdit, const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings, const QString &defaultValue = QString());
    void initializeWidget(QCheckBox *checkbox, const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings, bool defaultValue = false);
    void initializeWidget(QSpinBox *spinbox, const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings, int defaultValue = -1);
    void initializeWidget(QComboBox *comboBox, const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings, const QString &defaultValue = QString());
    void initializeWidget(QLabel *label, const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings, const QString &defaultValue);

    [[nodiscard]] QLabel *createBoldLabel(const QString &text);
    [[nodiscard]] QString urlFromRelativePath(const QString &relativePath);

    RocketChatAccount *const mAccount;

private:
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT bool updateSettings(const QString &settingName,
                                                                 const QVariant &value,
                                                                 RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo::ValueType typeValue,
                                                                 const QString &buttonObjectName = {});
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAdminSettingsDone(const QJsonObject &obj, const QString &buttonObjectName);
    LIBRUQOLAWIDGETS_NO_EXPORT void disableToolButton(const QString &variableName, bool differentFromDefaultValue);
    LIBRUQOLAWIDGETS_NO_EXPORT QToolButton *addRestoreButton(const QString &variable);
    LIBRUQOLAWIDGETS_NO_EXPORT QToolButton *addApplyButton(const QString &variable);
    LIBRUQOLAWIDGETS_NO_EXPORT QToolButton *addCancelButton(const QString &variable);
    LIBRUQOLAWIDGETS_NO_EXPORT void hideButtons(const QString &variableName);
    [[nodiscard]] LIBRUQOLAWIDGETS_NO_EXPORT bool hasNecessaryLicense(const QStringList &lst) const;
};
LIBRUQOLAWIDGETS_EXPORT QDebug operator<<(QDebug d, const SettingsWidgetBase::SettingsInfo &t);
