/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "threadmessagedialog.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QFrame>
#include <QStyle>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

namespace
{
static const char myThreadMessageDialogGroupName[] = "ThreadMessageDialog";
}

using namespace Qt::Literals::StringLiterals;
ThreadMessageDialog::ThreadMessageDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mThreadMessageWidget(new ThreadMessageWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Thread"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins({});

    mThreadMessageWidget->setObjectName(u"mThreadMessageWidget"_s);
    mainLayout->addWidget(mThreadMessageWidget);

    auto separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFixedHeight(1);

    mainLayout->addWidget(separator);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setContentsMargins(style()->pixelMetric(QStyle::PM_LayoutLeftMargin),
                               style()->pixelMetric(QStyle::PM_LayoutTopMargin),
                               style()->pixelMetric(QStyle::PM_LayoutRightMargin),
                               style()->pixelMetric(QStyle::PM_LayoutBottomMargin));
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ThreadMessageDialog::reject);
    readConfig();
    setAttribute(Qt::WA_DeleteOnClose);
}

ThreadMessageDialog::~ThreadMessageDialog() = default;

void ThreadMessageDialog::setThreadMessageInfo(const ThreadMessageWidget::ThreadMessageInfo &info)
{
    mThreadMessageWidget->setThreadMessageInfo(info);
}

void ThreadMessageDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myThreadMessageDialogGroupName), QSize(800, 300));
}

#include "moc_threadmessagedialog.cpp"
