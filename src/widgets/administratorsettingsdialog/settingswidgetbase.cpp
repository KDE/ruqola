/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "settingswidgetbase.h"
#include "connection.h"
#include "dialogs/confirmpassworddialog.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"

#include <KLocalizedString>

#include <KPasswordLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
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
    connect(checkBox, &QCheckBox::clicked, this, [this, variable, checkBox](bool checked) {
        if (!updateSettings(variable, checked, RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo::Boolean)) {
            checkBox->setChecked(!checked);
        }
    });
}

bool SettingsWidgetBase::updateSettings(const QString &settingName,
                                        const QVariant &value,
                                        RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo::ValueType typeValue,
                                        const QString &buttonObjectName)
{
    bool status = false;
    if (mAccount) {
        QString password;
        QPointer<ConfirmPasswordDialog> dialog(new ConfirmPasswordDialog(this));
        if (dialog->exec()) {
            password = dialog->password();
            auto job = new RocketChatRestApi::UpdateAdminSettingsJob(this);
            RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo info;
            info.settingsValue = value;
            info.settingName = settingName;
            info.valueType = typeValue;
            job->setUpdateAdminSettingsInfo(info);
            job->setAuthMethod(QStringLiteral("password"));
            job->setAuthCode(QString::fromLatin1(Utils::convertSha256Password(password)));
            mAccount->restApi()->initializeRestApiJob(job);
            connect(job, &RocketChatRestApi::UpdateAdminSettingsJob::updateAdminSettingsDone, this, [this, buttonObjectName](const QJsonObject &obj) {
                slotAdminSettingsDone(obj, buttonObjectName);
            });
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start UpdateAdminSettingsJob job";
                status = false;
            } else {
                status = true;
            }
        } else {
            status = false;
        }
        delete dialog;
    }
    return status;
}

void SettingsWidgetBase::slotAdminSettingsDone(const QJsonObject &obj, const QString &buttonObjectName)
{
    qDebug() << "AccountSettingsWidget::slotAdminSettingsDone " << obj;
    if (obj[QLatin1String("success")].toBool()) {
        // Disable apply button
        if (!buttonObjectName.isEmpty()) {
            Q_EMIT changedDone(buttonObjectName);
        }
    } else {
        // Failed
    }
}

void SettingsWidgetBase::addSpinbox(const QString &labelStr, QSpinBox *spinBox, const QString &variable)
{
    auto layout = new QHBoxLayout;
    auto label = new QLabel(labelStr, this);
    label->setObjectName(QStringLiteral("label_%1").arg(variable));
    layout->addWidget(label);
    layout->addWidget(spinBox);
    auto toolButton = new QToolButton(this);
    toolButton->setObjectName(QStringLiteral("toolbutton_%1").arg(variable));
    toolButton->setText(i18n("Apply"));
    toolButton->setProperty(s_property, variable);
    spinBox->setProperty(s_property, variable);
    layout->addWidget(toolButton);
    toolButton->setEnabled(false);
    connect(toolButton, &QToolButton::clicked, this, [this, variable, spinBox, toolButton]() {
        updateSettings(variable, spinBox->value(), RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo::Integer, toolButton->objectName());
    });
    connect(this, &SettingsWidgetBase::changedDone, this, [toolButton](const QString &buttonName) {
        if (toolButton->objectName() == buttonName) {
            toolButton->setEnabled(false);
        }
    });
    connect(spinBox, &QSpinBox::valueChanged, this, [toolButton]() {
        toolButton->setEnabled(true);
    });

    mMainLayout->addRow(layout);
}

void SettingsWidgetBase::addLineEdit(const QString &labelStr, QLineEdit *lineEdit, const QString &variable, bool readOnly)
{
    auto layout = new QHBoxLayout;
    auto label = new QLabel(labelStr, this);
    label->setObjectName(QStringLiteral("label_%1").arg(variable));
    layout->addWidget(label);
    layout->addWidget(lineEdit);
    auto toolButton = new QToolButton(this);
    toolButton->setObjectName(QStringLiteral("toolbutton_%1").arg(variable));
    toolButton->setText(i18n("Apply"));
    toolButton->setProperty(s_property, variable);
    lineEdit->setProperty(s_property, variable);
    lineEdit->setReadOnly(readOnly);
    layout->addWidget(toolButton);
    toolButton->setEnabled(false);
    toolButton->setVisible(!readOnly);
    connect(this, &SettingsWidgetBase::changedDone, this, [toolButton](const QString &buttonName) {
        if (toolButton->objectName() == buttonName) {
            toolButton->setEnabled(false);
        }
    });
    if (!readOnly) {
        connect(toolButton, &QToolButton::clicked, this, [this, variable, lineEdit, toolButton]() {
            updateSettings(variable, lineEdit->text(), RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo::String, toolButton->objectName());
        });
        connect(lineEdit, &QLineEdit::textChanged, this, [toolButton]() {
            toolButton->setEnabled(true);
        });
    }

    mMainLayout->addRow(layout);
}

