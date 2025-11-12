/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchmessagewidget.h"
#include "ruqola.h"
#include "texttospeech/texttospeechenqueuemanager.h"

#include "chat/searchmessagejob.h"
#include "connection.h"
#include "model/commonmessagefilterproxymodel.h"
#include "model/commonmessagesmodel.h"
#include "rocketchataccount.h"
#include "room/messagelistview.h"
#include "ruqolawidgets_debug.h"
#include "searchmessagewithdelaylineedit.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

#include "config-ruqola.h"

#if HAVE_TEXT_TO_SPEECH
#include <TextEditTextToSpeech/TextToSpeechContainerWidget>
#endif

constexpr ushort numberOfElment = 20;

using namespace Qt::Literals::StringLiterals;
SearchMessageWidget::SearchMessageWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mSearchMessageModel(new CommonMessagesModel(account, this))
    , mSearchMessageFilterProxyModel(new CommonMessageFilterProxyModel(mSearchMessageModel, this))
    , mSearchLabel(new QLabel(this))
    , mSearchLineEdit(new SearchMessageWithDelayLineEdit(account, this))
    , mResultListWidget(new MessageListView(account, MessageListView::Mode::Viewing, this))
    , mCurrentRocketChatAccount(account)
#if HAVE_TEXT_TO_SPEECH
    , mTextToSpeechWidget(new TextEditTextToSpeech::TextToSpeechContainerWidget(this))
#endif
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mSearchLineEdit->setObjectName(u"mSearchLineEdit"_s);
    mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search messages"));
    KLineEditEventHandler::catchReturnKey(mSearchLineEdit);
    mainLayout->addWidget(mSearchLineEdit);
    auto labelRegularExpression = new QLabel(i18n("You can search using <a href=\"https://en.wikipedia.org/wiki/Regular_expression\">Regular "
                                                  "Expression</a>. e.g. <code>/^text$/i</code>"),
                                             this);
    labelRegularExpression->setObjectName(u"labelRegularExpression"_s);
    labelRegularExpression->setOpenExternalLinks(true);

    QFont font = labelRegularExpression->font();
    font.setItalic(true);
    labelRegularExpression->setFont(font);

    mainLayout->addWidget(labelRegularExpression);
    connect(mSearchLineEdit, &SearchWithDelayLineEdit::searchRequested, this, &SearchMessageWidget::slotSearchMessages);
    connect(mSearchLineEdit, &SearchWithDelayLineEdit::searchCleared, this, &SearchMessageWidget::slotClearedMessages);

    mSearchLabel->setObjectName(u"mSearchLabel"_s);
    QFont labFont = mSearchLabel->font();
    labFont.setBold(true);
    mSearchLabel->setFont(labFont);
    mainLayout->addWidget(mSearchLabel);

#if HAVE_TEXT_TO_SPEECH
    mTextToSpeechWidget->setObjectName(u"mTextToSpeechWidget"_s);
    mainLayout->addWidget(mTextToSpeechWidget);
    connect(mResultListWidget, &MessageListView::textToSpeech, this, [this](const QString &str) {
        Ruqola::self()->accountManager()->textToSpeechEnqueueManager()->insertDummyInfo();
        mTextToSpeechWidget->enqueue(str);
    });
#endif

    mResultListWidget->setObjectName(u"mResultListWidget"_s);
    mainLayout->addWidget(mResultListWidget);
    connect(mSearchLineEdit, &QLineEdit::returnPressed, this, &SearchMessageWidget::slotSearchLineMessagesEnterPressed);
    connect(mResultListWidget, &MessageListView::goToMessageRequested, this, &SearchMessageWidget::goToMessageRequested);
    connect(mResultListWidget, &MessageListView::loadHistoryRequested, this, &SearchMessageWidget::slotLoadHistory);

    mResultListWidget->setModel(mSearchMessageFilterProxyModel);
    connect(mSearchMessageFilterProxyModel, &CommonMessageFilterProxyModel::stringNotFoundChanged, this, &SearchMessageWidget::updateLabel);
    connect(mSearchMessageFilterProxyModel, &CommonMessageFilterProxyModel::loadingInProgressChanged, this, &SearchMessageWidget::updateLabel);
    updateLabel();
}

