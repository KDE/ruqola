/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "whatsnewmessagewidget.h"

#include "whatsnewdialog.h"
#include <KLocalizedString>
#include <QCoreApplication>

using namespace Qt::Literals::StringLiterals;
WhatsNewMessageWidget::WhatsNewMessageWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(true);
    setMessageType(Information);
    setTextFormat(Qt::RichText);
    setText(i18n("What's new in %2. %1", u"<a href=\"show_whats_new\">%1</a>"_s.arg(i18n("(Show News)")), QCoreApplication::applicationName()));
    setPosition(KMessageWidget::Header);
    connect(this, &KMessageWidget::linkActivated, this, &WhatsNewMessageWidget::slotLinkActivated);
}

WhatsNewMessageWidget::~WhatsNewMessageWidget() = default;

void WhatsNewMessageWidget::slotLinkActivated(const QString &contents)
{
    if (contents == "show_whats_new"_L1) {
        WhatsNewDialog dlg(this);
        dlg.updateInformations();
        dlg.exec();
    }
}

#include "moc_whatsnewmessagewidget.cpp"