void SettingsWidgetBase::addLabel(const QString &labelStr, QLabel *labelElement, const QString &variable)
{
    auto layout = new QHBoxLayout;
    auto label = new QLabel(labelStr, this);
    label->setObjectName(QStringLiteral("label_%1").arg(variable));
    layout->addWidget(label);
    layout->addWidget(labelElement);
    labelElement->setProperty(s_property, variable);
    mMainLayout->addRow(layout);
}

void SettingsWidgetBase::addPlainTextEdit(const QString &labelStr, QPlainTextEdit *lineEdit, const QString &variable)
{
    auto layout = new QHBoxLayout;
    auto label = new QLabel(labelStr, this);
    label->setObjectName(QStringLiteral("label_%1").arg(variable));
    layout->addWidget(label, 0, Qt::AlignTop);
    layout->addWidget(lineEdit);
    auto toolButton = new QToolButton(this);
    toolButton->setObjectName(QStringLiteral("toolbutton_%1").arg(variable));
    toolButton->setText(i18n("Apply"));
    toolButton->setProperty(s_property, variable);
    lineEdit->setProperty(s_property, variable);
    layout->addWidget(toolButton, 0, Qt::AlignTop);
    toolButton->setEnabled(false);
    connect(toolButton, &QToolButton::clicked, this, [this, variable, lineEdit, toolButton]() {
        updateSettings(variable, lineEdit->toPlainText(), RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo::String, toolButton->objectName());
    });
    connect(lineEdit, &QPlainTextEdit::textChanged, this, [toolButton]() {
        toolButton->setEnabled(true);
    });
    connect(this, &SettingsWidgetBase::changedDone, this, [toolButton](const QString &buttonName) {
        if (toolButton->objectName() == buttonName) {
            toolButton->setEnabled(false);
        }
    });

    mMainLayout->addRow(layout);
}

void SettingsWidgetBase::addPasswordEdit(const QString &labelStr, KPasswordLineEdit *lineEdit, const QString &variable)
{
    auto layout = new QHBoxLayout;
    auto label = new QLabel(labelStr, this);
    label->setObjectName(QStringLiteral("label_%1").arg(variable));
    layout->addWidget(label);
    layout->addWidget(lineEdit);
    auto toolButton = new QToolButton(this);
    toolButton->setObjectName(QStringLiteral("toolbutton_%1").arg(variable));
    toolButton->setText(i18n("Apply"));
    toolButton->setProperty(s_property, variable);
    lineEdit->setProperty(s_property, variable);
    layout->addWidget(toolButton);
    toolButton->setEnabled(false);
    connect(toolButton, &QToolButton::clicked, this, [this, variable, lineEdit, toolButton]() {
        updateSettings(variable, lineEdit->password(), RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo::String, toolButton->objectName());
    });
    connect(lineEdit, &KPasswordLineEdit::passwordChanged, this, [toolButton]() {
        toolButton->setEnabled(true);
    });
    connect(this, &SettingsWidgetBase::changedDone, this, [toolButton](const QString &buttonName) {
        if (toolButton->objectName() == buttonName) {
            toolButton->setEnabled(false);
        }
    });

    mMainLayout->addRow(layout);
}

void SettingsWidgetBase::fillComboBox(QComboBox *comboBox, const QMap<QString, QString> &items)
{
    QMapIterator<QString, QString> i(items);
    while (i.hasNext()) {
        i.next();
        comboBox->addItem(i.value(), i.key());
    }
}

