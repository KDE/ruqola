/*
   SPDX-FileCopyrightText: 2022-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "uploadwidget.h"
#include "room/uploadfileprogressstatuslistwidget.h"
#include <QPushButton>
#include <QVBoxLayout>
int UploadWidget::identifier = 0;
UploadWidget::UploadWidget(QWidget *parent)
    : QWidget{parent}
    , mUploadFileProgressStatusListWidget(new UploadFileProgressStatusListWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    auto addElement = new QPushButton(QStringLiteral("Add"), this);
    mainLayout->addWidget(mUploadFileProgressStatusListWidget);
    mainLayout->addWidget(addElement);
    connect(addElement, &QPushButton::clicked, this, &UploadWidget::addButton);
}

void UploadWidget::addButton()
{
    mUploadFileProgressStatusListWidget->addProgressStatusWidget(++identifier);
    // TODO
}

#include "moc_uploadwidget.cpp"
