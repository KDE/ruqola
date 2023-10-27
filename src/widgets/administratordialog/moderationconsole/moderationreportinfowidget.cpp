/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationreportinfowidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/moderationreportinfomodel.h"
#include "moderationreportinfolistview.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QLineEdit>
#include <QListView>
#include <QVBoxLayout>

#include <config-ruqola.h>

#if HAVE_TEXT_TO_SPEECH
#include <TextEditTextToSpeech/TextToSpeechContainerWidget>
#endif

ModerationReportInfoWidget::ModerationReportInfoWidget(QWidget *parent)
    : QWidget{parent}
    , mListNotificationsListView(new ModerationReportInfoListView(this))
    , mSearchLineEdit(new QLineEdit(this))
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

    mListNotificationsListView->setModel(mModerationReportInfoModel);
#if 0
    connect(model, &QAbstractItemModel::rowsAboutToBeInserted, mListNotificationsListView, &MessageListViewBase::checkIfAtBottom);
    connect(model, &QAbstractItemModel::rowsAboutToBeRemoved, mListNotificationsListView, &MessageListViewBase::checkIfAtBottom);
    connect(model, &QAbstractItemModel::modelAboutToBeReset, mListNotificationsListView, &MessageListViewBase::checkIfAtBottom);
#endif
    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &ModerationReportInfoWidget::slotTextChanged);
}

ModerationReportInfoWidget::~ModerationReportInfoWidget() = default;

void ModerationReportInfoWidget::slotTextChanged(const QString &str)
{
    // TODO mNotificationFilterProxyModel->setFilterString(str);
    mListNotificationsListView->setSearchText(str);
}

#include "moc_moderationreportinfowidget.cpp"
