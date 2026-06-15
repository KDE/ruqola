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
#include <QJsonArray>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
FeaturePreviewWidget::FeaturePreviewWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mAllowFeaturePreview(new QCheckBox(i18nc("@option:check", "Allow Feature Preview"), this))
    , mQuickReactions(new QCheckBox(i18nc("@option:check", "Quick Reactions"), this))
    , mTimestampInMessages(new QCheckBox(i18nc("@option:check", "Timestamp in Messages"), this))
    , mDraftMessages(new QCheckBox(i18nc("@option:check", "Draft Messages"), this))
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

    mDraftMessages->setObjectName(u"mDraftMessages"_s);
    mainLayout->addWidget(mDraftMessages);

    mTimestampInMessages->setObjectName(u"mTimestampInMessages"_s);
    mainLayout->addWidget(mTimestampInMessages);
    // Move in stable in RC 8.0.0
    if (account && account->hasAtLeastVersion(8, 0, 0)) {
        if (!account->ownUserPreferences().serverHasPreviewFeature(FeaturePreviewPreferences::FeaturePreviewType::EnableTimestampMessageParser)) {
            mTimestampInMessages->hide();
        }
        if (account->hasAtLeastVersion(8, 5, 0)) {
            if (!account->ownUserPreferences().serverHasPreviewFeature(FeaturePreviewPreferences::FeaturePreviewType::EnableDraftSupport)) {
                mDraftMessages->hide();
            }
        } else {
            mDraftMessages->hide();
        }
        mQuickReactions->hide();
    }

    connect(mAllowFeaturePreview, &QCheckBox::clicked, this, &FeaturePreviewWidget::slotFeaturePreviewChanged);
    mainLayout->addStretch(1);
    slotFeaturePreviewChanged(mAllowFeaturePreview->isChecked());
}

FeaturePreviewWidget::~FeaturePreviewWidget() = default;

void FeaturePreviewWidget::slotFeaturePreviewChanged(bool checked)
{
    mQuickReactions->setEnabled(checked);
    mTimestampInMessages->setEnabled(checked);
    mDraftMessages->setEnabled(checked);
}

void FeaturePreviewWidget::initialize()
{
    if (!mRocketChatAccount) {
        return;
    }
    if (mRocketChatAccount->ownUserPreferences().serverHasPreviewFeature(FeaturePreviewPreferences::FeaturePreviewType::EnableDraftSupport)) {
        mDraftMessages->setChecked(mRocketChatAccount->ownUserPreferences().hasFeature(FeaturePreviewPreferences::FeaturePreviewType::EnableDraftSupport));
    }
    if (mRocketChatAccount->ownUserPreferences().serverHasPreviewFeature(FeaturePreviewPreferences::FeaturePreviewType::EnableTimestampMessageParser)) {
        mTimestampInMessages->setChecked(
            mRocketChatAccount->ownUserPreferences().hasFeature(FeaturePreviewPreferences::FeaturePreviewType::EnableTimestampMessageParser));
    }
}

void FeaturePreviewWidget::save()
{
    auto job = new RocketChatRestApi::MethodCallJob(this);
    QJsonArray params;
    QJsonArray previewFeatures;
    {
        QJsonObject obj;
        obj["_id"_L1] = u"Accounts_AllowFeaturePreview"_s;
        obj["value"_L1] = mAllowFeaturePreview->isChecked();
        previewFeatures.append(obj);
    }
    {
        QJsonObject obj;
        obj["_id"_L1] = u"Accounts_Default_User_Preferences_featuresPreview"_s;
        QJsonObject objValue;
        if (mDraftMessages->isChecked()) {
            QString value = QStringLiteral("[{\\\"name\\\":\\\"sidebarDrafts\\\",\\\"value\\\":true}]");
            objValue["value"_L1] = value;
        } else {
            QString value = QStringLiteral("[{\\\"name\\\":\\\"sidebarDrafts\\\",\\\"value\\\":false}]");
            objValue["value"_L1] = value;
        }
        obj["value"_L1] = objValue;
        previewFeatures.append(obj);
    }
    params.append(previewFeatures);
    const QString methodName{u"saveSettings"_s};
    const RocketChatRestApi::MethodCallJob::MethodCallJobInfo info{
        .messageObj = mRocketChatAccount->ddp()->generateJsonObject(methodName, params),
        .methodName = methodName,
        .anonymous = false,
    };
    job->setMethodCallJobInfo(std::move(info));
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start saveSettings job";
    }
}

#include "moc_featurepreviewwidget.cpp"
