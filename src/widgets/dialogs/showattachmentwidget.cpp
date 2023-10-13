/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showattachmentwidget.h"
#include "attachment/listattachmentdelegate.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/filesforroomfilterproxymodel.h"
#include "showattachmentcombobox.h"
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QVBoxLayout>

ShowAttachmentWidget::ShowAttachmentWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mSearchAttachmentFileLineEdit(new QLineEdit(this))
    , mAttachmentCombobox(new ShowAttachmentComboBox(this))
    , mInfo(new QLabel(this))
    , mListAttachment(new QListView(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto searchAttachmentLayout = new QHBoxLayout;
    searchAttachmentLayout->setObjectName(QStringLiteral("searchAttachmentLayout"));
    searchAttachmentLayout->setContentsMargins({});
    mainLayout->addLayout(searchAttachmentLayout);

    mSearchAttachmentFileLineEdit->setObjectName(QStringLiteral("mSearchAttachmentFileLineEdit"));
    mSearchAttachmentFileLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchAttachmentFileLineEdit, this);
    mSearchAttachmentFileLineEdit->setPlaceholderText(i18n("Search attachments..."));
    connect(mSearchAttachmentFileLineEdit, &QLineEdit::textChanged, this, &ShowAttachmentWidget::slotSearchMessageTextChanged);
    searchAttachmentLayout->addWidget(mSearchAttachmentFileLineEdit);

    mAttachmentCombobox->setObjectName(QStringLiteral("mAttachmentCombobox"));
    searchAttachmentLayout->addWidget(mAttachmentCombobox);
    connect(mAttachmentCombobox, &ShowAttachmentComboBox::currentIndexChanged, this, &ShowAttachmentWidget::slotChangeAttachmentType);

    mInfo->setObjectName(QStringLiteral("mInfo"));
    mInfo->setTextFormat(Qt::RichText);
    mInfo->setContextMenuPolicy(Qt::NoContextMenu);
    mainLayout->addWidget(mInfo);
    QFont labFont = mInfo->font();
    labFont.setBold(true);
    mInfo->setFont(labFont);
    connect(mInfo, &QLabel::linkActivated, this, &ShowAttachmentWidget::loadMoreFileAttachment);

    mListAttachment->setObjectName(QStringLiteral("mListAttachment"));
    mainLayout->addWidget(mListAttachment);
    auto delegate = new ListAttachmentDelegate(account, this);
    connect(delegate, &ListAttachmentDelegate::deleteAttachment, this, &ShowAttachmentWidget::deleteAttachment);
    mListAttachment->setItemDelegate(delegate);
}

ShowAttachmentWidget::~ShowAttachmentWidget()
{
    // Reset it
    if (mModel) {
        mModel->setFilterString(QString());
        mModel->clear();
    }
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

#include "moc_showattachmentwidget.cpp"
