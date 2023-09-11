/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "purposemenuwidget.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <Purpose/AlternativesModel>
#include <Purpose/Menu>
#include <QJsonArray>
#include <QTemporaryFile>
#include <QUrl>

PurposeMenuWidget::PurposeMenuWidget(QObject *parent)
    : QObject(parent)
    , mShareMenu(new Purpose::Menu)
{
    mShareMenu->setTitle(i18n("Share Text..."));
    mShareMenu->setIcon(QIcon::fromTheme(QStringLiteral("document-share")));
    mShareMenu->setObjectName(QStringLiteral("purposesharemenu"));
    connect(mShareMenu, &Purpose::Menu::aboutToShow, this, &PurposeMenuWidget::slotInitializeShareMenu);
    connect(mShareMenu, &Purpose::Menu::finished, this, &PurposeMenuWidget::slotShareActionFinished);
}

PurposeMenuWidget::~PurposeMenuWidget()
{
    delete mTemporaryShareFile;
    delete mShareMenu;
}

QByteArray PurposeMenuWidget::text()
{
    return mSelectedText.toUtf8();
}

QMenu *PurposeMenuWidget::menu() const
{
    return mShareMenu;
}

void PurposeMenuWidget::setSelectedText(const QString &str)
{
    mSelectedText = str;
}

void PurposeMenuWidget::slotInitializeShareMenu()
{
    delete mTemporaryShareFile;
    mTemporaryShareFile = new QTemporaryFile();
    mTemporaryShareFile->open();
    mTemporaryShareFile->setPermissions(QFile::ReadUser);
    mTemporaryShareFile->write(text());
    mTemporaryShareFile->close();
    mShareMenu->model()->setInputData(QJsonObject{{QStringLiteral("urls"), QJsonArray{{QUrl::fromLocalFile(mTemporaryShareFile->fileName()).toString()}}},
                                                  {QStringLiteral("mimeType"), {QStringLiteral("text/plain")}}});
    mShareMenu->model()->setPluginType(QStringLiteral("Export"));
    mShareMenu->reload();
}

void PurposeMenuWidget::slotShareActionFinished(const QJsonObject &output, int error, const QString &message)
{
    if (error) {
        Q_EMIT errorMessage(i18n("There was a problem sharing the document: %1", message));
    } else {
        const QString url = output[QLatin1String("url")].toString();
        if (url.isEmpty()) {
            Q_EMIT successMessage(i18n("File was shared."));
        } else {
            Q_EMIT successMessage(i18n("<qt>You can find the new request at:<br /><a href='%1'>%1</a> </qt>"));
        }
    }
}

#include "moc_purposemenuwidget.cpp"
