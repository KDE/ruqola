/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "messagelineextratoolbutton.h"
#include <KLocalizedString>
using namespace Qt::Literals::StringLiterals;
MessageLineExtraToolButton::MessageLineExtraToolButton(QWidget *parent)
    : QToolButton(parent)
{
    setIcon(QIcon::fromTheme(u"list-add"_s));
    setToolTip(i18nc("@info:tooltip", "More Actions"));
    setVisible(false);
    setAutoRaise(true);
}

MessageLineExtraToolButton::~MessageLineExtraToolButton() = default;

#include "moc_messagelineextratoolbutton.cpp"
