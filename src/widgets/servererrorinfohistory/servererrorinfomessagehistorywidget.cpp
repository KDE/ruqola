/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfomessagehistorywidget.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/serverscombobox.h"
#include "model/servererrorinfohistoryfilterproxymodel.h"
#include "model/servererrorinfohistorymodel.h"
#include "servererrorinfohistorymanager.h"
#include "servererrorinfomessagehistorylistview.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLineEdit>
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
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto searchLayout = new QHBoxLayout;
    searchLayout->setObjectName(u"searchLayout"_s);
    searchLayout->setContentsMargins({});

    mSearchLineEdit->setObjectName(u"mSearchLineEdit"_s);
    mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search…"));
    searchLayout->addWidget(mSearchLineEdit);
    mSearchLineEdit->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(mSearchLineEdit);

    mServersComboBox->setObjectName(u"mServersComboBox"_s);
    searchLayout->addWidget(mServersComboBox);

    mainLayout->addLayout(searchLayout);

    auto model = ServerErrorInfoHistoryManager::self()->serverErrorInfoHistoryModel();

    mListServerInfosListView->setObjectName(u"mListServerInfosListView"_s);

    mServerErrorInfoHistoryFilterProxyModel->setObjectName(u"mServerErrorInfoHistoryFilterProxyModel"_s);
    mServerErrorInfoHistoryFilterProxyModel->setSourceModel(model);
    mListServerInfosListView->setModel(mServerErrorInfoHistoryFilterProxyModel);

    connect(model, &QAbstractItemModel::rowsAboutToBeInserted, mListServerInfosListView, &MessageListViewBase::checkIfAtBottom);
    connect(model, &QAbstractItemModel::rowsAboutToBeRemoved, mListServerInfosListView, &MessageListViewBase::checkIfAtBottom);
    connect(model, &QAbstractItemModel::modelAboutToBeReset, mListServerInfosListView, &MessageListViewBase::checkIfAtBottom);

#if HAVE_TEXT_TO_SPEECH
    mTextToSpeechWidget->setObjectName(u"mTextToSpeechWidget"_s);
    mainLayout->addWidget(mTextToSpeechWidget);
#if HAVE_TEXTTOSPEECH_ENQQUEUE_SUPPORT
    connect(mListServerInfosListView,
            &ServerErrorInfoMessageHistoryListView::textToSpeech,
            mTextToSpeechWidget,
            &TextEditTextToSpeech::TextToSpeechContainerWidget::enqueue);
#else
    connect(mListServerInfosListView,
            &ServerErrorInfoMessageHistoryListView::textToSpeech,
            mTextToSpeechWidget,
            &TextEditTextToSpeech::TextToSpeechContainerWidget::say);
#endif
#endif

    mListServerInfosListView->setObjectName(u"mListServerInfosListView"_s);
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
