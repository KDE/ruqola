/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingssettingswidget.h"

#include "apps/appinfojob.h"
#include "apps/appupdateinfojob.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QShowEvent>
#include <QSpinBox>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsSettingsWidget::ApplicationsSettingsSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
    , mMainLayout(new QVBoxLayout(this))
{
    mMainLayout->setObjectName("mainLayout"_L1);
}

ApplicationsSettingsSettingsWidget::~ApplicationsSettingsSettingsWidget() = default;

void ApplicationsSettingsSettingsWidget::showEvent(QShowEvent *event)
{
    if (!event->spontaneous() && !mWasInitialized) {
        mWasInitialized = true;
        initialize();
    }
    QWidget::showEvent(event);
}

void ApplicationsSettingsSettingsWidget::generateSettings(const QJsonObject &obj)
{
    const QJsonObject settings = obj[QStringLiteral("settings")].toObject();
    for (const QJsonValue &current : settings) {
        ApplicationsSettingsSettingsInfo i;
        i.parseSettings(current.toObject());
        mSettingsInfo.append(std::move(i));
    }
    if (mSettingsInfo.isEmpty()) {
        auto label = new QLabel(i18n("Application does not have settings."), this);
        QFont f = label->font();
        f.setBold(true);
        f.setItalic(true);
        label->setFont(f);
        mMainLayout->addWidget(label);
        mMainLayout->addStretch(1);
    } else {
        generateSettingsWidgets(mSettingsInfo);
    }
}

void ApplicationsSettingsSettingsWidget::generateSettingsWidgets(const QList<ApplicationsSettingsSettingsInfo> &infos)
{
    for (const ApplicationsSettingsSettingsInfo &info : infos) {
        switch (info.settingType()) {
        case ApplicationsSettingsSettingsInfo::SettingType::Unknown: {
            break;
        }
        case ApplicationsSettingsSettingsInfo::SettingType::Boolean: {
            addBooleanSettings(info);
            break;
        }
        case ApplicationsSettingsSettingsInfo::SettingType::String: {
            addStringSettings(info);
            break;
        }
        case ApplicationsSettingsSettingsInfo::SettingType::Int: {
            addIntSettings(info);
            break;
        }
        case ApplicationsSettingsSettingsInfo::SettingType::Select: {
            addSelectSettings(info);
            break;
        }
        }
    }
    mMainLayout->addStretch(1);
    // TODO show or not
    auto hLayout = new QHBoxLayout;
    hLayout->setContentsMargins({});
    hLayout->addStretch(1);
    auto cancelButton = new QPushButton(i18n("Cancel"), this);
    connect(cancelButton, &QPushButton::clicked, this, [this]() {
        Q_EMIT resetValue();
        Q_EMIT dataChanged(false);
    });
    cancelButton->setEnabled(false);
    hLayout->addWidget(cancelButton);

    auto applyButton = new QPushButton(i18n("Apply"), this);
    connect(applyButton, &QPushButton::clicked, this, [this]() {
        auto job = new RocketChatRestApi::AppUpdateInfoJob(this);
        RocketChatRestApi::AppUpdateInfoJob::AppUpdateInfo info;
        info.mAppInfoType = RocketChatRestApi::AppUpdateInfoJob::AppInfoType::Apps;
        info.mAppMode = RocketChatRestApi::AppUpdateInfoJob::AppMode::Post;
        info.mAppsId = mAppId;
        job->setAppUpdateInfo(info);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start appUpdateInfoDone";
        }
    });
    applyButton->setEnabled(false);
    hLayout->addWidget(applyButton);

    mMainLayout->addLayout(hLayout);
    connect(this, &ApplicationsSettingsSettingsWidget::dataChanged, this, [cancelButton, applyButton](bool status) {
        applyButton->setEnabled(status);
        cancelButton->setEnabled(status);
    });
}

QString ApplicationsSettingsSettingsWidget::getTranslatedIdentifier(const QString &lang, const QString &id) const
{
    const QString translatedString =
        mRocketChatAccount->getTranslatedIdentifier(lang, QStringLiteral("app-") + QString::fromLatin1(mAppId) + QLatin1Char('.') + id);
    if (translatedString.isEmpty()) {
        qCWarning(RUQOLAWIDGETS_LOG) << " Translated string not found: " << QString::fromLatin1(mAppId) + id;
        return id;
    }
    return translatedString;
}

void ApplicationsSettingsSettingsWidget::addBooleanSettings(const ApplicationsSettingsSettingsInfo &info)
{
    const QString lang = QLocale().name();
    QHBoxLayout *hbox = new QHBoxLayout;
    auto checkBox = new QCheckBox(getTranslatedIdentifier(lang, info.i18nLabel()), this);
    checkBox->setObjectName(info.id());
    checkBox->setToolTip(getTranslatedIdentifier(lang, info.i18nDescription()));
    hbox->addWidget(checkBox);
    const QVariant r = info.value().isValid() ? info.value() : info.packageValue();
    checkBox->setCheckState(r.toBool() ? Qt::Checked : Qt::Unchecked);

    connect(checkBox, &QCheckBox::clicked, this, [this]() {
        Q_EMIT dataChanged(true);
    });
    connect(this, &ApplicationsSettingsSettingsWidget::resetValue, this, [checkBox, r]() {
        QSignalBlocker b(checkBox);
        checkBox->setCheckState(r.toBool() ? Qt::Checked : Qt::Unchecked);
    });
    mMainLayout->addLayout(hbox);
}

