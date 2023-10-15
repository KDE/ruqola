/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessageinfowidget.h"
#include <KLocalizedString>
#include <QVBoxLayout>

ModerationMessageInfoWidget::ModerationMessageInfoWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
}

ModerationMessageInfoWidget::~ModerationMessageInfoWidget() = default;

#include "moc_moderationmessageinfowidget.cpp"
