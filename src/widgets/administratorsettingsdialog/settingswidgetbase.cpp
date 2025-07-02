/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "settingswidgetbase.h"

#include "colorsandmessageviewstyle.h"
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
#include <kcolorscheme.h>

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
    mCurrentWidget->setObjectName(u"mCurrentWidget"_s);
    setWidgetResizable(true);
    setWidget(mCurrentWidget);

    mMainLayout = new QFormLayout(mCurrentWidget);
    mMainLayout->setObjectName(u"mainLayout"_s);
}

SettingsWidgetBase::~SettingsWidgetBase() = default;

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
            job->setAuthMethod(u"password"_s);
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

void SettingsWidgetBase::addCheckBox(QCheckBox *checkBox, const QString &variable)
{
    auto layout = new QHBoxLayout;
    auto applyButton = addApplyButton(variable);
    checkBox->setProperty(s_property, variable);
    layout->addWidget(checkBox);
    auto labelEnterprise = addNotLicenseLabel(variable);
    layout->addWidget(labelEnterprise);

    layout->addWidget(applyButton);
    setTabOrder(checkBox, applyButton);

    auto cancelButton = addCancelButton(variable);
    layout->addWidget(cancelButton);
    setTabOrder(applyButton, cancelButton);

    auto restoreToolButton = addRestoreButton(variable);
    layout->addWidget(restoreToolButton);
    setTabOrder(cancelButton, restoreToolButton);

    connect(restoreToolButton, &QToolButton::clicked, this, [variable, checkBox, this]() {
        checkBox->setChecked(checkBox->property(s_property_default_value).toBool());
        Q_EMIT changedCanceled(variable);
    });

    connect(cancelButton, &QToolButton::clicked, this, [variable, checkBox, this]() {
        checkBox->setChecked(checkBox->property(s_property_current_value).toBool());
        Q_EMIT changedCanceled(variable);
    });
    connect(applyButton, &QToolButton::clicked, this, [this, variable, checkBox, applyButton]() {
        if (!updateSettings(variable,
                            checkBox->isChecked(),
                            RocketChatRestApi::UpdateAdminSettingsJob::UpdateAdminSettingsInfo::ValueType::Boolean,
                            applyButton->objectName())) {
            checkBox->setChecked(checkBox->property(s_property_current_value).toBool());
            Q_EMIT changedCanceled(variable);
        }
    });
    connect(this, &SettingsWidgetBase::changedDone, this, [applyButton, checkBox, restoreToolButton, cancelButton](const QString &buttonName) {
        if (applyButton->objectName() == buttonName) {
            applyButton->setEnabled(false);
            restoreToolButton->setEnabled(false);
            cancelButton->setEnabled(false);
            checkBox->setProperty(s_property_current_value, checkBox->isChecked());
        }
    });
    connect(checkBox, &QCheckBox::toggled, this, [applyButton, checkBox, restoreToolButton, cancelButton](bool value) {
        if (checkBox->property(s_property_default_value).toBool() == value) {
            restoreToolButton->setEnabled(false);
        } else {
            restoreToolButton->setEnabled(true);
        }
        if (checkBox->property(s_property_current_value).toBool() == value) {
            applyButton->setEnabled(false);
            cancelButton->setEnabled(false);
        } else {
            applyButton->setEnabled(true);
            cancelButton->setEnabled(true);
        }
    });

    mMainLayout->addRow(layout);
}

void SettingsWidgetBase::addSpinbox(const QString &labelStr, QSpinBox *spinBox, const QString &variable)
{
    auto layout = new QHBoxLayout;
    auto label = new QLabel(labelStr, this);
    label->setObjectName(u"label_%1"_s.arg(variable));
    layout->addWidget(label);
    layout->addWidget(spinBox);
    auto applyButton = addApplyButton(variable);
    spinBox->setProperty(s_property, variable);
    layout->addWidget(applyButton);
    setTabOrder(spinBox, applyButton);

    auto cancelButton = addCancelButton(variable);
    layout->addWidget(cancelButton);
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

QLabel *SettingsWidgetBase::addNotLicenseLabel(const QString &variable)
{
    auto label = new QLabel(i18nc("@label:textbox", "Enterprise"), this);
    label->setObjectName(u"label_enterprise_%1"_s.arg(variable));
    label->setProperty(s_property, variable);
    label->setToolTip(i18nc("@info:tooltip", "Need to activate enterprise license"));

    const auto color = ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::NegativeText).color();
    QPalette palette = label->palette();
    palette.setColor(QPalette::WindowText, color);
    label->setPalette(palette);

    label->hide();
    return label;
}