void ApplicationsSettingsSettingsWidget::addStringSettings(const ApplicationsSettingsSettingsInfo &info)
{
    const QString lang = QLocale().name();
    QHBoxLayout *hbox = new QHBoxLayout;
    auto label = new QLabel(getTranslatedIdentifier(lang, info.i18nLabel()), this);
    label->setToolTip(getTranslatedIdentifier(lang, info.i18nDescription()));
    hbox->addWidget(label, 0, info.multiLine() ? Qt::AlignTop : Qt::Alignment());
    if (info.multiLine()) {
        auto plainTextEdit = new QPlainTextEdit(this);
        plainTextEdit->setObjectName(info.id());
        hbox->addWidget(plainTextEdit);
        const QVariant r = info.value().isValid() ? info.value() : info.packageValue();
        plainTextEdit->setPlainText(r.toString());

        connect(plainTextEdit, &QPlainTextEdit::textChanged, this, [this]() {
            Q_EMIT dataChanged(true);
        });
        connect(this, &ApplicationsSettingsSettingsWidget::resetValue, this, [plainTextEdit, r]() {
            QSignalBlocker b(plainTextEdit);
            plainTextEdit->setPlainText(r.toString());
        });
    } else {
        auto lineEdit = new QLineEdit(this);
        lineEdit->setObjectName(info.id());
        hbox->addWidget(lineEdit);
        const QVariant r = info.value().isValid() ? info.value() : info.packageValue();
        lineEdit->setText(r.toString());

        connect(lineEdit, &QLineEdit::textChanged, this, [this]() {
            Q_EMIT dataChanged(true);
        });
        connect(this, &ApplicationsSettingsSettingsWidget::resetValue, this, [lineEdit, r]() {
            QSignalBlocker b(lineEdit);
            lineEdit->setText(r.toString());
        });
    }
    mMainLayout->addLayout(hbox);
}

void ApplicationsSettingsSettingsWidget::addIntSettings(const ApplicationsSettingsSettingsInfo &info)
{
    const QString lang = QLocale().name();
    QHBoxLayout *hbox = new QHBoxLayout;
    auto label = new QLabel(getTranslatedIdentifier(lang, info.i18nLabel()), this);
    hbox->addWidget(label);
    auto spinbox = new QSpinBox(this);
    spinbox->setMaximum(999999999);
    spinbox->setObjectName(info.id());
    spinbox->setToolTip(getTranslatedIdentifier(lang, info.i18nDescription()));
    hbox->addWidget(spinbox);
    const QVariant r = info.value().isValid() ? info.value() : info.packageValue();
    spinbox->setValue(r.toInt());
    connect(spinbox, &QSpinBox::valueChanged, this, [this]() {
        Q_EMIT dataChanged(true);
    });
    connect(this, &ApplicationsSettingsSettingsWidget::resetValue, this, [spinbox, r]() {
        QSignalBlocker b(spinbox);
        spinbox->setValue(r.toInt());
    });
    mMainLayout->addLayout(hbox);
}

void ApplicationsSettingsSettingsWidget::addSelectSettings(const ApplicationsSettingsSettingsInfo &info)
{
    const QString lang = QLocale().name();
    QHBoxLayout *hbox = new QHBoxLayout;
    auto label = new QLabel(getTranslatedIdentifier(lang, info.i18nLabel()), this);
    hbox->addWidget(label);
    auto combobox = new QComboBox(this);
    combobox->setObjectName(info.id());
    combobox->setToolTip(getTranslatedIdentifier(lang, info.i18nDescription()));
    hbox->addWidget(combobox);
    // Fill Combobox
    QMapIterator<QString, QString> i(info.values());
    while (i.hasNext()) {
        i.next();
        combobox->addItem(getTranslatedIdentifier(lang, i.value()), i.key());
    }
    const QVariant r = info.value().isValid() ? info.value() : info.packageValue();
    combobox->setCurrentIndex(combobox->findData(r.toString()));
    connect(combobox, &QComboBox::currentIndexChanged, this, [this]() {
        Q_EMIT dataChanged(true);
    });
    connect(this, &ApplicationsSettingsSettingsWidget::resetValue, this, [combobox, r]() {
        QSignalBlocker b(combobox);
        combobox->setCurrentIndex(combobox->findData(r.toString()));
    });
    mMainLayout->addLayout(hbox);
}

void ApplicationsSettingsSettingsWidget::initialize()
{
    if (mRocketChatAccount) {
        auto job = new RocketChatRestApi::AppInfoJob(this);
        job->setAppsId(mAppId);
        job->setAppInfoType(RocketChatRestApi::AppInfoJob::AppInfoType::Settings);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        connect(job, &RocketChatRestApi::AppInfoJob::appInfoDone, this, &ApplicationsSettingsSettingsWidget::generateSettings);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start AppInfoJob";
        }
    }
}

void ApplicationsSettingsSettingsWidget::setApplicationId(const QByteArray &appId)
{
    mAppId = appId;
}

#include "moc_applicationssettingssettingswidget.cpp"
