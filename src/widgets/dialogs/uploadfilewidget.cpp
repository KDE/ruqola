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

#include "uploadfilewidget.h"
#include <KLocalizedString>
#include <KUrlRequester>

#include <QFormLayout>
#include <KLineEdit>

UploadFileWidget::UploadFileWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QFormLayout(this);
    layout->setObjectName(QStringLiteral("layout"));
    layout->setContentsMargins(0, 0, 0, 0);

    mDescription = new KLineEdit(this);
    mDescription->setTrapReturnKey(true);
    mDescription->setObjectName(QStringLiteral("mDescription"));
    mDescription->setClearButtonEnabled(true);
    layout->addRow(i18n("Description:"), mDescription);

    mSelectFile = new KUrlRequester(this);
    mSelectFile->setObjectName(QStringLiteral("mSelectFile"));
    layout->addRow(i18n("File:"), mSelectFile);

    connect(mSelectFile->lineEdit(), &KLineEdit::textChanged, this, [this](const QString &str) {
        Q_EMIT updateOkButton(!str.trimmed().isEmpty());
    });
}

UploadFileWidget::~UploadFileWidget()
{
}

QString UploadFileWidget::description() const
{
    return mDescription->text();
}

QUrl UploadFileWidget::fileUrl() const
{
    return mSelectFile->url();
}
