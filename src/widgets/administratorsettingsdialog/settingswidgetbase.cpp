/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "settingswidgetbase.h"
#include "connection.h"
#include "dialogs/confirmpassworddialog.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"

#include <KLocalizedString>

#include <QCheckBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QToolButton>

namespace
{
const char s_property[] = "settings_name";
}

SettingsWidgetBase::SettingsWidgetBase(RocketChatAccount *account, QWidget *parent)
    : QScrollArea{parent}
    , mCurrentWidget(new QWidget(parent))
    , mAccount(account)
{
    mCurrentWidget->setObjectName(QStringLiteral("mCurrentWidget"));
    setWidgetResizable(true);
    setWidget(mCurrentWidget);

    mMainLayout = new QFormLayout(mCurrentWidget);
    mMainLayout->setObjectName(QStringLiteral("mainLayout"));
}

SettingsWidgetBase::~SettingsWidgetBase() = default;

void SettingsWidgetBase::connectCheckBox(QCheckBox *checkBox, const QString &variable)
{
    checkBox->setProperty(s_property, variable);
    connect(checkBox, &QCheckBox::clicked, this, [this, variable](bool checked) {
        updateSettings(variable, checked, RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo::Boolean);
    });
}

void SettingsWidgetBase::updateSettings(const QString &settingName,
                                        const QVariant &value,
                                        RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo::ValueType typeValue)
{
    if (mAccount) {
        QString password;
        QPointer<ConfirmPasswordDialog> dialog(new ConfirmPasswordDialog(this));
        if (dialog->exec()) {
            password = dialog->password();
        }
        delete dialog;

        auto job = new RocketChatRestApi::UpdateAdminSettingsJob(this);
        RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo info;
        info.settingsValue = value;
        info.settingName = settingName;
        info.valueType = typeValue;
        job->setUpdateAdminSettingsInfo(info);
        job->setAuthMethod(QStringLiteral("password"));
        job->setAuthCode(QString::fromLatin1(Utils::convertSha256Password(password)));
        mAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::UpdateAdminSettingsJob::updateAdminSettingsDone, this, &SettingsWidgetBase::slotAdminSettingsDone);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start UpdateAdminSettingsJob job";
        }
    }
}

void SettingsWidgetBase::slotAdminSettingsDone(const QJsonObject &obj)
{
    qDebug() << "AccountSettingsWidget::slotAdminSettingsDone " << obj;
}

void SettingsWidgetBase::addSpinbox(const QString &labelStr, QSpinBox *spinBox, const QString &variable)
{
    auto layout = new QHBoxLayout;
    auto label = new QLabel(labelStr, this);
    layout->addWidget(label);
    layout->addWidget(spinBox);
    auto toolButton = new QToolButton(this);
    toolButton->setText(i18n("Apply"));
    toolButton->setProperty(s_property, variable);
    spinBox->setProperty(s_property, variable);
    layout->addWidget(toolButton);
    toolButton->setEnabled(false);
    connect(toolButton, &QToolButton::clicked, this, [this, variable, spinBox]() {
        updateSettings(variable, spinBox->value(), RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo::Integer);
    });
    connect(spinBox, &QSpinBox::valueChanged, this, [this, toolButton]() {
        toolButton->setEnabled(true);
    });

    mMainLayout->addRow(layout);
}

void SettingsWidgetBase::addLineEdit(const QString &labelStr, QLineEdit *lineEdit, const QString &variable)
{
    auto layout = new QHBoxLayout;
    auto label = new QLabel(labelStr, this);
    layout->addWidget(label);
    layout->addWidget(lineEdit);
    auto toolButton = new QToolButton(this);
    toolButton->setText(i18n("Apply"));
    toolButton->setProperty(s_property, variable);
    lineEdit->setProperty(s_property, variable);
    layout->addWidget(toolButton);
    toolButton->setEnabled(false);
    connect(toolButton, &QToolButton::clicked, this, [this, variable, lineEdit]() {
        updateSettings(variable, lineEdit->text(), RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo::String);
    });
    connect(lineEdit, &QLineEdit::textChanged, this, [this, toolButton]() {
        toolButton->setEnabled(true);
    });

    mMainLayout->addRow(layout);
}

void SettingsWidgetBase::initializeWidget(QLineEdit *lineEdit, const QMap<QString, QVariant> &mapSettings)
{
    const QString variableName = lineEdit->property(s_property).toString();
    if (mapSettings.contains(variableName)) {
        const auto value = mapSettings.value(variableName);
        lineEdit->setText(value.toString());
    }
}

void SettingsWidgetBase::initializeWidget(QCheckBox *checkbox, const QMap<QString, QVariant> &mapSettings)
{
    const QString variableName = checkbox->property(s_property).toString();
    if (mapSettings.contains(variableName)) {
        const auto value = mapSettings.value(variableName);
        checkbox->setChecked(value.toBool());
    }
}

void SettingsWidgetBase::initializeWidget(QSpinBox *spinbox, const QMap<QString, QVariant> &mapSettings)
{
    const QString variableName = spinbox->property(s_property).toString();
    if (mapSettings.contains(variableName)) {
        const auto value = mapSettings.value(variableName);
        spinbox->setValue(value.toInt());
    }
}
