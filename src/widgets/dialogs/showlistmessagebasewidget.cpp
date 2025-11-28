/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showlistmessagebasewidget.h"

#include "dialogs/searchmessagelinewidget.h"
#include "room/messagelistview.h"

#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>

#include "config-ruqola.h"

#if HAVE_TEXT_TO_SPEECH
#include <TextEditTextToSpeech/TextToSpeechContainerWidget>
#endif

using namespace Qt::Literals::StringLiterals;
ShowListMessageBaseWidget::ShowListMessageBaseWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mSearchMessageLineWidget(new SearchMessageLineWidget(this))
    , mMessageListInfo(new QLabel(this))
    , mMessageListView(new MessageListView(account, MessageListView::Mode::Viewing, this))
#if HAVE_TEXT_TO_SPEECH
    , mTextToSpeechWidget(new TextEditTextToSpeech::TextToSpeechContainerWidget(this))
#endif
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mSearchMessageLineWidget->setObjectName(u"mSearchMessageLineWidget"_s);
    connect(mSearchMessageLineWidget, &SearchMessageLineWidget::textChanged, this, &ShowListMessageBaseWidget::slotSearchMessageTextChanged);
    mainLayout->addWidget(mSearchMessageLineWidget);

    mMessageListInfo->setObjectName(u"mMessageListInfo"_s);
    mMessageListInfo->setTextFormat(Qt::RichText);
    mMessageListInfo->setContextMenuPolicy(Qt::NoContextMenu);
    mMessageListView->setProperty("_breeze_borders_sides", QVariant{});
    QFont labFont = mMessageListInfo->font();
    labFont.setBold(true);
    mMessageListInfo->setFont(labFont);
    connect(mMessageListInfo, &QLabel::linkActivated, this, &ShowListMessageBaseWidget::loadMoreElements);
    mainLayout->addWidget(mMessageListInfo);

    mMessageListView->setObjectName(u"mMessageListView"_s);

#if HAVE_TEXT_TO_SPEECH
    mTextToSpeechWidget->setObjectName(u"mTextToSpeechWidget"_s);
    mainLayout->addWidget(mTextToSpeechWidget);

    connect(mMessageListView, &MessageListView::textToSpeech, mTextToSpeechWidget, &TextEditTextToSpeech::TextToSpeechContainerWidget::enqueue);
#endif

    mainLayout->addWidget(mMessageListView);
    connect(mMessageListView, &MessageListView::modelChanged, this, &ShowListMessageBaseWidget::updateLabel);
}

ShowListMessageBaseWidget::~ShowListMessageBaseWidget()
{
    // Clear search text
    if (mModel && mModel->listMessageModel()) {
        mModel->listMessageModel()->setSearchText({});
        mModel->clear();
    }
}

void ShowListMessageBaseWidget::setFilteringByType(ListMessagesFilterProxyModel::FilteringByType type)
{
    mModel->setFilteringByType(type);
}
void ShowListMessageBaseWidget::setFilterProxyModel(ListMessagesFilterProxyModel *model)
{
    mModel = model;
    mMessageListView->setModel(model);
    mModel->setFilterString({});
    connect(mModel, &ListMessagesFilterProxyModel::hasFullListChanged, this, &ShowListMessageBaseWidget::updateLabel);
    connect(mModel, &ListMessagesFilterProxyModel::loadingInProgressChanged, this, &ShowListMessageBaseWidget::updateLabel);
    updateLabel();
}

MessageListView *ShowListMessageBaseWidget::messageListView() const
{
    return mMessageListView;
}

void ShowListMessageBaseWidget::setRoom(Room *room)
{
    mMessageListView->setRoom(room);
}

void ShowListMessageBaseWidget::addMessageLineWidget(QWidget *w)
{
    mSearchMessageLineWidget->addMessageLineWidget(w);
}

void ShowListMessageBaseWidget::updateLabel()
{
    if (mModel->loadMoreListMessagesInProgress()) {
        mMessageListInfo->setText(i18n("Loading…"));
    } else {
        mMessageListInfo->setText(mModel->numberOfMessages() == 0 ? i18n("No Message found") : displayShowMessageInRoom());
    }
}

QString ShowListMessageBaseWidget::displayShowMessageInRoom() const
{
    QString displayMessageStr = i18np("%1 Message in room (Total: %2)", "%1 Messages in room (Total: %2)", mModel->numberOfMessages(), mModel->total());
    if (!mModel->hasFullList()) {
        displayMessageStr += u" <a href=\"loadmoreelement\">%1</a>"_s.arg(i18n("(Click here for Loading more…)"));
    }
    return displayMessageStr;
}

void ShowListMessageBaseWidget::slotSearchMessageTextChanged(const QString &str)
{
    mModel->setFilterString(str);
    mModel->listMessageModel()->setSearchText(str);
    mMessageListView->setSearchText(str);
}

#include "moc_showlistmessagebasewidget.cpp"
