/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadfilewidget.h"
#include <KLocalizedString>
#include <KUrlRequester>

#include <KLineEdit>
#include <QFormLayout>

UploadFileWidget::UploadFileWidget(QWidget *parent)
    : QWidget(parent)
    , mDescription(new QLineEdit(this))
    , mSelectFile(new KUrlRequester(this))
{
    auto layout = new QFormLayout(this);
    layout->setObjectName(QStringLiteral("layout"));
    layout->setContentsMargins({});

    mDescription->setObjectName(QStringLiteral("mDescription"));
    mDescription->setClearButtonEnabled(true);
    layout->addRow(i18n("Description:"), mDescription);

    mSelectFile->setObjectName(QStringLiteral("mSelectFile"));
    layout->addRow(i18n("File:"), mSelectFile);

    connect(mSelectFile->lineEdit(), &KLineEdit::textChanged, this, [this](const QString &str) {
        Q_EMIT updateOkButton(!str.trimmed().isEmpty());
    });
}

UploadFileWidget::~UploadFileWidget() = default;

QString UploadFileWidget::description() const
{
    return mDescription->text();
}

QUrl UploadFileWidget::fileUrl() const
{
    return mSelectFile->url();
}

void UploadFileWidget::setFileUrl(const QUrl &url)
{
    mSelectFile->setUrl(url);
}

void UploadFileWidget::setAuthorizedMediaTypes(const QStringList &mediaTypes)
{
    mSelectFile->setMimeTypeFilters(mediaTypes);
}
