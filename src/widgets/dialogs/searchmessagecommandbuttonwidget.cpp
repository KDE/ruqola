/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchmessagecommandbuttonwidget.h"
#include "common/flowlayout.h"
#include "dialogs/searchmessagecommand.h"
#include <KLocalizedString>
#include <QPushButton>
using namespace Qt::Literals::StringLiterals;
SearchMessageCommandButtonWidget::SearchMessageCommandButtonWidget(QWidget *parent)
    : QWidget{parent}
{
    auto flowLayout = new FlowLayout(this);
    flowLayout->setObjectName(u"flowLayout"_s);
    flowLayout->setContentsMargins({});
    flowLayout->setSpacing(0);
    flowLayout->setHorizontalSpacing(0);
    flowLayout->setVerticalSpacing(0);
    const QList<SearchMessageCommandButtonWidget::ButtonInfo> buttonsList = fillCommandLineText();
    Q_ASSERT(!buttonsList.isEmpty());
    for (const auto &info : std::as_const(buttonsList)) {
        flowLayout->addWidget(createPushButton(info));
    }
}

SearchMessageCommandButtonWidget::~SearchMessageCommandButtonWidget() = default;

QList<SearchMessageCommandButtonWidget::ButtonInfo> SearchMessageCommandButtonWidget::fillCommandLineText() const
{
    const QList<SearchMessageCommandButtonWidget::ButtonInfo> buttonInfo = {
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::FromMe),
         i18nc("@action:button", "From Me"),
         i18nc("@info:tooltip", "Finds Messages sent by you.")},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::FromUserName),
         i18nc("@action:button", "From"),
         i18nc("@info:tooltip", "Finds Messages from a specific user. (Use the username format without space)")},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::Order),
         i18nc("@action:button", "Order:desc"),
         i18nc("@info:tooltip", "Sorts message by descending timestamp")},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::Before), i18nc("@action:button", "Before"), i18nc("@info:tooltip", "")},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::HasLocation),
         i18nc("@action:button", "Has Location"),
         i18nc("@info:tooltip", "Finds messages that include a location.")},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::HasUrl),
         i18nc("@action:button", "Has Url"),
         i18nc("@info:tooltip", "Finds messages that contain a link")},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::IsPinned),
         i18nc("@action:button", "Is Pinned"),
         i18nc("@info:tooltip", "Displays pinned messages in the current room.")},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::HasStar),
         i18nc("@action:button", "Has Star"),
         i18nc("@info:tooltip", "Shows messages you've starred.")},
    };
    return buttonInfo;
}

QPushButton *SearchMessageCommandButtonWidget::createPushButton(const SearchMessageCommandButtonWidget::ButtonInfo &info)
{
    auto pushButton = new QPushButton(info.i18n, this);
    pushButton->setObjectName(info.identifier);
    pushButton->setToolTip(info.toolTip);
    const QString identifier = info.identifier;
    connect(pushButton, &QPushButton::clicked, this, [this, identifier]() {
        Q_EMIT insertCommand(identifier);
    });
    return pushButton;
}
#include "moc_searchmessagecommandbuttonwidget.cpp"
