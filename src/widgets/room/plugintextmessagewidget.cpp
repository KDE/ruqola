/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plugintextmessagewidget.h"
#include "ruqolautils.h"
#include <QUrl>

PluginTextMessageWidget::PluginTextMessageWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setVisible(false);
    setPosition(KMessageWidget::Header);
    setCloseButtonVisible(true);
    connect(this, &KMessageWidget::linkActivated, this, [](const QString &contents) {
        RuqolaUtils::self()->openUrl(QUrl(contents));
    });
}

PluginTextMessageWidget::~PluginTextMessageWidget() = default;

void PluginTextMessageWidget::slotShareError(const QString &message)
{
    setMessageType(KMessageWidget::MessageType::Error);
    setText(message);
    animatedShow();
}

void PluginTextMessageWidget::slotShareSuccess(const QString &message)
{
    setMessageType(KMessageWidget::MessageType::Information);
    setText(message);
    animatedShow();
}

#include "moc_plugintextmessagewidget.cpp"
