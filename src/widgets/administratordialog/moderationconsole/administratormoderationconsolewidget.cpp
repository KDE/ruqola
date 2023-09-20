/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratormoderationconsolewidget.h"
#include "connection.h"
#include "moderation/moderationreportsbyusersjob.h"
#include "moderationconsoletreewidget.h"
#include "rocketchataccount.h"

#include <KLocalizedString>
#include <QVBoxLayout>

AdministratorModerationConsoleWidget::AdministratorModerationConsoleWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mModerationConsoleTreeWidget(new ModerationConsoleTreeWidget(account, this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mModerationConsoleTreeWidget->setObjectName(QStringLiteral("mModerationConsoleTreeWidget"));
    mainLayout->addWidget(mModerationConsoleTreeWidget);

    auto moderationJob = new RocketChatRestApi::ModerationReportsByUsersJob(this);
    connect(moderationJob, &RocketChatRestApi::ModerationReportsByUsersJob::moderationReportByUserDone, this, [this](const QJsonObject &obj) {
        qDebug() << " obj" << obj;
        // {\n    \"count\": 0,\n    \"offset\": 0,\n    \"reports\": [\n    ],\n    \"success\": true,\n    \"total\": 0\n}\n"
        // obj QJsonObject({"count":1,"offset":0,"reports":[{"count":1,"isUserDeleted":false,"message":"Fghd","msgId":"eJ443teFnx7hTG5pZ","name":"Laurent
        // m","rooms":[{"_id":"GotJhd87jLScanhwr","fname":"test4","name":"test4","t":"c"}],"ts":"2023-09-20T15:09:37.959Z","userId":"dddd","username":"laurent"}],"success":true,"total":1})
    });
    mRocketChatAccount->restApi()->initializeRestApiJob(moderationJob);
    if (!moderationJob->start()) {
        qDebug() << "Impossible to start ModerationReportsByUsersJob";
    }
}

AdministratorModerationConsoleWidget::~AdministratorModerationConsoleWidget() = default;

#include "moc_administratormoderationconsolewidget.cpp"
