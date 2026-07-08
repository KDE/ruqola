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

void PendingAttachmentWidget::setAttachments(const QStringList &fileNames)
{
    mFlowLayout->clearAndDeleteWidgets();

    for (const QString &fileName : fileNames) {
        if (mMap.contains(fileName)) {
            continue;
        }
        auto clickableWidget = new PendingAttachmentClickableWidget(fileName, this);
        connect(clickableWidget, &PendingAttachmentClickableWidget::removeAttachment, this, &PendingAttachmentWidget::slotRemoveAttachment);
        mFlowLayout->addWidget(clickableWidget);
        mMap.insert(fileName, clickableWidget);
    }
    setVisible(!mMap.isEmpty());
}

void PendingAttachmentWidget::slotRemoveAttachment(const QString &fileName)
{
    PendingAttachmentClickableWidget *clickableWidget = mMap.value(fileName);
    if (clickableWidget) {
        const int index = mFlowLayout->indexOf(clickableWidget);
        if (index != -1) {
            mFlowLayout->deleteLater();
            delete mFlowLayout->takeAt(index);
            mMap.remove(fileName);
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
}

#include "moc_pendingattachmentwidget.cpp"
