/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "offlinewidget.h"
#include <KLocalizedString>
using namespace Qt::Literals::StringLiterals;
OffLineWidget::OffLineWidget(QWidget *parent)
    : KMessageWidget{parent}
{
    setPosition(KMessageWidget::Header);
    setVisible(false);
    setCloseButtonVisible(false);
    setMessageType(Warning);
    setText(u"<qt><b>"_s + i18n("Network is offline mode.") + u"</b><qt>"_s);
    setTextFormat(Qt::RichText);
}

OffLineWidget::~OffLineWidget() = default;

#include "moc_offlinewidget.cpp"
