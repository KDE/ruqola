/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "settingswidgetbase.h"

#include "connection.h"
#include "dialogs/confirmpassworddialog.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"

#include <KLineEditEventHandler>
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

using namespace Qt::Literals::StringLiterals;
namespace
{
const char s_property[] = "settings_name";
const char s_property_current_value[] = "current_value";
const char s_property_default_value[] = "default_value";
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
        if (!updateSettings(variable, checked, RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo::ValueType::Boolean)) {
            checkBox->setChecked(!checked);
            Q_EMIT changedCanceled(variable);
        } else {
            Q_EMIT changedDone(variable);
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
    if (obj["success"_L1].toBool()) {
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
    auto applyButton = addApplyButton(variable);
    spinBox->setProperty(s_property, variable);
    layout->addWidget(applyButton);
    applyButton->setEnabled(false);
    setTabOrder(spinBox, applyButton);

    auto cancelButton = addCancelButton(variable);
    layout->addWidget(cancelButton);
    cancelButton->setEnabled(false);
    setTabOrder(applyButton, cancelButton);

    auto restoreToolButton = addRestoreButton(variable);
    layout->addWidget(restoreToolButton);
    setTabOrder(cancelButton, restoreToolButton);

    connect(restoreToolButton, &QToolButton::clicked, this, [variable, spinBox, this]() {
        spinBox->setValue(spinBox->property(s_property_default_value).toInt());
        Q_EMIT changedCanceled(variable);
    });

    connect(cancelButton, &QToolButton::clicked, this, [variable, spinBox, this]() {
        spinBox->setValue(spinBox->property(s_property_current_value).toInt());
        Q_EMIT changedCanceled(variable);
    });

    connect(applyButton, &QToolButton::clicked, this, [this, variable, spinBox, applyButton]() {
        if (!updateSettings(variable,
                            spinBox->value(),
                            RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo::ValueType::Integer,
                            applyButton->objectName())) {
            spinBox->setValue(spinBox->property(s_property_current_value).toInt());
            Q_EMIT changedCanceled(variable);
        }
    });
    connect(this, &SettingsWidgetBase::changedDone, this, [applyButton, spinBox, restoreToolButton, cancelButton](const QString &buttonName) {
        if (applyButton->objectName() == buttonName) {
            applyButton->setEnabled(false);
            restoreToolButton->setEnabled(false);
            cancelButton->setEnabled(false);
            spinBox->setProperty(s_property_current_value, spinBox->value());
        }
    });
    connect(spinBox, &QSpinBox::valueChanged, this, [applyButton, spinBox, restoreToolButton, cancelButton](int value) {
        if (spinBox->property(s_property_default_value).toInt() == value) {
            restoreToolButton->setEnabled(false);
        } else {
            restoreToolButton->setEnabled(true);
        }
        if (spinBox->property(s_property_current_value).toInt() == value) {
            applyButton->setEnabled(false);
            cancelButton->setEnabled(false);
        } else {
            applyButton->setEnabled(true);
            cancelButton->setEnabled(true);
        }
    });

    mMainLayout->addRow(layout);
}

QToolButton *SettingsWidgetBase::addApplyButton(const QString &variable)
{
    auto toolButton = new QToolButton(this);
    toolButton->setObjectName(QStringLiteral("toolbutton_%1").arg(variable));
    toolButton->setIcon(QIcon::fromTheme(QStringLiteral("dialog-ok-apply")));
    toolButton->setToolTip(i18nc("@info:tooltip", "Apply"));
    toolButton->setProperty(s_property, variable);
    return toolButton;
}

QToolButton *SettingsWidgetBase::addCancelButton(const QString &variable)
{
    auto toolButton = new QToolButton(this);
    toolButton->setObjectName(QStringLiteral("cancelToolbutton%1").arg(variable));
    toolButton->setIcon(QIcon::fromTheme(QStringLiteral("dialog-cancel")));
    toolButton->setToolTip(i18nc("@info:tooltip", "Cancel"));
    toolButton->setProperty(s_property, variable);
    return toolButton;
}

QToolButton *SettingsWidgetBase::addRestoreButton(const QString &variable)
{
    auto restoreToolButton = new QToolButton(this);
    restoreToolButton->setToolTip(i18nc("@info:tooltip", "Restore to Default Server Value"));
    restoreToolButton->setIcon(QIcon::fromTheme(QStringLiteral("edit-undo")));
    restoreToolButton->setObjectName(QStringLiteral("restoreToolButton%1").arg(variable));
    restoreToolButton->setProperty(s_property, variable);
    restoreToolButton->setEnabled(false);
    return restoreToolButton;
}

void SettingsWidgetBase::addLineEdit(const QString &labelStr, QLineEdit *lineEdit, const QString &variable, bool readOnly)
{
    auto layout = new QHBoxLayout;
    auto label = new QLabel(labelStr, this);
    label->setObjectName(QStringLiteral("label_%1").arg(variable));
    layout->addWidget(label);
    layout->addWidget(lineEdit);
    auto applyButton = addApplyButton(variable);
    setTabOrder(lineEdit, applyButton);
    lineEdit->setProperty(s_property, variable);
    KLineEditEventHandler::catchReturnKey(lineEdit);
    lineEdit->setReadOnly(readOnly);
    layout->addWidget(applyButton);
    applyButton->setEnabled(false);
    applyButton->setVisible(!readOnly);

    auto cancelButton = addCancelButton(variable);
    layout->addWidget(cancelButton);
    cancelButton->setEnabled(false);
    setTabOrder(applyButton, cancelButton);

    auto restoreToolButton = addRestoreButton(variable);
    layout->addWidget(restoreToolButton);
    setTabOrder(cancelButton, restoreToolButton);

    connect(restoreToolButton, &QToolButton::clicked, this, [variable, lineEdit, this]() {
        lineEdit->setText(lineEdit->property(s_property_default_value).toString());
        Q_EMIT changedCanceled(variable);
    });

    connect(cancelButton, &QToolButton::clicked, this, [variable, lineEdit, this]() {
        lineEdit->setText(lineEdit->property(s_property_current_value).toString());
        Q_EMIT changedCanceled(variable);
    });

    connect(this, &SettingsWidgetBase::changedDone, this, [applyButton, lineEdit, restoreToolButton, cancelButton](const QString &buttonName) {
        if (applyButton->objectName() == buttonName) {
            applyButton->setEnabled(false);
            restoreToolButton->setEnabled(false);
            cancelButton->setEnabled(false);
            lineEdit->setProperty(s_property_current_value, lineEdit->text());
        }
    });
    if (!readOnly) {
        connect(applyButton, &QToolButton::clicked, this, [this, variable, lineEdit, applyButton]() {
            if (!updateSettings(variable,
                                lineEdit->text(),
                                RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo::ValueType::String,
                                applyButton->objectName())) {
                lineEdit->setText(lineEdit->property(s_property_current_value).toString());
                Q_EMIT changedCanceled(variable);
            }
        });
        connect(lineEdit, &QLineEdit::textChanged, this, [applyButton, lineEdit, restoreToolButton, cancelButton](const QString &str) {
            if (lineEdit->property(s_property_default_value).toString() == str) {
                restoreToolButton->setEnabled(false);
            } else {
                restoreToolButton->setEnabled(true);
            }
            if (lineEdit->property(s_property_current_value).toString() == str) {
                applyButton->setEnabled(false);
                cancelButton->setEnabled(false);
            } else {
                applyButton->setEnabled(true);
                cancelButton->setEnabled(true);
            }
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

void SettingsWidgetBase::addPlainTextEdit(const QString &labelStr, QPlainTextEdit *plainTextEdit, const QString &variable)
{
    auto layout = new QHBoxLayout;
    auto label = new QLabel(labelStr, this);
    label->setObjectName(QStringLiteral("label_%1").arg(variable));
    layout->addWidget(label, 0, Qt::AlignTop);
    layout->addWidget(plainTextEdit);
    auto applyButton = addApplyButton(variable);
    plainTextEdit->setProperty(s_property, variable);
    layout->addWidget(applyButton, 0, Qt::AlignTop);
    applyButton->setEnabled(false);
    setTabOrder(plainTextEdit, applyButton);

    auto cancelButton = addCancelButton(variable);
    layout->addWidget(cancelButton);
    cancelButton->setEnabled(false);
    setTabOrder(applyButton, cancelButton);

    auto restoreToolButton = addRestoreButton(variable);
    layout->addWidget(restoreToolButton, 0, Qt::AlignTop);
    setTabOrder(cancelButton, restoreToolButton);

    connect(restoreToolButton, &QToolButton::clicked, this, [variable, plainTextEdit, this]() {
        plainTextEdit->setPlainText(plainTextEdit->property(s_property_default_value).toString());
        Q_EMIT changedCanceled(variable);
    });

    connect(cancelButton, &QToolButton::clicked, this, [variable, plainTextEdit, this]() {
        plainTextEdit->setPlainText(plainTextEdit->property(s_property_current_value).toString());
        Q_EMIT changedCanceled(variable);
    });

    connect(applyButton, &QToolButton::clicked, this, [this, variable, plainTextEdit, applyButton]() {
        if (!updateSettings(variable,
                            plainTextEdit->toPlainText(),
                            RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo::ValueType::String,
                            applyButton->objectName())) {
            plainTextEdit->setPlainText(plainTextEdit->property(s_property_current_value).toString());
            Q_EMIT changedCanceled(variable);
        }
    });
    connect(plainTextEdit, &QPlainTextEdit::textChanged, this, [applyButton, plainTextEdit, cancelButton, restoreToolButton]() {
        if (plainTextEdit->property(s_property_default_value).toString() == plainTextEdit->toPlainText()) {
            restoreToolButton->setEnabled(false);
        } else {
            restoreToolButton->setEnabled(true);
        }
        if (plainTextEdit->toPlainText() != plainTextEdit->property(s_property_current_value).toString()) {
            applyButton->setEnabled(true);
            cancelButton->setEnabled(true);
        } else {
            applyButton->setEnabled(false);
            cancelButton->setEnabled(false);
        }
    });
    connect(this, &SettingsWidgetBase::changedDone, this, [applyButton, plainTextEdit, restoreToolButton, cancelButton](const QString &buttonName) {
        if (applyButton->objectName() == buttonName) {
            plainTextEdit->setProperty(s_property_current_value, plainTextEdit->toPlainText());
            applyButton->setEnabled(false);
            restoreToolButton->setEnabled(true);
            cancelButton->setEnabled(true);
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
    auto applyButton = addApplyButton(variable);
    lineEdit->setProperty(s_property, variable);
    layout->addWidget(applyButton);
    applyButton->setEnabled(false);
    setTabOrder(lineEdit, applyButton);

    auto cancelButton = addCancelButton(variable);
    layout->addWidget(cancelButton);
    cancelButton->setEnabled(false);
    setTabOrder(applyButton, cancelButton);

    connect(applyButton, &QToolButton::clicked, this, [this, variable, lineEdit, applyButton]() {
        if (!updateSettings(variable,
                            lineEdit->password(),
                            RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo::ValueType::String,
                            applyButton->objectName())) {
            Q_EMIT changedCanceled(variable);
        }
    });
    connect(lineEdit, &KPasswordLineEdit::passwordChanged, this, [applyButton]() {
        applyButton->setEnabled(true);
    });
    connect(this, &SettingsWidgetBase::changedDone, this, [applyButton](const QString &buttonName) {
        if (applyButton->objectName() == buttonName) {
            applyButton->setEnabled(false);
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
    auto applyButton = addApplyButton(variable);
    comboBox->setProperty(s_property, variable);
    layout->addWidget(applyButton);
    applyButton->setEnabled(false);
    setTabOrder(comboBox, applyButton);

    auto cancelButton = addCancelButton(variable);
    layout->addWidget(cancelButton);
    cancelButton->setEnabled(false);
    setTabOrder(applyButton, cancelButton);

    auto restoreToolButton = addRestoreButton(variable);
    layout->addWidget(restoreToolButton);
    setTabOrder(cancelButton, restoreToolButton);

    connect(restoreToolButton, &QToolButton::clicked, this, [variable, comboBox, this]() {
        comboBox->setCurrentIndex(comboBox->findData(comboBox->property(s_property_default_value).toString()));
        Q_EMIT changedCanceled(variable);
    });

    connect(cancelButton, &QToolButton::clicked, this, [variable, comboBox, this]() {
        comboBox->setCurrentIndex(comboBox->findData(comboBox->property(s_property_current_value).toString()));
        Q_EMIT changedCanceled(variable);
    });

    connect(applyButton, &QToolButton::clicked, this, [this, variable, comboBox, applyButton]() {
        if (!updateSettings(variable,
                            comboBox->currentData().toString(),
                            RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo::ValueType::String,
                            applyButton->objectName())) {
            comboBox->setCurrentIndex(comboBox->findData(comboBox->property(s_property_current_value).toString()));
            Q_EMIT changedCanceled(variable);
        }
    });
    connect(this, &SettingsWidgetBase::changedDone, this, [applyButton, comboBox, restoreToolButton, cancelButton](const QString &buttonName) {
        if (applyButton->objectName() == buttonName) {
            applyButton->setEnabled(false);
            restoreToolButton->setEnabled(false);
            cancelButton->setEnabled(false);
            comboBox->setProperty(s_property_current_value, comboBox->currentText());
        }
    });
    connect(comboBox, &QComboBox::currentIndexChanged, this, [applyButton, comboBox, restoreToolButton, cancelButton]() {
        if (comboBox->currentIndex() == comboBox->findData(comboBox->property(s_property_default_value).toString())) {
            restoreToolButton->setEnabled(false);
        } else {
            restoreToolButton->setEnabled(true);
        }

        if (comboBox->currentIndex() == comboBox->findData(comboBox->property(s_property_current_value).toString())) {
            applyButton->setEnabled(false);
            cancelButton->setEnabled(false);
        } else {
            applyButton->setEnabled(true);
            cancelButton->setEnabled(true);
        }
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
    lineEdit->setProperty(s_property_current_value, value);
    lineEdit->setProperty(s_property_default_value, defaultValue);
    disableToolButton(variableName, (value != defaultValue));
}

void SettingsWidgetBase::initializeWidget(KPasswordLineEdit *lineEdit, const QMap<QString, QVariant> &mapSettings)
{
    const QString variableName = lineEdit->property(s_property).toString();
    if (mapSettings.contains(variableName)) {
        const auto value = mapSettings.value(variableName);
        lineEdit->setPassword(value.toString());
        disableToolButton(variableName, false);
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
    spinbox->setProperty(s_property_current_value, spinboxValue);
    spinbox->setProperty(s_property_default_value, defaultValue);

    disableToolButton(variableName, (spinboxValue != defaultValue));
}

void SettingsWidgetBase::initializeWidget(QComboBox *comboBox, const QMap<QString, QVariant> &mapSettings, const QString &defaultValue)
{
    const QString variableName = comboBox->property(s_property).toString();
    QString value = defaultValue;
    if (mapSettings.contains(variableName)) {
        value = mapSettings.value(variableName).toString();
    }
    comboBox->setCurrentIndex(comboBox->findData(value));
    comboBox->setProperty(s_property_current_value, value);
    comboBox->setProperty(s_property_default_value, defaultValue);
    disableToolButton(variableName, (value != defaultValue));
}

void SettingsWidgetBase::initializeWidget(QPlainTextEdit *plainTextEdit, const QMap<QString, QVariant> &mapSettings, const QString &defaultValue)
{
    const QString variableName = plainTextEdit->property(s_property).toString();
    QString value = defaultValue;
    if (mapSettings.contains(variableName)) {
        value = mapSettings.value(variableName).toString();
    }
    plainTextEdit->setPlainText(value);
    plainTextEdit->setProperty(s_property_current_value, value);
    plainTextEdit->setProperty(s_property_default_value, defaultValue);
    disableToolButton(variableName, (value != defaultValue));
}

void SettingsWidgetBase::disableToolButton(const QString &variableName, bool differentFromDefaultValue)
{
    auto toolButton = findChild<QToolButton *>(QStringLiteral("toolbutton_%1").arg(variableName));
    if (toolButton) {
        toolButton->setEnabled(false);
    }

    auto restoreToolButton = findChild<QToolButton *>(QStringLiteral("restoreToolButton%1").arg(variableName));
    if (restoreToolButton) {
        restoreToolButton->setEnabled(differentFromDefaultValue);
    }

    auto cancelToolButton = findChild<QToolButton *>(QStringLiteral("cancelToolbutton%1").arg(variableName));
    if (cancelToolButton) {
        cancelToolButton->setEnabled(false);
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
