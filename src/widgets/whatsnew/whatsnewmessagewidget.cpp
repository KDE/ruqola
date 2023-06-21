/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "whatsnewmessagewidget.h"
#include "whatsnewdialog.h"
#include <KLocalizedString>

WhatsNewMessageWidget::WhatsNewMessageWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(true);
    setMessageType(Information);
    setText(i18n("What's new in Ruqola. %1", QStringLiteral("<a href=\"show_whats_new\">%1</a>").arg(i18n("(Show News in Ruqola)"))));
    connect(this, &KMessageWidget::linkActivated, this, &WhatsNewMessageWidget::slotLinkActivated);
}

WhatsNewMessageWidget::~WhatsNewMessageWidget() = default;

void WhatsNewMessageWidget::slotLinkActivated(const QString &contents)
{
    if (contents == QLatin1String("show_whats_new")) {
        WhatsNewDialog dlg(this);
        dlg.updateInformations();
        dlg.exec();
    }
}

#include "moc_whatsnewmessagewidget.cpp"
