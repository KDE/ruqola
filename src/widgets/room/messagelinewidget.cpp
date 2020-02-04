/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "messagelineedit.h"
#include "messagelinewidget.h"
#include "misc/emoticonmenuwidget.h"
#include "dialogs/uploadfiledialog.h"
#include <QPointer>
#include <QHBoxLayout>
#include <QToolButton>
#include <QMenu>
#include <QWidgetAction>

MessageLineWidget::MessageLineWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    mSendFile = new QToolButton(this);
    mSendFile->setObjectName(QStringLiteral("mSendFile"));
    mainLayout->addWidget(mSendFile);
    mSendFile->setIcon(QIcon::fromTheme(QStringLiteral("document-send-symbolic")));
    connect(mSendFile, &QToolButton::clicked, this, &MessageLineWidget::slotSendFile);

    mMessageLineEdit = new MessageLineEdit(this);
    mMessageLineEdit->setObjectName(QStringLiteral("mMessageLineEdit"));
    mainLayout->addWidget(mMessageLineEdit);
    connect(mMessageLineEdit, &MessageLineEdit::sendMessage, this, &MessageLineWidget::sendMessage);

    mEmoticonButton = new QToolButton(this);
    mEmoticonButton->setObjectName(QStringLiteral("mEmoticonButton"));
    mEmoticonButton->setIcon(QIcon::fromTheme(QStringLiteral("face-smile")));
    mEmoticonButton->setPopupMode(QToolButton::InstantPopup);
    mainLayout->addWidget(mEmoticonButton);

    mSendMessageButton = new QToolButton(this);
    mSendMessageButton->setObjectName(QStringLiteral("mSendMessageButton"));
    mSendMessageButton->setIcon(QIcon::fromTheme(QStringLiteral("mail-sent"))); //Change it when we edit message
    mainLayout->addWidget(mSendMessageButton);

    QMenu *emoticonMenu = new QMenu(this);
    QWidgetAction *action = new QWidgetAction(emoticonMenu);
    mEmoticonMenuWidget = new EmoticonMenuWidget(this);
    action->setDefaultWidget(mEmoticonMenuWidget);
    emoticonMenu->addAction(action);
    mEmoticonButton->setMenu(emoticonMenu);
    connect(mEmoticonMenuWidget, &EmoticonMenuWidget::insertEmoticons, mMessageLineEdit, &MessageLineEdit::insert);

    setFocusProxy(mMessageLineEdit);
}

MessageLineWidget::~MessageLineWidget()
{
}

void MessageLineWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    mEmoticonMenuWidget->setCurrentRocketChatAccount(account);
}

void MessageLineWidget::setText(const QString &text)
{
    mMessageLineEdit->setText(text);
}

QString MessageLineWidget::text() const
{
    return mMessageLineEdit->text();
}

void MessageLineWidget::slotSendFile()
{
    QPointer<UploadFileDialog> dlg = new UploadFileDialog(this);
    if (dlg->exec()) {
        const UploadFileDialog::UploadFileInfo result = dlg->fileInfo();
        Q_EMIT sendFile(result);
    }
    delete dlg;
}
