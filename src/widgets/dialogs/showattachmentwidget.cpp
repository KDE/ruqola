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

#include "showattachmentwidget.h"
#include "listattachmentdelegate.h"
#include "model/filesforroomfilterproxymodel.h"
#include <KLineEdit>
#include <QVBoxLayout>
#include <KLocalizedString>
#include <QLabel>
#include <QListView>

ShowAttachmentWidget::ShowAttachmentWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mSearchAttachmentFileLineEdit = new KLineEdit(this);
    mSearchAttachmentFileLineEdit->setObjectName(QStringLiteral("mSearchAttachmentFileLineEdit"));
    mSearchAttachmentFileLineEdit->setClearButtonEnabled(true);
    mSearchAttachmentFileLineEdit->setTrapReturnKey(true);
    mSearchAttachmentFileLineEdit->setPlaceholderText(i18n("Search Attachments..."));
    connect(mSearchAttachmentFileLineEdit, &KLineEdit::textChanged, this, &ShowAttachmentWidget::slotSearchMessageTextChanged);
    mainLayout->addWidget(mSearchAttachmentFileLineEdit);

    mInfo = new QLabel(this);
    mInfo->setObjectName(QStringLiteral("mInfo"));
    mInfo->setTextFormat(Qt::RichText);
    mainLayout->addWidget(mInfo);
    QFont labFont = mInfo->font();
    labFont.setBold(true);
    mInfo->setFont(labFont);
    connect(mInfo, &QLabel::linkActivated, this, &ShowAttachmentWidget::loadMoreFileAttachment);

    mListAttachment = new QListView(this);
    mListAttachment->setObjectName(QStringLiteral("mListAttachment"));
    mainLayout->addWidget(mListAttachment);
    mListAttachment->setItemDelegate(new ListAttachmentDelegate(this));

    //TODO need to update label !!!
}

ShowAttachmentWidget::~ShowAttachmentWidget()
{
}

void ShowAttachmentWidget::setModel(FilesForRoomFilterProxyModel *model)
{
    mModel = model;
    mListAttachment->setModel(model);
    updateLabel();
}

void ShowAttachmentWidget::slotSearchMessageTextChanged(const QString &str)
{
    mModel->setFilterString(str);
}

void ShowAttachmentWidget::updateLabel()
{
    mInfo->setText(mModel->rowCount() == 0 ? i18n("No Messages found") : displayShowMessageInRoom());
}

QString ShowAttachmentWidget::displayShowMessageInRoom() const
{
    QString displayMessageStr;
    displayMessageStr = i18np("%1 Attachment in room (Total: %2)", "%1 Attachments in room (Total: %2)", mModel->rowCount(), mModel->total());
    if (!mModel->hasFullList()) {
        displayMessageStr += QStringLiteral("<a href=\"loadmoreelement\">%1</a>").arg(i18n("(Click here for Loading more...)"));
    }
    return displayMessageStr;
}
