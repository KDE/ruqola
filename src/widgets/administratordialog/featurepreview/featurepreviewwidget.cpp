/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "featurepreviewwidget.h"
#include "connection.h"
#include "misc/methodcalljob.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
FeaturePreviewWidget::FeaturePreviewWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mAllowFeaturePreview(new QCheckBox(i18nc("@option:check", "Allow Feature Preview"), this))
    , mQuickReactions(new QCheckBox(i18nc("@option:check", "Quick Reactions"), this))
    , mTimestampInMessages(new QCheckBox(i18nc("@option:check", "Timestamp in Messages"), this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mAllowFeaturePreview->setObjectName(u"mAllowFeaturePreview"_s);
    mainLayout->addWidget(mAllowFeaturePreview);

    mQuickReactions->setObjectName(u"mQuickReactions"_s);
    mainLayout->addWidget(mQuickReactions);

    mTimestampInMessages->setObjectName(u"mTimestampInMessages"_s);
    mainLayout->addWidget(mTimestampInMessages);
    // Move in stable in RC 8.0.0
    if (account && account->hasAtLeastVersion(8, 0, 0)) {
        mQuickReactions->hide();
        mTimestampInMessages->hide();
    }

    mainLayout->addStretch(1);
}

FeaturePreviewWidget::~FeaturePreviewWidget() = default;

void FeaturePreviewWidget::initialize()
{
    // TODO mAllowFeaturePreview->setChecked...
}

void FeaturePreviewWidget::save()
{
#if 0
    auto job = new RocketChatRestApi::MethodCallJob(this);
    const QJsonArray params{{QString::fromLatin1(fileId)}};
    const QString methodName{u"saveSettings"_s};
    const RocketChatRestApi::MethodCallJob::MethodCallJobInfo info{
        .messageObj = mRocketChatAccount->ddp()->generateJsonObject(methodName, params),
        .methodName = methodName,
        .anonymous = false,
    };
    job->setMethodCallJobInfo(std::move(info));
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::MethodCallJob::methodCallDone, this, [this]([[maybe_unused]] const QJsonObject &replyObject) {
        mRocketChatAccount->roomFiles(mRoomId, mRoomType);
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start saveSettings job";
    }
#endif
}

#include "moc_featurepreviewwidget.cpp"
