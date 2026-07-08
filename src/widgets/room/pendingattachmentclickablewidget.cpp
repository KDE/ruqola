/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "pendingattachmentclickablewidget.h"
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

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});

    mPendingAttachmentInfo.fileUrl = url;
    mPendingAttachmentInfo.fileName = info.fileName();

    auto chip = new QFrame(this);
    chip->setFrameShape(QFrame::StyledPanel);
    chip->setFrameShadow(QFrame::Raised);
    mainLayout->addWidget(chip);

    auto chipLayout = new QHBoxLayout(chip);
    chipLayout->setContentsMargins(4, 1, 2, 1);
    chipLayout->setSpacing(3);

    auto iconLabel = new QLabel(chip);
    iconLabel->setObjectName(u"iconLabel"_s);
    const QString mimeTypeIconPath = KIconLoader::global()->iconPath(mimeTypeIconName, KIconLoader::Small);
    iconLabel->setPixmap(QPixmap(mimeTypeIconPath));
    chipLayout->addWidget(iconLabel);

    auto nameLabel = new QLabel(chip);
    nameLabel->setObjectName(u"nameLabel"_s);
    const QString elided = nameLabel->fontMetrics().elidedText(mPendingAttachmentInfo.fileName, Qt::ElideMiddle, 120);
    nameLabel->setText(elided);
    nameLabel->setToolTip(url.toString());
    chipLayout->addWidget(nameLabel);

    auto removeBtn = new QToolButton(chip);
    removeBtn->setObjectName(u"removeBtn"_s);
    removeBtn->setAutoRaise(true);
    removeBtn->setIcon(QIcon::fromTheme(u"edit-delete-remove"_s));
    removeBtn->setFixedSize(18, 18);
    removeBtn->setIconSize(QSize(12, 12));
    removeBtn->setToolTip(i18nc("@info:tooltip", "Remove attachment"));
    connect(removeBtn, &QToolButton::clicked, this, [this, url]() {
        Q_EMIT removeAttachment(url);
    });
    chipLayout->addWidget(removeBtn);
}

PendingAttachmentClickableWidget::~PendingAttachmentClickableWidget() = default;

PendingAttachmentClickableWidget::PendingAttachmentInfo PendingAttachmentClickableWidget::pendingAttachmentInfo() const
{
    return mPendingAttachmentInfo;
}

#include "moc_pendingattachmentclickablewidget.cpp"