SearchMessageWidget::~SearchMessageWidget()
{
    if (mCurrentRocketChatAccount) {
        clearSearchModel();
    }
}

void SearchMessageWidget::clearSearchModel()
{
    mSearchMessageModel->clearModel();
    mMessageIsEmpty = false;
    mOffset = -1;
}

void SearchMessageWidget::messageSearch(const QString &pattern, const QByteArray &rid, bool userRegularExpression, int offset)
{
    if (pattern.isEmpty()) {
        clearSearchModel();
    } else {
        if (!mMessageIsEmpty) {
            mSearchMessageModel->setLoadCommonMessagesInProgress(true);
            searchMessages(rid, pattern, userRegularExpression, offset);
        }
    }
}

void SearchMessageWidget::slotSearchMessagesDone(const QJsonObject &obj)
{
    mSearchMessageModel->setLoadCommonMessagesInProgress(false);
    mMessageIsEmpty = mSearchMessageModel->parse(obj, false, true);
}

void SearchMessageWidget::searchMessages(const QByteArray &roomId, const QString &pattern, bool useRegularExpression, int offset)
{
    auto job = new RocketChatRestApi::SearchMessageJob(this);
    job->setRoomId(QString::fromLatin1(roomId));
    job->setSearchText(pattern);
    job->setCount(numberOfElment);
    job->setOffset(offset);
    job->setUseRegularExpression(useRegularExpression);
    mCurrentRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::SearchMessageJob::searchMessageDone, this, &SearchMessageWidget::slotSearchMessagesDone);
    connect(job, &RocketChatRestApi::SearchMessageJob::failed, this, &SearchMessageWidget::slotSearchMessagesFailed);
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start searchMessages job";
    }
}

void SearchMessageWidget::slotSearchMessagesFailed()
{
    mSearchMessageModel->setLoadCommonMessagesInProgress(false);
    updateLabel();
}

void SearchMessageWidget::slotLoadHistory()
{
    messageSearch(mSearchLineEdit->text(), mRoomId, true, mOffset);
    mOffset += numberOfElment;
}

void SearchMessageWidget::slotClearedMessages()
{
    mSearchMessageFilterProxyModel->clearModel();
    updateLabel();
}

void SearchMessageWidget::slotSearchMessages(const QString &str)
{
    if (mSearchText != str) {
        mSearchText = str;
        clearSearchModel();
        mSearchMessageFilterProxyModel->setSearchText(str);
        mSearchLineEdit->addCompletionItem(str);
        messageSearch(str, mRoomId, true);
        mOffset = numberOfElment;
    }
}

void SearchMessageWidget::slotSearchLineMessagesEnterPressed()
{
    slotSearchMessages(mSearchLineEdit->text());
}

QByteArray SearchMessageWidget::roomId() const
{
    return mRoomId;
}

void SearchMessageWidget::setRoomId(const QByteArray &roomId)
{
    mRoomId = roomId;
}

void SearchMessageWidget::updateLabel()
{
    if (mSearchMessageFilterProxyModel->loadCommonMessagesInProgress()) {
        mSearchLabel->setText(i18n("Loading…"));
    } else if (mSearchLineEdit->text().isEmpty() && mSearchMessageFilterProxyModel->rowCount() == 0) {
        mSearchLabel->clear();
    } else {
        mSearchLabel->setText(mSearchMessageFilterProxyModel->rowCount() == 0 ? i18n("No Message found") : displayShowSearch());
    }
}

QString SearchMessageWidget::displayShowSearch() const
{
    const QString displayMessageStr = i18np("%1 Message in room", "%1 Messages in room", mSearchMessageFilterProxyModel->rowCount());
    return displayMessageStr;
}

RocketChatAccount *SearchMessageWidget::currentRocketChatAccount() const
{
    return mCurrentRocketChatAccount;
}

void SearchMessageWidget::setRoom(Room *room)
{
    mResultListWidget->setRoom(room);
}

#include "moc_searchmessagewidget.cpp"
