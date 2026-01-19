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
        flowLayout->addWidget(createPushButton(info.i18n, info.identifier));
    }
}

SearchMessageCommandButtonWidget::~SearchMessageCommandButtonWidget() = default;

QList<SearchMessageCommandButtonWidget::ButtonInfo> SearchMessageCommandButtonWidget::fillCommandLineText() const
{
    const QList<SearchMessageCommandButtonWidget::ButtonInfo> buttonInfo = {
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::FromMe), i18nc("@action:button", "From Me")},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::FromUserName), i18nc("@action:button", "From")},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::Order), i18nc("@action:button", "Order")},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::Before), i18nc("@action:button", "Before")},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::HasLocation), i18nc("@action:button", "Has Location")},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::HasUrl), i18nc("@action:button", "Has Url")},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::IsPinned), i18nc("@action:button", "Is Pinned")},
        {SearchMessageCommand::generateCommandText(SearchMessageCommand::HasStar), i18nc("@action:button", "Has Star")},
    };
    return buttonInfo;
}

QPushButton *SearchMessageCommandButtonWidget::createPushButton(const QString &i18nStr, const QString &commandStr)
{
    auto pushButton = new QPushButton(i18nStr, this);
    pushButton->setObjectName(commandStr);
    pushButton->setToolTip(commandStr);
    connect(pushButton, &QPushButton::clicked, this, [this, commandStr]() {
        Q_EMIT insertCommand(commandStr);
    });
    return pushButton;
}
#include "moc_searchmessagecommandbuttonwidget.cpp"
