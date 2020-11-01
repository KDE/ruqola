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

#include "showattachmentcombobox.h"
#include "showattachmentwidget.h"
#include "attachment/listattachmentdelegate.h"
#include "model/filesforroomfilterproxymodel.h"
#include "misc/lineeditcatchreturnkey.h"
#include <QLineEdit>
#include <QVBoxLayout>
#include <KLocalizedString>
#include <QLabel>
#include <QListView>

ShowAttachmentWidget::ShowAttachmentWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto *searchAttachmentLayout = new QHBoxLayout;
    searchAttachmentLayout->setObjectName(QStringLiteral("searchAttachmentLayout"));
    searchAttachmentLayout->setContentsMargins({});
    mainLayout->addLayout(searchAttachmentLayout);

    mSearchAttachmentFileLineEdit = new QLineEdit(this);
    mSearchAttachmentFileLineEdit->setObjectName(QStringLiteral("mSearchAttachmentFileLineEdit"));
    mSearchAttachmentFileLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchAttachmentFileLineEdit, this);
    mSearchAttachmentFileLineEdit->setPlaceholderText(i18n("Search Attachments..."));
    connect(mSearchAttachmentFileLineEdit, &QLineEdit::textChanged, this, &ShowAttachmentWidget::slotSearchMessageTextChanged);
    searchAttachmentLayout->addWidget(mSearchAttachmentFileLineEdit);

    mAttachmentCombobox = new ShowAttachmentComboBox(this);
    mAttachmentCombobox->setObjectName(QStringLiteral("mAttachmentCombobox"));
    searchAttachmentLayout->addWidget(mAttachmentCombobox);
    connect(mAttachmentCombobox, qOverload<int>(&ShowAttachmentComboBox::currentIndexChanged), this, &ShowAttachmentWidget::slotChangeAttachmentType);

    mInfo = new QLabel(this);
    mInfo->setObjectName(QStringLiteral("mInfo"));
    mInfo->setTextFormat(Qt::RichText);
    mInfo->setContextMenuPolicy(Qt::NoContextMenu);
    mainLayout->addWidget(mInfo);
    QFont labFont = mInfo->font();
    labFont.setBold(true);
    mInfo->setFont(labFont);
    connect(mInfo, &QLabel::linkActivated, this, &ShowAttachmentWidget::loadMoreFileAttachment);

    mListAttachment = new QListView(this);
    mListAttachment->setObjectName(QStringLiteral("mListAttachment"));
    mainLayout->addWidget(mListAttachment);
    auto *delegate = new ListAttachmentDelegate(this);
    connect(delegate, &ListAttachmentDelegate::deleteAttachment, this, &ShowAttachmentWidget::deleteAttachment);
    mListAttachment->setItemDelegate(delegate);
}

ShowAttachmentWidget::~ShowAttachmentWidget()
{
}

void ShowAttachmentWidget::slotChangeAttachmentType(int index)
{
    mModel->setTypeGroup(mAttachmentCombobox->itemData(index).toString());
}

void ShowAttachmentWidget::setModel(FilesForRoomFilterProxyModel *model)
{
    mModel = model;
    mModel->resetTypeGroup();
    mListAttachment->setModel(model);
    connect(mModel, &FilesForRoomFilterProxyModel::hasFullListChanged, this, &ShowAttachmentWidget::updateLabel);
    connect(mModel, &FilesForRoomFilterProxyModel::totalChanged, this, &ShowAttachmentWidget::updateLabel);
    connect(mModel, &FilesForRoomFilterProxyModel::loadingInProgressChanged, this, &ShowAttachmentWidget::updateLabel);
    updateLabel();
}

void ShowAttachmentWidget::slotSearchMessageTextChanged(const QString &str)
{
    mModel->setFilterString(str);
}

void ShowAttachmentWidget::updateLabel()
{
    mInfo->setText(mModel->attachmentCount() == 0 ? i18n("No Attachments found") : displayShowMessageInRoom());
}

QString ShowAttachmentWidget::displayShowMessageInRoom() const
{
    QString displayMessageStr = i18np("%1 Attachment in room (Total: %2)", "%1 Attachments in room (Total: %2)", mModel->attachmentCount(), mModel->total());
    if (!mModel->hasFullList()) {
        displayMessageStr += QStringLiteral(" <a href=\"loadmoreelement\">%1</a>").arg(i18n("(Click here for Loading more...)"));
    }
    return displayMessageStr;
}
