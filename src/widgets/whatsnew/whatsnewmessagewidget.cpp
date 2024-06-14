/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "whatsnewmessagewidget.h"
using namespace Qt::Literals::StringLiterals;

#include "whatsnewdialog.h"
#include <KLocalizedString>
#include <QCoreApplication>

WhatsNewMessageWidget::WhatsNewMessageWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(true);
    setMessageType(Information);
    setText(i18n("What's new in %2. %1",
                 QStringLiteral("<a href=\"show_whats_new\">%1</a>").arg(i18n("(Show News in Ruqola)")),
                 QCoreApplication::applicationName()));
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
