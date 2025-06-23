/*
  SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "purposemenuwidget.h"
#include <KLocalizedString>
#include <Purpose/AlternativesModel>
#include <Purpose/Menu>
#include <QJsonArray>
#include <QTemporaryFile>
#include <QUrl>
using namespace Qt::Literals::StringLiterals;
PurposeMenuWidget::PurposeMenuWidget(QObject *parent)
    : QObject(parent)
    , mShareMenu(new Purpose::Menu)
{
    mShareMenu->setTitle(i18n("Share Text…"));
    mShareMenu->setIcon(QIcon::fromTheme(u"document-share"_s));
    mShareMenu->setObjectName(u"purposesharemenu"_s);
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
    mShareMenu->model()->setInputData(
        QJsonObject{{u"urls"_s, QJsonArray{{QUrl::fromLocalFile(mTemporaryShareFile->fileName()).toString()}}}, {u"mimeType"_s, {u"text/plain"_s}}});
    mShareMenu->model()->setPluginType(u"Export"_s);
    mShareMenu->reload();
}

void PurposeMenuWidget::slotShareActionFinished(const QJsonObject &output, int error, const QString &message)
{
    if (error) {
        Q_EMIT errorMessage(i18n("There was a problem sharing the document: %1", message));
    } else {
        const QString url = output["url"_L1].toString();
        if (url.isEmpty()) {
            Q_EMIT successMessage(i18n("File was shared."));
        } else {
            Q_EMIT successMessage(i18n("<qt>You can find the new request at:<br /><a href='%1'>%1</a> </qt>", url));
        }
    }
}

#include "moc_purposemenuwidget.cpp"
