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
         i18nc("@info:tooltip", "Finds Messages sent by you."),
         SearchMessageCommand::mustBeUnique(SearchMessageCommand::FromMe)},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::FromUserName),
         i18nc("@action:button", "From"),
         i18nc("@info:tooltip", "Finds Messages from a specific user. (Use the username format without space)"),
         SearchMessageCommand::mustBeUnique(SearchMessageCommand::FromUserName)},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::Order),
         i18nc("@action:button", "Order:desc"),
         i18nc("@info:tooltip", "Sorts message by descending timestamp"),
         SearchMessageCommand::mustBeUnique(SearchMessageCommand::Order)},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::Before),
         i18nc("@action:button", "Before"),
         i18nc("@info:tooltip", "Filter by date"),
         SearchMessageCommand::mustBeUnique(SearchMessageCommand::Before)},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::After),
         i18nc("@action:button", "After"),
         i18nc("@info:tooltip", "Filter by date"),
         SearchMessageCommand::mustBeUnique(SearchMessageCommand::After)},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::Day),
         i18nc("@action:button", "Day"),
         i18nc("@info:tooltip", "Filter by date"),
         SearchMessageCommand::mustBeUnique(SearchMessageCommand::Day)},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::HasLocation),
         i18nc("@action:button", "Has Location"),
         i18nc("@info:tooltip", "Finds messages that include a location."),
         SearchMessageCommand::mustBeUnique(SearchMessageCommand::HasLocation)},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::HasUrl),
         i18nc("@action:button", "Has Url"),
         i18nc("@info:tooltip", "Finds messages that contain a link"),
         SearchMessageCommand::mustBeUnique(SearchMessageCommand::HasUrl)},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::IsPinned),
         i18nc("@action:button", "Is Pinned"),
         i18nc("@info:tooltip", "Displays pinned messages in the current room."),
         SearchMessageCommand::mustBeUnique(SearchMessageCommand::IsPinned)},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::HasStar),
         i18nc("@action:button", "Has Star"),
         i18nc("@info:tooltip", "Shows messages you've starred."),
         SearchMessageCommand::mustBeUnique(SearchMessageCommand::HasStar)},
    };
    return buttonInfo;
}

QPushButton *SearchMessageCommandButtonWidget::createPushButton(const SearchMessageCommandButtonWidget::ButtonInfo &info)
{
    auto pushButton = new QPushButton(info.i18n, this);
    pushButton->setObjectName(info.identifier);
    pushButton->setToolTip(info.toolTip);
    const QString identifier = info.identifier;
    const bool needSpace = info.needSpace;
    connect(pushButton, &QPushButton::clicked, this, [this, identifier, needSpace]() {
        Q_EMIT insertSearchString(needSpace ? identifier + u' ' : identifier);
    });
    return pushButton;
}
#include "moc_searchmessagecommandbuttonwidget.cpp"