QToolButton *SettingsWidgetBase::addApplyButton(const QString &variable)
{
    auto toolButton = new QToolButton(this);
    toolButton->setObjectName(u"toolbutton_%1"_s.arg(variable));
    toolButton->setIcon(QIcon::fromTheme(u"dialog-ok-apply"_s));
    toolButton->setToolTip(i18nc("@info:tooltip", "Apply"));
    toolButton->setProperty(s_property, variable);
    toolButton->setEnabled(false);
    return toolButton;
}

QToolButton *SettingsWidgetBase::addCancelButton(const QString &variable)
{
    auto toolButton = new QToolButton(this);
    toolButton->setObjectName(u"cancelToolbutton%1"_s.arg(variable));
    toolButton->setIcon(QIcon::fromTheme(u"dialog-cancel"_s));
    toolButton->setToolTip(i18nc("@info:tooltip", "Cancel"));
    toolButton->setProperty(s_property, variable);
    toolButton->setEnabled(false);
    return toolButton;
}

QToolButton *SettingsWidgetBase::addRestoreButton(const QString &variable)
{
    auto restoreToolButton = new QToolButton(this);
    restoreToolButton->setToolTip(i18nc("@info:tooltip", "Restore to Default Server Value"));
    restoreToolButton->setIcon(QIcon::fromTheme(u"edit-undo"_s));
    restoreToolButton->setObjectName(u"restoreToolButton%1"_s.arg(variable));
    restoreToolButton->setProperty(s_property, variable);
    restoreToolButton->setEnabled(false);
    return restoreToolButton;
}

void SettingsWidgetBase::addLineEdit(const QString &labelStr, QLineEdit *lineEdit, const QString &variable)
{
    auto layout = new QHBoxLayout;
    auto label = new QLabel(labelStr, this);
    label->setObjectName(u"label_%1"_s.arg(variable));
    layout->addWidget(label);
    layout->addWidget(lineEdit);
    auto applyButton = addApplyButton(variable);
    setTabOrder(lineEdit, applyButton);
    lineEdit->setProperty(s_property, variable);
    KLineEditEventHandler::catchReturnKey(lineEdit);
    layout->addWidget(applyButton);

    auto cancelButton = addCancelButton(variable);
    layout->addWidget(cancelButton);
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

    mMainLayout->addRow(layout);
}

void SettingsWidgetBase::addLabel(const QString &labelStr, QLabel *labelElement, const QString &variable)
{
    auto layout = new QHBoxLayout;
    auto label = new QLabel(labelStr, this);
    label->setObjectName(u"label_%1"_s.arg(variable));
    layout->addWidget(label);
    layout->addWidget(labelElement);
    labelElement->setProperty(s_property, variable);
    mMainLayout->addRow(layout);
}

