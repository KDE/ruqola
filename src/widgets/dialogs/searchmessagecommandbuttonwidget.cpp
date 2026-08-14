/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchmessagecommandbuttonwidget.h"
#include <KLocalizedString>
#include <QPushButton>
#include <TextAddonsWidgets/TextAddonsWidgetFlowLayout>
using namespace Qt::Literals::StringLiterals;
SearchMessageCommandButtonWidget::SearchMessageCommandButtonWidget(QWidget *parent)
    : QWidget{parent}
{
    auto flowLayout = new TextAddonsWidgets::TextAddonsWidgetFlowLayout(this);
    flowLayout->setObjectName(u"flowLayout"_s);
    flowLayout->setContentsMargins({});
    flowLayout->setHorizontalSpacing(0);
    flowLayout->setVerticalSpacing(0);
    const QList<SearchMessageCommandButtonWidget::ButtonInfo> buttonsList = fillCommandLineText();
    Q_ASSERT(!buttonsList.isEmpty());
    for (const auto &info : buttonsList) {
        flowLayout->addWidget(createPushButton(info));
    }
    hide();
}

SearchMessageCommandButtonWidget::~SearchMessageCommandButtonWidget() = default;

SearchMessageCommandButtonWidget::ButtonInfo
SearchMessageCommandButtonWidget::createButtonInfo(SearchMessageCommand::SearchMessageCommandType type, const QString &title, const QString &tooltip) const
{
    return {SearchMessageCommand::generateCommandText(type), title, tooltip, SearchMessageCommand::mustBeUnique(type), SearchMessageCommand::needSpace(type)};
}

QList<SearchMessageCommandButtonWidget::ButtonInfo> SearchMessageCommandButtonWidget::fillCommandLineText() const
{
    const QList<SearchMessageCommandButtonWidget::ButtonInfo> buttonInfo = {
        createButtonInfo(SearchMessageCommand::FromMe, i18nc("@action:button", "From Me"), i18nc("@info:tooltip", "Finds Messages sent by you.")),
        createButtonInfo(SearchMessageCommand::FromUserName,
                         i18nc("@action:button", "From"),
                         i18nc("@info:tooltip", "Finds Messages from a specific user. (Use the username format without space)")),
        createButtonInfo(SearchMessageCommand::Order, i18nc("@action:button", "Order:desc"), i18nc("@info:tooltip", "Sorts message by descending timestamp")),
        createButtonInfo(SearchMessageCommand::Before,
                         i18nc("@action:button", "Before"),
                         i18nc("@info:tooltip", "Filter by date as %1", u"before:dd/mm/yyyy"_s)),
        createButtonInfo(SearchMessageCommand::After, i18nc("@action:button", "After"), i18nc("@info:tooltip", "Filter by date as %1", u"after:dd/mm/yyyy"_s)),
        createButtonInfo(SearchMessageCommand::Day, i18nc("@action:button", "Day"), i18nc("@info:tooltip", "Filter by date as %1", u"on:dd/mm/yyyy"_s)),
        createButtonInfo(SearchMessageCommand::HasLocation,
                         i18nc("@action:button", "Has Location"),
                         i18nc("@info:tooltip", "Finds messages that include a location.")),
        createButtonInfo(SearchMessageCommand::HasUrl, i18nc("@action:button", "Has Url"), i18nc("@info:tooltip", "Finds messages that contain a link")),
        createButtonInfo(SearchMessageCommand::IsPinned,
                         i18nc("@action:button", "Is Pinned"),
                         i18nc("@info:tooltip", "Displays pinned messages in the current room.")),
        createButtonInfo(SearchMessageCommand::HasStar, i18nc("@action:button", "Has Star"), i18nc("@info:tooltip", "Shows messages you've starred.")),
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
    const bool unique = info.unique;
    connect(pushButton, &QPushButton::clicked, this, [this, identifier, needSpace, unique]() {
        Q_EMIT insertSearchString(needSpace, identifier, unique);
    });
    return pushButton;
}
#include "moc_searchmessagecommandbuttonwidget.cpp"
