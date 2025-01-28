/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingssettingswidget.h"

#include "apps/appinfojob.h"
#include "connection.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QShowEvent>
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
    QList<ApplicationsSettingsSettingsInfo> infos;
    const QJsonObject settings = obj[QStringLiteral("settings")].toObject();
    for (const QJsonValue &current : settings) {
        ApplicationsSettingsSettingsInfo i;
        i.parseSettings(current.toObject());
        infos.append(std::move(i));
    }
    generateSettingsWidgets(infos);
}

void ApplicationsSettingsSettingsWidget::generateSettingsWidgets(const QList<ApplicationsSettingsSettingsInfo> &infos)
{
    for (const ApplicationsSettingsSettingsInfo &info : infos) {
        switch (info.settingType()) {
        case ApplicationsSettingsSettingsInfo::SettingType::Boolean: {
            addBooleanSettings(info);
            break;
        }
        case ApplicationsSettingsSettingsInfo::SettingType::String: {
            addStringSettings(info);
            break;
        }
        default:
            break;
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
        // TODO apply value
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
    const auto defaultValue = info.packageValue() == "true"_L1 ? Qt::Checked : Qt::Unchecked;
    checkBox->setCheckState(defaultValue);
    connect(checkBox, &QCheckBox::clicked, this, [this]() {
        Q_EMIT dataChanged(true);
    });
    connect(this, &ApplicationsSettingsSettingsWidget::resetValue, this, [checkBox, defaultValue]() {
        QSignalBlocker b(checkBox);
        checkBox->setCheckState(defaultValue);
    });
    mMainLayout->addLayout(hbox);
}

void ApplicationsSettingsSettingsWidget::addStringSettings(const ApplicationsSettingsSettingsInfo &info)
{
    const QString lang = QLocale().name();
    QHBoxLayout *hbox = new QHBoxLayout;
    auto label = new QLabel(getTranslatedIdentifier(lang, info.i18nLabel()), this);
    label->setToolTip(getTranslatedIdentifier(lang, info.i18nDescription()));
    hbox->addWidget(label);

    auto lineEdit = new QLineEdit(this);
    lineEdit->setObjectName(info.id());
    hbox->addWidget(lineEdit);
    const QString defaultValue = info.packageValue();
    lineEdit->setText(defaultValue);
    connect(lineEdit, &QLineEdit::textChanged, this, [this]() {
        Q_EMIT dataChanged(true);
    });
    connect(this, &ApplicationsSettingsSettingsWidget::resetValue, this, [lineEdit, defaultValue]() {
        QSignalBlocker b(lineEdit);
        lineEdit->setText(defaultValue);
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
