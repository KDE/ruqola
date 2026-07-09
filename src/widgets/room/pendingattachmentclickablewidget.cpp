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
    : QWidget{parent}
{
    const QMimeDatabase db;
    const QFileInfo info(url.toLocalFile());
    const QMimeType mimeType = db.mimeTypeForFile(info);
    const QString mimeTypeIconName = mimeType.iconName();

    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});

    mPendingAttachmentInfo.fileUrl = url;
    mPendingAttachmentInfo.fileName = info.fileName();

    auto iconLabel = new QLabel(this);
    iconLabel->setObjectName(u"iconLabel"_s);
    const QString mimeTypeIconPath = KIconLoader::global()->iconPath(mimeTypeIconName, KIconLoader::Small);
    iconLabel->setPixmap(QPixmap(mimeTypeIconPath));
    mainLayout->addWidget(iconLabel, 0, Qt::AlignTop);

    auto vboxLayout = new QVBoxLayout;
    vboxLayout->setContentsMargins({});
    vboxLayout->setSpacing(0);
    mainLayout->addLayout(vboxLayout);

    auto nameLabel = new QLabel(this);
    nameLabel->setObjectName(u"nameLabel"_s);
    const QString elided = nameLabel->fontMetrics().elidedText(mPendingAttachmentInfo.fileName, Qt::ElideMiddle, 120);
    nameLabel->setText(elided);
    nameLabel->setToolTip(url.toString());
    vboxLayout->addWidget(nameLabel);

    auto sizeLabel = new QLabel(this);
    sizeLabel->setObjectName(u"sizeLabel"_s);
    sizeLabel->setText(KFormat().formatByteSize(info.size()));
    vboxLayout->addWidget(sizeLabel);

    auto removeBtn = new QToolButton(this);
    removeBtn->setObjectName(u"removeBtn"_s);
    removeBtn->setAutoRaise(true);
    removeBtn->setIcon(QIcon::fromTheme(u"edit-delete-remove"_s));
    removeBtn->setFixedSize(18, 18);
    removeBtn->setIconSize(QSize(12, 12));
    removeBtn->setToolTip(i18nc("@info:tooltip", "Remove attachment"));
    connect(removeBtn, &QToolButton::clicked, this, [this, url]() {
        Q_EMIT removeAttachment(url);
    });
    mainLayout->addWidget(removeBtn, 0, Qt::AlignTop);
}

PendingAttachmentClickableWidget::~PendingAttachmentClickableWidget() = default;

PendingAttachmentClickableWidget::PendingAttachmentInfo PendingAttachmentClickableWidget::pendingAttachmentInfo() const
{
    return mPendingAttachmentInfo;
}

#include "moc_pendingattachmentclickablewidget.cpp"
