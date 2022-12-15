/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportlibreofficeautocorrection.h"
#include "autocorrection/autocorrectionutils.h"
#include "pimcommonautocorrection_debug.h"
#include <KZip>
#include <QDir>
#include <QTemporaryFile>
#include <QXmlStreamWriter>

using namespace PimCommonAutoCorrection;

ExportLibreOfficeAutocorrection::ExportLibreOfficeAutocorrection() = default;

ExportLibreOfficeAutocorrection::~ExportLibreOfficeAutocorrection()
{
    delete mZip;
}

bool ExportLibreOfficeAutocorrection::exportData(const QString &language, const QString &fileName, QString &errorMessage, const QString &writablePath)
{
    const QString libreOfficeWritableLocalAutoCorrectionPath =
        writablePath.isEmpty() ? AutoCorrectionUtils::libreOfficeWritableLocalAutoCorrectionPath() : writablePath;
    QDir().mkpath(libreOfficeWritableLocalAutoCorrectionPath);
    QString fixLangExtension = language;
    fixLangExtension.replace(QLatin1Char('_'), QLatin1Char('-'));
    const QString fname = fileName.isEmpty() ? libreOfficeWritableLocalAutoCorrectionPath + QStringLiteral("acor_%1.dat").arg(fixLangExtension) : fileName;
    // qDebug() << " fname " << fname;
    mZip = new KZip(fname);
    const bool result = mZip->open(QIODevice::WriteOnly);
    if (!result) {
        qCWarning(PIMCOMMONAUTOCORRECTION_LOG) << "Impossible to open " << fileName;
        return false;
    }
    if (!exportDocumentList()) {
        return false;
    }
    if (!exportSentenceExceptList()) {
        return false;
    }
    if (!exportWordExceptList()) {
        return false;
    }
    if (!exportManifest()) {
        return false;
    }
    mZip->close();
    delete mZip;
    mZip = nullptr;
    return true;
}

bool ExportLibreOfficeAutocorrection::exportDocumentList()
{
    QTemporaryFile temporaryShareFile;
    temporaryShareFile.open();
    QXmlStreamWriter streamWriter(&temporaryShareFile);

    streamWriter.setAutoFormatting(true);
    streamWriter.setAutoFormattingIndent(2);
    streamWriter.writeStartDocument();

    streamWriter.writeStartElement(QStringLiteral("block-list:block-list"));
    streamWriter.writeAttribute(QStringLiteral("xmlns:block-list"), QStringLiteral("http://openoffice.org/2001/block-list"));
    QHashIterator<QString, QString> i(mAutocorrectEntries);
    while (i.hasNext()) {
        i.next();
        streamWriter.writeStartElement(QStringLiteral("block-list:block"));
        streamWriter.writeAttribute(QStringLiteral("block-list:abbreviated-name"), i.key());
        streamWriter.writeAttribute(QStringLiteral("block-list:name"), i.value());
        streamWriter.writeEndElement();
    }
    streamWriter.writeEndElement();
    streamWriter.writeEndDocument();
    temporaryShareFile.close();
    mZip->addLocalFile(temporaryShareFile.fileName(), QStringLiteral("DocumentList.xml"));
    return true;
}

bool ExportLibreOfficeAutocorrection::exportSentenceExceptList()
{
    QTemporaryFile temporaryShareFile;
    temporaryShareFile.open();

    QXmlStreamWriter streamWriter(&temporaryShareFile);

    streamWriter.setAutoFormatting(true);
    streamWriter.setAutoFormattingIndent(2);
    streamWriter.writeStartDocument();

    streamWriter.writeStartElement(QStringLiteral("block-list:block-list"));
    streamWriter.writeAttribute(QStringLiteral("xmlns:block-list"), QStringLiteral("http://openoffice.org/2001/block-list"));

    QSet<QString>::const_iterator upper = mUpperCaseExceptions.constBegin();
    while (upper != mUpperCaseExceptions.constEnd()) {
        streamWriter.writeStartElement(QStringLiteral("block-list:block"));
        streamWriter.writeAttribute(QStringLiteral("block-list:abbreviated-name"), *upper);
        streamWriter.writeEndElement();
        ++upper;
    }
    streamWriter.writeEndElement();
    streamWriter.writeEndDocument();
    temporaryShareFile.close();

    mZip->addLocalFile(temporaryShareFile.fileName(), QStringLiteral("SentenceExceptList.xml"));
    return true;
}

