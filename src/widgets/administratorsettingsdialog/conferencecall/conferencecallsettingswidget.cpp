/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "conferencecallsettingswidget.h"

#include "connection.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "video-conference/videoconferenceprovidersjob.h"
#include <KLocalizedString>
#include <QComboBox>
#include <QFormLayout>
#include <QJsonArray>

using namespace Qt::Literals::StringLiterals;
ConferenceCallSettingsWidget::ConferenceCallSettingsWidget(RocketChatAccount *account, QWidget *parent)
    : SettingsWidgetBase{account, parent}
    , mDefaultProvider(new QComboBox(this))
{
    mDefaultProvider->setObjectName(QStringLiteral("mDefaultProvider"));
    addComboBox(i18n("Default Provider"), {}, mDefaultProvider, QStringLiteral("VideoConf_Default_Provider"));
}

ConferenceCallSettingsWidget::~ConferenceCallSettingsWidget() = default;

void ConferenceCallSettingsWidget::initialize(const QMap<QString, QVariant> &mapSettings)
{
    auto job = new RocketChatRestApi::VideoConferenceProvidersJob(this);
    mAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::VideoConferenceProvidersJob::videoConferenceProvidersDone, this, [this, mapSettings](const QJsonObject &obj) {
        // {"data":[{"key":"jitsi","label":"Jitsi"}],"success":true}
        QMap<QString, QString> maps;
        const QJsonArray array = obj["data"_L1].toArray();
        for (const QJsonValue &current : array) {
            const QJsonObject roleObject = current.toObject();
            maps.insert(roleObject["key"_L1].toString(), roleObject["label"_L1].toString());
        }
        // qDebug() << " list " << obj << " maps " << maps;
        fillComboBox(mDefaultProvider, maps);
        initializeWidget(mDefaultProvider, mapSettings, QString());
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start VideoConferenceProvidersJob job";
    }
}

#include "moc_conferencecallsettingswidget.cpp"
