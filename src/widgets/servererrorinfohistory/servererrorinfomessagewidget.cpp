/*
   SPDX-FileCopyrightText: 2023-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "servererrorinfomessagewidget.h"

#include "accountmanager.h"
#include "ruqola.h"
#include "servererrorinfohistory/servererrorinfomessagehistorydialog.h"
#include <KLocalizedString>

using namespace Qt::Literals::StringLiterals;
ServerErrorInfoMessageWidget::ServerErrorInfoMessageWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setCloseButtonVisible(true);
    setMessageType(Error);
    setTextFormat(Qt::RichText);
    setPosition(KMessageWidget::Header);

    setText(i18n("Server errors reported. %1", u"<a href=\"show_errors\">%1</a>"_s.arg(i18n("(Show Errors)"))));
    connect(this, &KMessageWidget::linkActivated, this, &ServerErrorInfoMessageWidget::slotLinkActivated);
}

ServerErrorInfoMessageWidget::~ServerErrorInfoMessageWidget() = default;

void ServerErrorInfoMessageWidget::slotLinkActivated(const QString &contents)
{
    if (contents == "show_errors"_L1) {
        ServerErrorInfoMessageHistoryDialog dlg(this);
        dlg.addServerList(Ruqola::self()->accountManager()->accountDisplayInfoSorted());
        dlg.exec();
    }
}

#include "moc_servererrorinfomessagewidget.cpp"
