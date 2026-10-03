/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "pendingattachmentwidget.h"
#include "pendingattachmentclickablewidget.h"
#include <KLocalizedString>
#include <QFileInfo>
#include <QFrame>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QToolButton>
#include <TextAddonsWidgets/TextAddonsWidgetFlowLayout>

using namespace Qt::Literals::StringLiterals;

PendingAttachmentWidget::PendingAttachmentWidget(QWidget *parent)
    : QWidget(parent)
    , mFlowLayout(new TextAddonsWidgets::TextAddonsWidgetFlowLayout(this))
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

void PendingAttachmentWidget::addAttachment(const AccountRoomSettings::PendingAttachmentInfo &info)
{
    const QUrl url = info.fileUrl;
    if (mMap.contains(url)) {
        return;
    }
    auto clickableWidget = new PendingAttachmentClickableWidget(info, this);
    connect(clickableWidget, &PendingAttachmentClickableWidget::removeAttachment, this, &PendingAttachmentWidget::slotRemoveAttachment);
    mFlowLayout->addWidget(clickableWidget);
    mMap.insert(url, clickableWidget);
    updateAttachments();
}

void PendingAttachmentWidget::clearAttachments()
{
    // clearAndDeleteWidgets() defers the destruction (deleteLater()), so hide the widgets right away:
    // otherwise they keep painting over the ones added just below until the event loop spins.
    for (int i = 0, nbItems = mFlowLayout->count(); i < nbItems; ++i) {
        if (QWidget *w = mFlowLayout->itemAt(i)->widget()) {
            w->hide();
        }
    }
    mFlowLayout->clearAndDeleteWidgets();
    // The widgets are gone: dropping the map entries too, otherwise attachmentsInfo() would
    // dereference dangling pointers and addAttachment() would refuse to re-add a known url.
    mMap.clear();
}

void PendingAttachmentWidget::setAttachments(const QList<QUrl> &urls)
{
    clearAttachments();

    for (const QUrl &url : urls) {
        addAttachment(url);
    }
    updateAttachments();
}

void PendingAttachmentWidget::updateAttachments()
{
    const bool result = hasAttachments();
    setVisible(result);
    Q_EMIT attachmentsChanged(result);
}

void PendingAttachmentWidget::slotRemoveAttachment(const QUrl &url)
{
    if (PendingAttachmentClickableWidget *clickableWidget = mMap.take(url)) {
        const int index = mFlowLayout->indexOf(clickableWidget);
        if (index != -1) {
            delete mFlowLayout->takeAt(index);
        }
        clickableWidget->hide();
        clickableWidget->deleteLater();
        updateAttachments();
    }
}

bool PendingAttachmentWidget::hasAttachments() const
{
    return !mMap.isEmpty();
}

void PendingAttachmentWidget::clear()
{
    clearAttachments();
    hide();
    updateAttachments();
}

void PendingAttachmentWidget::setPendingAttachmentInfos(const QList<AccountRoomSettings::PendingAttachmentInfo> &infos)
{
    clearAttachments();

    for (const AccountRoomSettings::PendingAttachmentInfo &info : infos) {
        if (verifyExistingFile(info.fileUrl)) {
            addAttachment(info);
        }
    }
    // addAttachment() updates on each insertion, but an empty list must still hide the widget
    // and tell the composer that there is nothing left to send.
    updateAttachments();
}

bool PendingAttachmentWidget::verifyExistingFile(const QUrl &fileUrl)
{
    if (fileUrl.isLocalFile()) {
        const QFileInfo f(fileUrl.toLocalFile());
        return f.exists();
    }
    return true;
}

QList<AccountRoomSettings::PendingAttachmentInfo> PendingAttachmentWidget::attachmentsInfo() const
{
    QList<AccountRoomSettings::PendingAttachmentInfo> lst;
    lst.reserve(mMap.count());
    for (auto i = mMap.cbegin(), end = mMap.cend(); i != end; ++i) {
        lst += i.value()->pendingAttachmentInfo();
    }
    return lst;
}

#include "moc_pendingattachmentwidget.cpp"