bool ExportLibreOfficeAutocorrection::exportWordExceptList()
{
    QTemporaryFile temporaryShareFile;
    temporaryShareFile.open();

    QXmlStreamWriter streamWriter(&temporaryShareFile);

    streamWriter.setAutoFormatting(true);
    streamWriter.setAutoFormattingIndent(2);
    streamWriter.writeStartDocument();

    streamWriter.writeStartElement(QStringLiteral("block-list:block-list"));
    streamWriter.writeAttribute(QStringLiteral("xmlns:block-list"), QStringLiteral("http://openoffice.org/2001/block-list"));

    QSet<QString>::const_iterator twoUpper = mTwoUpperLetterExceptions.constBegin();
    while (twoUpper != mTwoUpperLetterExceptions.constEnd()) {
        streamWriter.writeStartElement(QStringLiteral("block-list:block"));
        streamWriter.writeAttribute(QStringLiteral("block-list:abbreviated-name"), *twoUpper);
        streamWriter.writeEndElement();
        ++twoUpper;
    }
    streamWriter.writeEndElement();
    streamWriter.writeEndDocument();
    temporaryShareFile.close();

    mZip->addLocalFile(temporaryShareFile.fileName(), QStringLiteral("WordExceptList.xml"));
    return true;
}

bool ExportLibreOfficeAutocorrection::exportManifest()
{
    QTemporaryFile temporaryShareFile;
    temporaryShareFile.open();

    QXmlStreamWriter streamWriter(&temporaryShareFile);
    streamWriter.setAutoFormatting(true);
    streamWriter.setAutoFormattingIndent(2);
    streamWriter.writeStartDocument();

    streamWriter.writeStartElement(QStringLiteral("manifest:manifest"));

    streamWriter.writeStartElement(QStringLiteral("manifest:file-entry"));
    streamWriter.writeAttribute(QStringLiteral("manifest:full-path"), QStringLiteral("/"));
    streamWriter.writeAttribute(QStringLiteral("manifest:media-type"), QString());
    streamWriter.writeEndElement();

    streamWriter.writeStartElement(QStringLiteral("manifest:file-entry"));
    streamWriter.writeAttribute(QStringLiteral("manifest:full-path"), QStringLiteral("DocumentList.xml"));
    streamWriter.writeAttribute(QStringLiteral("manifest:media-type"), QString());
    streamWriter.writeEndElement();

    streamWriter.writeStartElement(QStringLiteral("manifest:file-entry"));
    streamWriter.writeAttribute(QStringLiteral("manifest:full-path"), QStringLiteral("SentenceExceptList.xml"));
    streamWriter.writeAttribute(QStringLiteral("manifest:media-type"), QString());
    streamWriter.writeEndElement();

    streamWriter.writeStartElement(QStringLiteral("manifest:file-entry"));
    streamWriter.writeAttribute(QStringLiteral("manifest:full-path"), QStringLiteral("WordExceptList.xml"));
    streamWriter.writeAttribute(QStringLiteral("manifest:media-type"), QStringLiteral("text/xml"));
    streamWriter.writeEndElement();

    streamWriter.writeEndElement();
    streamWriter.writeEndDocument();
    temporaryShareFile.close();

    // Add mimetype file
    mZip->setCompression(KZip::NoCompression);
    mZip->writeFile(QStringLiteral("mimetype"), "");
    mZip->setCompression(KZip::DeflateCompression);
    mZip->addLocalFile(temporaryShareFile.fileName(), QStringLiteral("META-INF/manifest.xml"));
    return true;
}
