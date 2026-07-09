/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "pendingattachmentwidget.h"
#include "common/flowlayout.h"
#include "pendingattachmentclickablewidget.h"
#include <KLocalizedString>
#include <QFrame>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QToolButton>

using namespace Qt::Literals::StringLiterals;

PendingAttachmentWidget::PendingAttachmentWidget(QWidget *parent)
    : QWidget(parent)
    , mFlowLayout(new FlowLayout(this))
{
    setAutoFillBackground(true);
    mFlowLayout->setSpacing(4);
}

PendingAttachmentWidget::~PendingAttachmentWidget() = default;

void PendingAttachmentWidget::addAttachment(const QUrl &url)
{
    if (mMap.contains(url)) {
        return;
    }
    auto clickableWidget = new PendingAttachmentClickableWidget(url, this);
    connect(clickableWidget, &PendingAttachmentClickableWidget::removeAttachment, this, &PendingAttachmentWidget::slotRemoveAttachment);
    mFlowLayout->addWidget(clickableWidget);
    mMap.insert(url, clickableWidget);
    updateAttachments();
}

void PendingAttachmentWidget::setAttachments(const QList<QUrl> &urls)
{
    mFlowLayout->clearAndDeleteWidgets();

    for (const QUrl &url : urls) {
        addAttachment(url);
    }
}

void PendingAttachmentWidget::updateAttachments()
{
    const bool result = hasAttachments();
    setVisible(result);
    Q_EMIT attachmentsChanged(result);
}

void PendingAttachmentWidget::slotRemoveAttachment(const QUrl &url)
{
    PendingAttachmentClickableWidget *clickableWidget = mMap.value(url);
    if (clickableWidget) {
        const int index = mFlowLayout->indexOf(clickableWidget);
        if (index != -1) {
            clickableWidget->deleteLater();
            delete mFlowLayout->takeAt(index);
            mMap.remove(url);
            updateAttachments();
        }
    }
}

bool PendingAttachmentWidget::hasAttachments() const
{
    return !mMap.isEmpty();
}

void PendingAttachmentWidget::clear()
{
    mFlowLayout->clearAndDeleteWidgets();
    mMap.clear();
    hide();
}

QList<AccountRoomSettings::PendingAttachmentInfo> PendingAttachmentWidget::attachmentsInfo() const
{
    QList<AccountRoomSettings::PendingAttachmentInfo> lst;
    for (auto i = mMap.cbegin(), end = mMap.cend(); i != end; ++i) {
        lst += i.value()->pendingAttachmentInfo();
    }
    return lst;
}

#include "moc_pendingattachmentwidget.cpp"
