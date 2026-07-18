/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "pendingattachmentclickablewidget.h"
#include <KFormat>
#include <KIconLoader>
#include <KLocalizedString>
#include <QFileInfo>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QMimeDatabase>
#include <QMimeType>
#include <QToolButton>
using namespace Qt::Literals::StringLiterals;

PendingAttachmentClickableWidget::PendingAttachmentClickableWidget(const QUrl &url, QWidget *parent)
    : PendingAttachmentClickableWidget(parent)
{
    setUrl(url);
}

PendingAttachmentClickableWidget::PendingAttachmentClickableWidget(const AccountRoomSettings::PendingAttachmentInfo &info, QWidget *parent)
    : PendingAttachmentClickableWidget(parent)
{
    setPendingAttachmentInfo(info);
}

PendingAttachmentClickableWidget::PendingAttachmentClickableWidget(QWidget *parent)
    : QWidget{parent}
    , mIconLabel(new QLabel(this))
    , mNameLabel(new QLabel(this))
    , mSizeLabel(new QLabel(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mIconLabel->setObjectName(u"iconLabel"_s);
    mainLayout->addWidget(mIconLabel, 0, Qt::AlignTop);

    auto vboxLayout = new QVBoxLayout;
    vboxLayout->setContentsMargins({});
    vboxLayout->setSpacing(0);
    mainLayout->addLayout(vboxLayout);

    mNameLabel->setObjectName(u"nameLabel"_s);
    vboxLayout->addWidget(mNameLabel);

    mSizeLabel->setObjectName(u"sizeLabel"_s);
    vboxLayout->addWidget(mSizeLabel);

    auto removeBtn = new QToolButton(this);
    removeBtn->setObjectName(u"removeBtn"_s);
    removeBtn->setAutoRaise(true);
    removeBtn->setIcon(QIcon::fromTheme(u"edit-delete-remove"_s));
    removeBtn->setFixedSize(18, 18);
    removeBtn->setIconSize(QSize(12, 12));
    removeBtn->setToolTip(i18nc("@info:tooltip", "Remove attachment"));
    connect(removeBtn, &QToolButton::clicked, this, [this]() {
        Q_EMIT removeAttachment(mPendingAttachmentInfo.fileUrl);
    });
    mainLayout->addWidget(removeBtn, 0, Qt::AlignTop);
}

PendingAttachmentClickableWidget::~PendingAttachmentClickableWidget() = default;

void PendingAttachmentClickableWidget::setPendingAttachmentInfo(const AccountRoomSettings::PendingAttachmentInfo &pendingAttachment)
{
    mPendingAttachmentInfo = pendingAttachment;
    const QMimeDatabase db;
    const QFileInfo info(mPendingAttachmentInfo.fileUrl.toLocalFile());
    updateInfo(info);
}

void PendingAttachmentClickableWidget::setUrl(const QUrl &url)
{
    const QFileInfo info(url.toLocalFile());
    mPendingAttachmentInfo.fileUrl = url;
    mPendingAttachmentInfo.fileName = info.fileName();
    updateInfo(info);
}

void PendingAttachmentClickableWidget::updateInfo(const QFileInfo &info)
{
    const QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForFile(info);
    const QString mimeTypeIconName = mimeType.iconName();
    const QString mimeTypeIconPath = KIconLoader::global()->iconPath(mimeTypeIconName, KIconLoader::MainToolbar);
    mIconLabel->setPixmap(QPixmap(mimeTypeIconPath));
    const QString elided = mNameLabel->fontMetrics().elidedText(mPendingAttachmentInfo.fileName, Qt::ElideMiddle, 120);
    mNameLabel->setText(elided);
    mNameLabel->setToolTip(mPendingAttachmentInfo.fileUrl.toString());
    mSizeLabel->setText(KFormat().formatByteSize(info.size()));
}

AccountRoomSettings::PendingAttachmentInfo PendingAttachmentClickableWidget::pendingAttachmentInfo() const
{
    return mPendingAttachmentInfo;
}

#include "moc_pendingattachmentclickablewidget.cpp"
