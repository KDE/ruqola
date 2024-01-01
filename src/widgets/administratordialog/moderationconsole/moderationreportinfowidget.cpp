/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportinfowidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/moderationreportinfofilterproxymodel.h"
#include "model/moderationreportinfomodel.h"
#include "moderationreportinfolistview.h"
#include <KLocalizedString>
#include <QLineEdit>
#include <QListView>
#include <QVBoxLayout>

#include <config-ruqola.h>

#if HAVE_TEXT_TO_SPEECH
#include <TextEditTextToSpeech/TextToSpeechContainerWidget>
#endif

ModerationReportInfoWidget::ModerationReportInfoWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget{parent}
    , mListNotificationsListView(new ModerationReportInfoListView(account, this))
    , mSearchLineEdit(new QLineEdit(this))
    , mModerationReportInfoFilterProxyModel(new ModerationReportInfoFilterProxyModel(this))
    , mModerationReportInfoModel(new ModerationReportInfoModel(this))
#if HAVE_TEXT_TO_SPEECH
    , mTextToSpeechWidget(new TextEditTextToSpeech::TextToSpeechContainerWidget(this))
#endif
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mSearchLineEdit->setPlaceholderText(i18n("Search..."));
    mainLayout->addWidget(mSearchLineEdit);
    mSearchLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchLineEdit, this);

#if HAVE_TEXT_TO_SPEECH
    mTextToSpeechWidget->setObjectName(QStringLiteral("mTextToSpeechWidget"));
    mainLayout->addWidget(mTextToSpeechWidget);
    connect(mListNotificationsListView,
            &ModerationReportInfoListView::textToSpeech,
            mTextToSpeechWidget,
            &TextEditTextToSpeech::TextToSpeechContainerWidget::say);
#endif

    mListNotificationsListView->setObjectName(QStringLiteral("mListNotifications"));
    mainLayout->addWidget(mListNotificationsListView);

    mModerationReportInfoFilterProxyModel->setSourceModel(mModerationReportInfoModel);
    mListNotificationsListView->setModel(mModerationReportInfoFilterProxyModel);
    connect(mModerationReportInfoModel, &QAbstractItemModel::rowsAboutToBeInserted, mListNotificationsListView, &MessageListViewBase::checkIfAtBottom);
    connect(mModerationReportInfoModel, &QAbstractItemModel::rowsAboutToBeRemoved, mListNotificationsListView, &MessageListViewBase::checkIfAtBottom);
    connect(mModerationReportInfoModel, &QAbstractItemModel::modelAboutToBeReset, mListNotificationsListView, &MessageListViewBase::checkIfAtBottom);
    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &ModerationReportInfoWidget::slotTextChanged);
}

ModerationReportInfoWidget::~ModerationReportInfoWidget() = default;

void ModerationReportInfoWidget::setReportInfos(const ModerationReportInfos &infos)
{
    mModerationReportInfoModel->addModerationReportInfos(infos);
}

void ModerationReportInfoWidget::slotTextChanged(const QString &str)
{
    mModerationReportInfoFilterProxyModel->setFilterString(str);
    mListNotificationsListView->setSearchText(str);
}

#include "moc_moderationreportinfowidget.cpp"
