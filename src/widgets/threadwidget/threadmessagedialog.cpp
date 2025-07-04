/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "threadmessagedialog.h"
using namespace Qt::Literals::StringLiterals;

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QFrame>
#include <QStyle>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
static const char myThreadMessageDialogGroupName[] = "ThreadMessageDialog";
}

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

ThreadMessageDialog::~ThreadMessageDialog()
{
    writeConfig();
}

void ThreadMessageDialog::setThreadMessageInfo(const ThreadMessageWidget::ThreadMessageInfo &info)
{
    mThreadMessageWidget->setThreadMessageInfo(info);
}

void ThreadMessageDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myThreadMessageDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ThreadMessageDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myThreadMessageDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_threadmessagedialog.cpp"
