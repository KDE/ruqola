/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "newmessageindicator.h"
#include <KLocalizedString>
#include <KMessageWidget>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
NewMessageIndicator::NewMessageIndicator(QWidget *parent)
    : QWidget{parent}
    , mMessageWidget(new KMessageWidget(this))
{
    setObjectName(u"NewMessageIndicator"_s);
    setFocusPolicy(Qt::NoFocus);
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(u"mainLayout"_s);

    mMessageWidget->setObjectName(u"messageWidget"_s);
    // mMessageWidget->setCloseButtonVisible(false);

    mainLayout->addWidget(mMessageWidget);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    // by default, hide widgets
    // mMessageWidget->hide();
    // hide();

    mMessageWidget->setPosition(KMessageWidget::Inline);
    mMessageWidget->setText(i18n("New Message"));
    // TODO mMessageWidget->setIcon(QIcon::fromTheme(u""_s));

    mMessageWidget->setMessageType(KMessageWidget::Information);
}

NewMessageIndicator::~NewMessageIndicator() = default;

void NewMessageIndicator::showNewMessageIndicator(bool visible)
{
    if (visible) {
        show();
    } else {
        hide();
    }
}

#include "moc_newmessageindicator.cpp"
