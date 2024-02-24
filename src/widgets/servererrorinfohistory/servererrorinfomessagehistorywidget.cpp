/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfomessagehistorywidget.h"
#include "misc/serverscombobox.h"
#include "model/servererrorinfohistoryfilterproxymodel.h"
#include "model/servererrorinfohistorymodel.h"
#include "servererrorinfohistorymanager.h"
#include "servererrorinfomessagehistorylistview.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLineEdit>
#include <QListView>
#include <QVBoxLayout>

#include "config-ruqola.h"

#if HAVE_TEXT_TO_SPEECH
#include <TextEditTextToSpeech/TextToSpeechContainerWidget>
#endif

ServerErrorInfoMessageHistoryWidget::ServerErrorInfoMessageHistoryWidget(QWidget *parent)
    : QWidget{parent}
    , mSearchLineEdit(new QLineEdit(this))
    , mListServerInfosListView(new ServerErrorInfoMessageHistoryListView(this))
#if HAVE_TEXT_TO_SPEECH
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
    KLineEditEventHandler::catchReturnKey(mSearchLineEdit);

    mServersComboBox->setObjectName(QStringLiteral("mServersComboBox"));
    searchLayout->addWidget(mServersComboBox);

    mainLayout->addLayout(searchLayout);

    auto model = ServerErrorInfoHistoryManager::self()->serverErrorInfoHistoryModel();

    mListServerInfosListView->setObjectName(QStringLiteral("mListServerInfosListView"));

    mServerErrorInfoHistoryFilterProxyModel->setObjectName(QStringLiteral("mServerErrorInfoHistoryFilterProxyModel"));
    mServerErrorInfoHistoryFilterProxyModel->setSourceModel(model);
    mListServerInfosListView->setModel(mServerErrorInfoHistoryFilterProxyModel);

    connect(model, &QAbstractItemModel::rowsAboutToBeInserted, mListServerInfosListView, &MessageListViewBase::checkIfAtBottom);
    connect(model, &QAbstractItemModel::rowsAboutToBeRemoved, mListServerInfosListView, &MessageListViewBase::checkIfAtBottom);
    connect(model, &QAbstractItemModel::modelAboutToBeReset, mListServerInfosListView, &MessageListViewBase::checkIfAtBottom);

#if HAVE_TEXT_TO_SPEECH
    mTextToSpeechWidget->setObjectName(QStringLiteral("mTextToSpeechWidget"));
    mainLayout->addWidget(mTextToSpeechWidget);
    connect(mListServerInfosListView,
            &ServerErrorInfoMessageHistoryListView::textToSpeech,
            mTextToSpeechWidget,
            &TextEditTextToSpeech::TextToSpeechContainerWidget::say);
#endif

    mListServerInfosListView->setObjectName(QStringLiteral("mListServerInfosListView"));
    mainLayout->addWidget(mListServerInfosListView);

    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &ServerErrorInfoMessageHistoryWidget::slotTextChanged);

    connect(mServersComboBox, &ServersComboBox::accountSelected, this, &ServerErrorInfoMessageHistoryWidget::slotFilterAccount);
}

ServerErrorInfoMessageHistoryWidget::~ServerErrorInfoMessageHistoryWidget() = default;

void ServerErrorInfoMessageHistoryWidget::slotFilterAccount(const QString &accountName)
{
    mServerErrorInfoHistoryFilterProxyModel->setAccountNameFilter(accountName);
}

void ServerErrorInfoMessageHistoryWidget::slotTextChanged(const QString &str)
{
    mServerErrorInfoHistoryFilterProxyModel->setFilterString(str);
    mListServerInfosListView->setSearchText(str);
}

void ServerErrorInfoMessageHistoryWidget::addServerList(const QList<AccountManager::AccountDisplayInfo> &infos)
{
    mServersComboBox->addServerList(infos);
}

#include "moc_servererrorinfomessagehistorywidget.cpp"