void SettingsWidgetBase::addComboBox(const QString &labelStr, const QMap<QString, QString> &items, QComboBox *comboBox, const QString &variable)
{
    auto layout = new QHBoxLayout;
    auto label = new QLabel(labelStr, this);
    label->setObjectName(QStringLiteral("label_%1").arg(variable));
    layout->addWidget(label);
    layout->addWidget(comboBox);
    fillComboBox(comboBox, items);
    auto toolButton = new QToolButton(this);
    toolButton->setObjectName(QStringLiteral("toolbutton_%1").arg(variable));
    toolButton->setText(i18n("Apply"));
    toolButton->setProperty(s_property, variable);
    comboBox->setProperty(s_property, variable);
    layout->addWidget(toolButton);
    toolButton->setEnabled(false);
    connect(toolButton, &QToolButton::clicked, this, [this, variable, comboBox, toolButton]() {
        updateSettings(variable,
                       comboBox->currentData().toString(),
                       RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo::String,
                       toolButton->objectName());
    });
    connect(this, &SettingsWidgetBase::changedDone, this, [toolButton](const QString &buttonName) {
        if (toolButton->objectName() == buttonName) {
            toolButton->setEnabled(false);
        }
    });
    connect(comboBox, &QComboBox::currentIndexChanged, this, [toolButton]() {
        toolButton->setEnabled(true);
    });

    mMainLayout->addRow(layout);
}

void SettingsWidgetBase::initializeWidget(QLineEdit *lineEdit, const QMap<QString, QVariant> &mapSettings, const QString &defaultValue)
{
    const QString variableName = lineEdit->property(s_property).toString();
    QString value = defaultValue;
    if (mapSettings.contains(variableName)) {
        value = mapSettings.value(variableName).toString();
    }
    lineEdit->setText(value);
    disableTooButton(variableName);
}

void SettingsWidgetBase::initializeWidget(KPasswordLineEdit *lineEdit, const QMap<QString, QVariant> &mapSettings)
{
    const QString variableName = lineEdit->property(s_property).toString();
    if (mapSettings.contains(variableName)) {
        const auto value = mapSettings.value(variableName);
        lineEdit->setPassword(value.toString());
        disableTooButton(variableName);
    }
}

void SettingsWidgetBase::initializeWidget(QCheckBox *checkbox, const QMap<QString, QVariant> &mapSettings, bool defaultValue)
{
    const QString variableName = checkbox->property(s_property).toString();
    if (mapSettings.contains(variableName)) {
        const auto value = mapSettings.value(variableName);
        checkbox->setChecked(value.toBool());
    } else {
        checkbox->setChecked(defaultValue);
    }
}

void SettingsWidgetBase::initializeWidget(QLabel *label, const QMap<QString, QVariant> &mapSettings, const QString &defaultValue)
{
    const QString variableName = label->property(s_property).toString();
    QString value = defaultValue;
    if (mapSettings.contains(variableName)) {
        value = mapSettings.value(variableName).toString();
    }
    label->setText(value);
}

void SettingsWidgetBase::initializeWidget(QSpinBox *spinbox, const QMap<QString, QVariant> &mapSettings, int defaultValue)
{
    const QString variableName = spinbox->property(s_property).toString();
    const bool hasValue = mapSettings.contains(variableName);
    int spinboxValue = defaultValue;
    if (hasValue) {
        spinboxValue = mapSettings.value(variableName).toInt();
    }
    spinbox->setValue(spinboxValue);
    disableTooButton(variableName);
}

void SettingsWidgetBase::initializeWidget(QComboBox *comboBox, const QMap<QString, QVariant> &mapSettings, const QString &defaultValue)
{
    const QString variableName = comboBox->property(s_property).toString();
    QString value = defaultValue;
    if (mapSettings.contains(variableName)) {
        value = mapSettings.value(variableName).toString();
    }
    comboBox->setCurrentIndex(comboBox->findData(value));
    disableTooButton(variableName);
}

void SettingsWidgetBase::initializeWidget(QPlainTextEdit *plainTextEdit, const QMap<QString, QVariant> &mapSettings, const QString &defaultValue)
{
    const QString variableName = plainTextEdit->property(s_property).toString();
    QString value = defaultValue;
    if (mapSettings.contains(variableName)) {
        value = mapSettings.value(variableName).toString();
    }
    plainTextEdit->setPlainText(value);
    disableTooButton(variableName);
}

void SettingsWidgetBase::disableTooButton(const QString &variableName)
{
    auto toolButton = findChild<QToolButton *>(QStringLiteral("toolbutton_%1").arg(variableName));
    if (toolButton) {
        toolButton->setEnabled(false);
    }
}

QLabel *SettingsWidgetBase::createBoldLabel(const QString &text)
{
    auto label = new QLabel(text, this);
    QFont apiRateLimiterLabelFont = label->font();
    apiRateLimiterLabelFont.setBold(true);
    label->setFont(apiRateLimiterLabelFont);
    return label;
}

QString SettingsWidgetBase::urlFromRelativePath(const QString &relativePath)
{
    if (mAccount) {
        return mAccount->serverUrl() + QLatin1Char('/') + relativePath;
    }
    return {};
}

#include "moc_settingswidgetbase.cpp"
