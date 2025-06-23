/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingssettingswidget.h"
#include "applictionsettingscustomwidgets.h"

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
    const QJsonObject settings = obj[u"settings"_s].toObject();
    for (const QJsonValue &current : settings) {
        // TODO store json => update it when widget settings change
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
            mCustomWidgets.append(addBooleanSettings(info));
            break;
        }
        case ApplicationsSettingsSettingsInfo::SettingType::String: {
            mCustomWidgets.append(addStringSettings(info));
            break;
        }
        case ApplicationsSettingsSettingsInfo::SettingType::Int: {
            mCustomWidgets.append(addIntSettings(info));
            break;
        }
        case ApplicationsSettingsSettingsInfo::SettingType::Select: {
            mCustomWidgets.append(addSelectSettings(info));
            break;
        }
        }
    }
    mMainLayout->addStretch(1);
    auto hLayout = new QHBoxLayout;
    hLayout->setContentsMargins({});
    hLayout->addStretch(1);
    auto cancelButton = new QPushButton(QIcon::fromTheme(u"dialog-cancel"_s), i18n("Cancel"), this);
    connect(cancelButton, &QPushButton::clicked, this, [this]() {
        Q_EMIT resetValue();
        Q_EMIT dataChanged(false);
    });
    cancelButton->setEnabled(false);
    hLayout->addWidget(cancelButton);

    auto applyButton = new QPushButton(QIcon::fromTheme(u"dialog-ok-apply"_s), i18n("Apply"), this);
    connect(applyButton, &QPushButton::clicked, this, [this]() {
        QJsonArray array;
        for (ApplictionSettingsCustomWidgetsBase *widget : std::as_const(mCustomWidgets)) {
            QJsonObject obj = widget->info().jsonObj();
            obj["value"_L1] = widget->value();
            array.append(obj);
        }
        auto job = new RocketChatRestApi::AppUpdateInfoJob(this);
        RocketChatRestApi::AppUpdateInfoJob::AppUpdateInfo info;
        info.mAppInfoType = RocketChatRestApi::AppUpdateInfoJob::AppInfoType::Settings;
        info.mAppMode = RocketChatRestApi::AppUpdateInfoJob::AppMode::Post;
        info.mAppsId = mAppId;
        info.mAppsSettings = array;
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
    if (id.isEmpty()) {
        return {};
    }
    const QString translatedString = mRocketChatAccount->getTranslatedIdentifier(lang, u"app-"_s + QString::fromLatin1(mAppId) + u'.' + id);
    if (translatedString.isEmpty()) {
        qCWarning(RUQOLAWIDGETS_LOG) << " Translated string not found: " << QString::fromLatin1(mAppId) + id;
        return id;
    }
    return translatedString;
}

ApplictionSettingsCustomWidgetsBase *ApplicationsSettingsSettingsWidget::addBooleanSettings(const ApplicationsSettingsSettingsInfo &info)
{
    auto checkBoxWidget = new ApplictionSettingsCustomWidgetsCheckBox(mAppId, mRocketChatAccount, info, this);
    connect(checkBoxWidget, &ApplictionSettingsCustomWidgetsBase::dataChanged, this, [this]() {
        Q_EMIT dataChanged(true);
    });
    connect(this, &ApplicationsSettingsSettingsWidget::resetValue, this, [checkBoxWidget]() {
        Q_EMIT checkBoxWidget->resetValue();
    });
    mMainLayout->addWidget(checkBoxWidget);
    return checkBoxWidget;
}

ApplictionSettingsCustomWidgetsBase *ApplicationsSettingsSettingsWidget::addStringSettings(const ApplicationsSettingsSettingsInfo &info)
{
    auto stringWidget = new ApplictionSettingsCustomWidgetsString(mAppId, mRocketChatAccount, info, this);
    connect(stringWidget, &ApplictionSettingsCustomWidgetsBase::dataChanged, this, [this]() {
        Q_EMIT dataChanged(true);
    });
    connect(this, &ApplicationsSettingsSettingsWidget::resetValue, this, [stringWidget]() {
        Q_EMIT stringWidget->resetValue();
    });
    mMainLayout->addWidget(stringWidget);
    return stringWidget;
}

ApplictionSettingsCustomWidgetsBase *ApplicationsSettingsSettingsWidget::addIntSettings(const ApplicationsSettingsSettingsInfo &info)
{
    auto spinboxWidget = new ApplictionSettingsCustomWidgetsSpinBox(mAppId, mRocketChatAccount, info, this);
    connect(spinboxWidget, &ApplictionSettingsCustomWidgetsBase::dataChanged, this, [this]() {
        Q_EMIT dataChanged(true);
    });
    connect(this, &ApplicationsSettingsSettingsWidget::resetValue, this, [spinboxWidget]() {
        Q_EMIT spinboxWidget->resetValue();
    });
    mMainLayout->addWidget(spinboxWidget);
    return spinboxWidget;
}

ApplictionSettingsCustomWidgetsBase *ApplicationsSettingsSettingsWidget::addSelectSettings(const ApplicationsSettingsSettingsInfo &info)
{
    auto comboboxWidget = new ApplictionSettingsCustomWidgetsComboBox(mAppId, mRocketChatAccount, info, this);
    connect(comboboxWidget, &ApplictionSettingsCustomWidgetsBase::dataChanged, this, [this]() {
        Q_EMIT dataChanged(true);
    });
    connect(this, &ApplicationsSettingsSettingsWidget::resetValue, this, [comboboxWidget]() {
        Q_EMIT comboboxWidget->resetValue();
    });
    mMainLayout->addWidget(comboboxWidget);
    return comboboxWidget;
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
