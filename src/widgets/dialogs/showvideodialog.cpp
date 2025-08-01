/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showvideodialog.h"
using namespace Qt::Literals::StringLiterals;

#include "showvideowidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
const char myShowVideoDialogGroupName[] = "ShowVideoDialog";
}

ShowVideoDialog::ShowVideoDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mShowVideoWidget(new ShowVideoWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Video"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mShowVideoWidget->setObjectName(u"mShowVideoWidget"_s);
    mainLayout->addWidget(mShowVideoWidget);
    connect(mShowVideoWidget, &ShowVideoWidget::updateTitle, this, &ShowVideoDialog::slotUpdateTitle);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(u"button"_s);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ShowVideoDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

ShowVideoDialog::~ShowVideoDialog()
{
    writeConfig();
}

void ShowVideoDialog::slotUpdateTitle(const QUrl &url)
{
    setWindowTitle(i18nc("@title:window", "Video: %1", url.fileName()));
}

void ShowVideoDialog::setVideoPath(const QString &videoPath)
{
    mShowVideoWidget->setVideoPath(videoPath);
}

void ShowVideoDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myShowVideoDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ShowVideoDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myShowVideoDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_showvideodialog.cpp"
