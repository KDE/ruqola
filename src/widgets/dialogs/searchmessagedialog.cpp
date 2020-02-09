/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "searchmessagedialog.h"
#include "searchmessagewidget.h"

#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
#include <QVBoxLayout>
#include <QDialogButtonBox>

namespace {
static const char myConfigGroupName[] = "SearchMessageDialog";
}
SearchMessageDialog::SearchMessageDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Search Messages"));
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mSearchMessageWidget = new SearchMessageWidget(this);
    mSearchMessageWidget->setObjectName(QStringLiteral("mSearchMessageWidget"));
    mainLayout->addWidget(mSearchMessageWidget);

    QDialogButtonBox *button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &SearchMessageDialog::reject);
    readConfig();
}

SearchMessageDialog::~SearchMessageDialog()
{
    writeConfig();
}

void SearchMessageDialog::setRoomId(const QString &roomId)
{
    mSearchMessageWidget->setRoomId(roomId);
}

void SearchMessageDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void SearchMessageDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    group.writeEntry("Size", size());
}
