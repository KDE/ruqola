/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportinfowidget.h"
using namespace Qt::Literals::StringLiterals;

#include "model/moderationreportinfofilterproxymodel.h"
#include "model/moderationreportinfomodel.h"
#include "moderationreportinfolistview.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLineEdit>
#include <QVBoxLayout>

#include "config-ruqola.h"

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
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mSearchLineEdit->setObjectName(u"mSearchLineEdit"_s);
    mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search…"));
    mainLayout->addWidget(mSearchLineEdit);
    mSearchLineEdit->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(mSearchLineEdit);

#if HAVE_TEXT_TO_SPEECH
    mTextToSpeechWidget->setObjectName(u"mTextToSpeechWidget"_s);
    mainLayout->addWidget(mTextToSpeechWidget);
    connect(mListNotificationsListView,
            &ModerationReportInfoListView::textToSpeech,
            mTextToSpeechWidget,
            &TextEditTextToSpeech::TextToSpeechContainerWidget::enqueue);
#endif

    mListNotificationsListView->setObjectName(u"mListNotifications"_s);
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
