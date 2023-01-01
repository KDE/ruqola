/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showvideodialog.h"
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
static const char myShowVideoDialogGroupName[] = "ShowVideoDialog";
}

ShowVideoDialog::ShowVideoDialog(QWidget *parent)
    : QDialog(parent)
    , mShowVideoWidget(new ShowVideoWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Video"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mShowVideoWidget->setObjectName(QStringLiteral("mShowVideoWidget"));
    mainLayout->addWidget(mShowVideoWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ShowVideoDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

ShowVideoDialog::~ShowVideoDialog()
{
    writeConfig();
}

void ShowVideoDialog::setVideoUrl(const QUrl &videoPath)
{
    mShowVideoWidget->setVideoUrl(videoPath);
    setWindowTitle(i18nc("@title:window", "Video: %1", videoPath.fileName()));
}

void ShowVideoDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowVideoDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ShowVideoDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myShowVideoDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}
