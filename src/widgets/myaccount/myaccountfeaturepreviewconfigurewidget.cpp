/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountfeaturepreviewconfigurewidget.h"
#include "rocketchataccount.h"
#include "users/userssetpreferencesjob.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
MyAccountFeaturePreviewConfigureWidget::MyAccountFeaturePreviewConfigureWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mRocketChatAccount(account)
    // timestamp by default in RC 8.0.0 => remove it when we will depend against it
    , mTimeStampCheckBox(new QCheckBox(i18nc("@option:check", "Timestamp in messages"), this))
    , mAddDraftCheckBox(new QCheckBox(i18nc("@option:check", "Draft Messages"), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mTimeStampCheckBox->setObjectName(u"mTimeStampCheckBox"_s);
    mainLayout->addWidget(mTimeStampCheckBox);

    mAddDraftCheckBox->setObjectName(u"mAddDraftCheckBox"_s);
    mainLayout->addWidget(mAddDraftCheckBox);

    mainLayout->addStretch(1);
}

MyAccountFeaturePreviewConfigureWidget::~MyAccountFeaturePreviewConfigureWidget() = default;

void MyAccountFeaturePreviewConfigureWidget::load()
{
    // qDebug() << " mRocketChatAccount->ownUserPreferences() " << mRocketChatAccount->ownUserPreferences().featurePreviewPreferences().previewStatus();
    const OwnUserPreferences ownUserPreferences = mRocketChatAccount->ownUserPreferences();
    if (ownUserPreferences.serverHasPreviewFeature(FeaturePreviewPreferences::FeaturePreviewType::EnableDraftSupport)) {
        mAddDraftCheckBox->setChecked(ownUserPreferences.hasFeature(FeaturePreviewPreferences::FeaturePreviewType::EnableDraftSupport));
    }
    if (ownUserPreferences.serverHasPreviewFeature(FeaturePreviewPreferences::FeaturePreviewType::EnableTimestampMessageParser)) {
        mTimeStampCheckBox->setChecked(ownUserPreferences.hasFeature(FeaturePreviewPreferences::FeaturePreviewType::EnableTimestampMessageParser));
    }
}

void MyAccountFeaturePreviewConfigureWidget::save()
{
    RocketChatRestApi::UsersSetPreferencesJob::UsersSetPreferencesInfo info;
    QMap<QString, bool> featuresPreview;
    if (!mAddDraftCheckBox->isHidden()) {
        featuresPreview.insert("sidebarDrafts"_L1, mAddDraftCheckBox->isChecked());
    }
    if (!mTimeStampCheckBox->isHidden()) {
        // TODO
        featuresPreview.insert("timeStamp"_L1, mTimeStampCheckBox->isChecked());
    }
    info.featuresPreview = std::move(featuresPreview);
    info.userId = mRocketChatAccount->userId();
    mRocketChatAccount->setUserPreferences(info);
}

void MyAccountFeaturePreviewConfigureWidget::initialize()
{
    const bool hasV8 = mRocketChatAccount && mRocketChatAccount->hasAtLeastVersion(8, 0, 0);
    const OwnUserPreferences prefs = mRocketChatAccount ? mRocketChatAccount->ownUserPreferences() : OwnUserPreferences{};
    mTimeStampCheckBox->setHidden(!hasV8 || !prefs.serverHasPreviewFeature(FeaturePreviewPreferences::FeaturePreviewType::EnableTimestampMessageParser));
    mAddDraftCheckBox->setHidden(!hasV8 || !mRocketChatAccount->hasAtLeastVersion(8, 5, 0)
                                 || !prefs.serverHasPreviewFeature(FeaturePreviewPreferences::FeaturePreviewType::EnableDraftSupport));
}

#include "moc_myaccountfeaturepreviewconfigurewidget.cpp"
