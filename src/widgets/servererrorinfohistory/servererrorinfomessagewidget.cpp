/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfomessagewidget.h"
#include "accountmanager.h"
#include "ruqola.h"
#include "servererrorinfohistory/servererrorinfomessagehistorydialog.h"
#include <KLocalizedString>
#include <QDebug>

ServerErrorInfoMessageWidget::ServerErrorInfoMessageWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(true);
    setMessageType(Error);
#if QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    setPosition(KMessageWidget::Header);
#endif

    setText(i18n("Server errors reported. %1", QStringLiteral("<a href=\"show_errors\">%1</a>").arg(i18n("(Show Errors)"))));
    connect(this, &KMessageWidget::linkActivated, this, &ServerErrorInfoMessageWidget::slotLinkActivated);
}

ServerErrorInfoMessageWidget::~ServerErrorInfoMessageWidget() = default;

void ServerErrorInfoMessageWidget::slotLinkActivated(const QString &contents)
{
    if (contents == QLatin1String("show_errors")) {
        ServerErrorInfoMessageHistoryDialog dlg(this);
        dlg.addServerList(Ruqola::self()->accountManager()->accountNamesSorted());
        dlg.exec();
    }
}

#include "moc_servererrorinfomessagewidget.cpp"