void SettingsWidgetBase::addPlainTextEdit(const QString &labelStr, QPlainTextEdit *plainTextEdit, const QString &variable)
{
    auto layout = new QHBoxLayout;
    auto label = new QLabel(labelStr, this);
    label->setObjectName(u"label_%1"_s.arg(variable));
    layout->addWidget(label, 0, Qt::AlignTop);
    layout->addWidget(plainTextEdit);
    auto applyButton = addApplyButton(variable);
    plainTextEdit->setProperty(s_property, variable);
    layout->addWidget(applyButton, 0, Qt::AlignTop);
    setTabOrder(plainTextEdit, applyButton);

    auto cancelButton = addCancelButton(variable);
    layout->addWidget(cancelButton, 0, Qt::AlignTop);
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
    label->setObjectName(u"label_%1"_s.arg(variable));
    layout->addWidget(label);
    layout->addWidget(lineEdit);
    auto applyButton = addApplyButton(variable);
    lineEdit->setProperty(s_property, variable);
    layout->addWidget(applyButton);
    setTabOrder(lineEdit, applyButton);

    auto cancelButton = addCancelButton(variable);
    layout->addWidget(cancelButton);
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
    label->setObjectName(u"label_%1"_s.arg(variable));
    layout->addWidget(label);
    layout->addWidget(comboBox);
    fillComboBox(comboBox, items);
    auto applyButton = addApplyButton(variable);
    comboBox->setProperty(s_property, variable);
    layout->addWidget(applyButton);
    setTabOrder(comboBox, applyButton);

    auto cancelButton = addCancelButton(variable);
    layout->addWidget(cancelButton);
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

void SettingsWidgetBase::initializeWidget(QLineEdit *lineEdit, const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings, const QString &defaultValue)
{
    const QString variableName = lineEdit->property(s_property).toString();
    QString value = defaultValue;
    if (mapSettings.contains(variableName)) {
        const auto result = mapSettings.value(variableName);
        value = result.value.toString();
        lineEdit->setText(value);
        lineEdit->setProperty(s_property_current_value, value);
        lineEdit->setProperty(s_property_default_value, defaultValue);
        const bool readOnly = result.readOnly;
        lineEdit->setReadOnly(readOnly);
        disableToolButton(variableName, (value != defaultValue));
        if (readOnly) {
            hideButtons(variableName);
        }
        if (result.enterprise) {
            if (!hasNecessaryLicense(result.modules)) {
                hideButtons(variableName);
                lineEdit->setEnabled(false);
                // TODO show info about Missing license
            }
        }
    }
}

void SettingsWidgetBase::initializeWidget(KPasswordLineEdit *lineEdit, const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings)
{
    const QString variableName = lineEdit->property(s_property).toString();
    if (mapSettings.contains(variableName)) {
        const auto result = mapSettings.value(variableName);
        lineEdit->setPassword(result.value.toString());
        disableToolButton(variableName, false);
        const bool readOnly = result.readOnly;
        if (readOnly) {
            hideButtons(variableName);
        }
        if (result.enterprise) {
            if (!hasNecessaryLicense(result.modules)) {
                hideButtons(variableName);
                lineEdit->setEnabled(false);
                // TODO show info about Missing license
            }
        }
    }
}

void SettingsWidgetBase::initializeWidget(QCheckBox *checkbox, const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings, bool defaultValue)
{
    const QString variableName = checkbox->property(s_property).toString();
    bool value = defaultValue;
    if (mapSettings.contains(variableName)) {
        const auto result = mapSettings.value(variableName);
        value = result.value.toBool();
        const bool readOnly = result.readOnly;
        checkbox->setEnabled(!readOnly);
        if (readOnly) {
            hideButtons(variableName);
        }
        if (result.enterprise) {
            if (!hasNecessaryLicense(result.modules)) {
                hideButtons(variableName);
                checkbox->setEnabled(false);
                showEnterpriseLicense(variableName);
            }
        }
    } else {
        checkbox->setEnabled(false);
    }
    checkbox->setChecked(value);
    checkbox->setProperty(s_property_current_value, value);
    checkbox->setProperty(s_property_default_value, defaultValue);
    disableToolButton(variableName, (value != defaultValue));
}

void SettingsWidgetBase::initializeWidget(QLabel *label, const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings, const QString &defaultValue)
{
    const QString variableName = label->property(s_property).toString();
    QString value = defaultValue;
    if (mapSettings.contains(variableName)) {
        value = mapSettings.value(variableName).value.toString();
    }
    label->setText(value);
}

void SettingsWidgetBase::initializeWidget(QSpinBox *spinbox, const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings, int defaultValue)
{
    const QString variableName = spinbox->property(s_property).toString();
    const bool hasValue = mapSettings.contains(variableName);
    int spinboxValue = defaultValue;
    if (hasValue) {
        const auto result = mapSettings.value(variableName);
        spinboxValue = result.value.toInt();
        const bool readOnly = result.readOnly;
        spinbox->setEnabled(!readOnly);
        if (readOnly) {
            hideButtons(variableName);
        }
        if (result.enterprise) {
            if (!hasNecessaryLicense(result.modules)) {
                hideButtons(variableName);
                spinbox->setEnabled(false);
                // TODO show info about Missing license
            }
        }
    } else {
        spinbox->setEnabled(false);
    }
    spinbox->setValue(spinboxValue);
    spinbox->setProperty(s_property_current_value, spinboxValue);
    spinbox->setProperty(s_property_default_value, defaultValue);
    disableToolButton(variableName, (spinboxValue != defaultValue));
}

void SettingsWidgetBase::initializeWidget(QComboBox *comboBox, const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings, const QString &defaultValue)
{
    const QString variableName = comboBox->property(s_property).toString();
    QString value = defaultValue;
    if (mapSettings.contains(variableName)) {
        value = mapSettings.value(variableName).value.toString();
    }
    comboBox->setCurrentIndex(comboBox->findData(value));
    comboBox->setProperty(s_property_current_value, value);
    comboBox->setProperty(s_property_default_value, defaultValue);
    disableToolButton(variableName, (value != defaultValue));
    const bool readOnly = mapSettings.value(variableName).readOnly;
    comboBox->setEnabled(!readOnly);
    if (readOnly) {
        hideButtons(variableName);
    }
}

void SettingsWidgetBase::initializeWidget(QPlainTextEdit *plainTextEdit,
                                          const QMap<QString, SettingsWidgetBase::SettingsInfo> &mapSettings,
                                          const QString &defaultValue)
{
    const QString variableName = plainTextEdit->property(s_property).toString();
    QString value = defaultValue;
    if (mapSettings.contains(variableName)) {
        value = mapSettings.value(variableName).value.toString();
    }
    plainTextEdit->setPlainText(value);
    plainTextEdit->setProperty(s_property_current_value, value);
    plainTextEdit->setProperty(s_property_default_value, defaultValue);
    disableToolButton(variableName, (value != defaultValue));
    const bool readOnly = mapSettings.value(variableName).readOnly;
    plainTextEdit->setEnabled(!readOnly);
    if (readOnly) {
        hideButtons(variableName);
    }
}

void SettingsWidgetBase::showEnterpriseLicense(const QString &variableName)
{
    auto labelEnterprise = findChild<QLabel *>(u"label_enterprise_%1"_s.arg(variableName));
    if (labelEnterprise) {
        labelEnterprise->setVisible(true);
    }
}

void SettingsWidgetBase::hideButtons(const QString &variableName)
{
    auto toolButton = findChild<QToolButton *>(u"toolbutton_%1"_s.arg(variableName));
    if (toolButton) {
        toolButton->setVisible(false);
    }

    auto restoreToolButton = findChild<QToolButton *>(u"restoreToolButton%1"_s.arg(variableName));
    if (restoreToolButton) {
        restoreToolButton->setVisible(false);
    }

    auto cancelToolButton = findChild<QToolButton *>(u"cancelToolbutton%1"_s.arg(variableName));
    if (cancelToolButton) {
        cancelToolButton->setVisible(false);
    }
}

void SettingsWidgetBase::disableToolButton(const QString &variableName, bool differentFromDefaultValue)
{
    auto toolButton = findChild<QToolButton *>(u"toolbutton_%1"_s.arg(variableName));
    if (toolButton) {
        toolButton->setEnabled(false);
    }

    auto restoreToolButton = findChild<QToolButton *>(u"restoreToolButton%1"_s.arg(variableName));
    if (restoreToolButton) {
        restoreToolButton->setEnabled(differentFromDefaultValue);
    }

    auto cancelToolButton = findChild<QToolButton *>(u"cancelToolbutton%1"_s.arg(variableName));
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
        return mAccount->serverUrl() + u'/' + relativePath;
    }
    return {};
}

QDebug operator<<(QDebug d, const SettingsWidgetBase::SettingsInfo &t)
{
    d.space() << "readOnly:" << t.readOnly;
    d.space() << "enterprise:" << t.enterprise;
    d.space() << "modules:" << t.modules;
    d.space() << "value:" << t.value;
    return d;
}

bool SettingsWidgetBase::hasNecessaryLicense(const QStringList &lst) const
{
    bool hasModules = false;
    for (const QString &m : lst) {
        if (mAccount->hasLicense(m)) {
            hasModules = true;
        } else {
            hasModules = false;
            break;
        }
    }
    return hasModules;
}

#include "moc_settingswidgetbase.cpp"
