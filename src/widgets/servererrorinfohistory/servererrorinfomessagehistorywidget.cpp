/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfomessagehistorywidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "misc/serverscombobox.h"
#include "model/servererrorinfohistoryfilterproxymodel.h"
#include "model/servererrorinfohistorymodel.h"
#include "ruqolawidgets_debug.h"
#include "servererrorinfohistorymanager.h"
#include "servererrorinfomessagehistorylistview.h"
#include <KLocalizedString>
#include <QLineEdit>
#include <QListView>
#include <QVBoxLayout>
#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
#include <TextEditTextToSpeech/TextToSpeechContainerWidget>
#endif

ServerErrorInfoMessageHistoryWidget::ServerErrorInfoMessageHistoryWidget(QWidget *parent)
    : QWidget{parent}
    , mSearchLineEdit(new QLineEdit(this))
    , mListServerInfosListView(new ServerErrorInfoMessageHistoryListView(this))
#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
    , mTextToSpeechWidget(new TextEditTextToSpeech::TextToSpeechContainerWidget(this))
#endif
    , mServerErrorInfoHistoryFilterProxyModel(new ServerErrorInfoHistoryFilterProxyModel(this))
    , mServersComboBox(new ServersComboBox(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto searchLayout = new QHBoxLayout;
    searchLayout->setObjectName(QStringLiteral("searchLayout"));
    searchLayout->setContentsMargins({});

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mSearchLineEdit->setPlaceholderText(i18n("Search..."));
    searchLayout->addWidget(mSearchLineEdit);
    mSearchLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchLineEdit, this);

    mServersComboBox->setObjectName(QStringLiteral("mServersComboBox"));
    searchLayout->addWidget(mServersComboBox);

    mainLayout->addLayout(searchLayout);

    auto model = ServerErrorInfoHistoryManager::self()->serverErrorInfoHistoryModel();

    mServerErrorInfoHistoryFilterProxyModel->setObjectName(QStringLiteral("mServerErrorInfoHistoryFilterProxyModel"));
    mServerErrorInfoHistoryFilterProxyModel->setSourceModel(model);
    mListServerInfosListView->setModel(mServerErrorInfoHistoryFilterProxyModel);

    connect(model, &QAbstractItemModel::rowsAboutToBeInserted, mListServerInfosListView, &MessageListViewBase::checkIfAtBottom);
    connect(model, &QAbstractItemModel::rowsAboutToBeRemoved, mListServerInfosListView, &MessageListViewBase::checkIfAtBottom);
    connect(model, &QAbstractItemModel::modelAboutToBeReset, mListServerInfosListView, &MessageListViewBase::checkIfAtBottom);

#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
    mTextToSpeechWidget->setObjectName(QStringLiteral("mTextToSpeechWidget"));
    mainLayout->addWidget(mTextToSpeechWidget);
#endif
    mListServerInfosListView->setObjectName(QStringLiteral("mListServerInfosListView"));
    mainLayout->addWidget(mListServerInfosListView);

    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &ServerErrorInfoMessageHistoryWidget::slotTextChanged);

    connect(mServersComboBox, &ServersComboBox::textActivated, this, &ServerErrorInfoMessageHistoryWidget::slotFilterAccount);
#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
    connect(mListServerInfosListView, &ServerErrorInfoMessageHistoryListView::textToSpeech, this, &ServerErrorInfoMessageHistoryWidget::slotTextToSpeech);
#endif
}

ServerErrorInfoMessageHistoryWidget::~ServerErrorInfoMessageHistoryWidget() = default;

void ServerErrorInfoMessageHistoryWidget::slotFilterAccount(const QString &accountName)
{
    mServerErrorInfoHistoryFilterProxyModel->setAccountNameFilter(accountName);
}

#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
void ServerErrorInfoMessageHistoryWidget::slotTextToSpeech(const QString &messageText)
{
    mTextToSpeechWidget->say(messageText);
}
#endif

void ServerErrorInfoMessageHistoryWidget::slotTextChanged(const QString &str)
{
    mServerErrorInfoHistoryFilterProxyModel->setFilterString(str);
    mListServerInfosListView->setSearchText(str);
}

void ServerErrorInfoMessageHistoryWidget::addServerList(const QStringList &serverNames)
{
    mServersComboBox->addServerList(serverNames);
}
