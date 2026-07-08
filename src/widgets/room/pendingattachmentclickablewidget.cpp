/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "pendingattachmentclickablewidget.h"
#include <KLocalizedString>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
using namespace Qt::Literals::StringLiterals;
PendingAttachmentClickableWidget::PendingAttachmentClickableWidget(const QString &fileName, QWidget *parent)
    : QWidget{parent}
{
    auto chip = new QFrame(this);
    chip->setFrameShape(QFrame::StyledPanel);
    chip->setFrameShadow(QFrame::Raised);

    auto chipLayout = new QHBoxLayout(chip);
    chipLayout->setContentsMargins(4, 1, 2, 1);
    chipLayout->setSpacing(3);

    auto iconLabel = new QLabel(chip);
    iconLabel->setPixmap(QIcon::fromTheme(u"mail-attachment-symbolic"_s).pixmap(14, 14));
    chipLayout->addWidget(iconLabel);

    auto nameLabel = new QLabel(chip);
    const QString elided = nameLabel->fontMetrics().elidedText(fileName, Qt::ElideMiddle, 120);
    nameLabel->setText(elided);
    nameLabel->setToolTip(fileName);
    chipLayout->addWidget(nameLabel);

    auto removeBtn = new QToolButton(chip);
    removeBtn->setAutoRaise(true);
    removeBtn->setIcon(QIcon::fromTheme(u"edit-delete-remove"_s));
    removeBtn->setFixedSize(18, 18);
    removeBtn->setIconSize(QSize(12, 12));
    removeBtn->setToolTip(i18nc("@info:tooltip", "Remove attachment"));
    connect(removeBtn, &QToolButton::clicked, this, [this, fileName]() {
        Q_EMIT removeAttachment(fileName);
    });
    chipLayout->addWidget(removeBtn);
}

PendingAttachmentClickableWidget::~PendingAttachmentClickableWidget() = default;
