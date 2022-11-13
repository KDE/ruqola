/*
  SPDX-FileCopyrightText: 2012-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "importkmailautocorrection.h"

#include <QFile>
#include <QXmlStreamReader>
using namespace PimCommonAutoCorrection;

ImportKMailAutocorrection::ImportKMailAutocorrection() = default;

ImportKMailAutocorrection::~ImportKMailAutocorrection() = default;

bool ImportKMailAutocorrection::import(const QString &fileName, QString &errorMessage, LoadAttribute loadAttribute)
{
    Q_UNUSED(errorMessage);
    QFile xmlFile(fileName);
    if (!xmlFile.open(QIODevice::ReadOnly)) {
        return false;
    }
    QXmlStreamReader xml(&xmlFile);
    mMaxFindStringLength = 0;
    mMinFindStringLength = 0;
    if (xml.readNextStartElement()) {
        while (xml.readNextStartElement()) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            const QStringRef xmlName = xml.name();
#else
            const QStringView xmlName = xml.name();
#endif
            if (xmlName == QLatin1String("UpperCaseExceptions")) {
                if (loadAttribute == All) {
                    while (xml.readNextStartElement()) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        const QStringRef tagname = xml.name();
#else
                        const QStringView tagname = xml.name();
#endif
                        if (tagname == QLatin1String("word")) {
                            if (xml.attributes().hasAttribute(QStringLiteral("exception"))) {
                                const QString exception = xml.attributes().value(QStringLiteral("exception")).toString();
                                mUpperCaseExceptions += exception;
                                xml.skipCurrentElement();
                            }
                        }
                    }
                } else {
                    xml.skipCurrentElement();
                }
            } else if (xmlName == QLatin1String("TwoUpperLetterExceptions")) {
                if (loadAttribute == All) {
                    while (xml.readNextStartElement()) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        const QStringRef tagname = xml.name();
#else
                        const QStringView tagname = xml.name();
#endif
                        if (tagname == QLatin1String("word")) {
                            if (xml.attributes().hasAttribute(QStringLiteral("exception"))) {
                                const QString exception = xml.attributes().value(QStringLiteral("exception")).toString();
                                mTwoUpperLetterExceptions += exception;
                                xml.skipCurrentElement();
                            }
                        } else {
                            xml.skipCurrentElement();
                        }
                    }
                } else {
                    xml.skipCurrentElement();
                }
            } else if (xmlName == QLatin1String("DoubleQuote")) {
                if (loadAttribute == All) {
                    if (xml.readNextStartElement()) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        const QStringRef tagname = xml.name();
#else
                        const QStringView tagname = xml.name();
#endif
                        if (tagname == QLatin1String("doublequote")) {
                            mTypographicDoubleQuotes.begin = xml.attributes().value(QStringLiteral("begin")).toString().at(0);
                            mTypographicDoubleQuotes.end = xml.attributes().value(QStringLiteral("end")).toString().at(0);
                            xml.skipCurrentElement();
                        } else {
                            xml.skipCurrentElement();
                        }
                        xml.skipCurrentElement();
                    }
                } else {
                    xml.skipCurrentElement();
                }
            } else if (xmlName == QLatin1String("SimpleQuote")) {
                if (loadAttribute == All) {
                    if (xml.readNextStartElement()) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        const QStringRef tagname = xml.name();
#else
                        const QStringView tagname = xml.name();
#endif
                        if (tagname == QLatin1String("simplequote")) {
                            const QString simpleQuoteBegin = xml.attributes().value(QStringLiteral("begin")).toString();
                            if (!simpleQuoteBegin.isEmpty()) { // crash when we have old data with bug.
                                mTypographicSingleQuotes.begin = simpleQuoteBegin.at(0);
                            }
                            const QString simpleQuoteEnd = xml.attributes().value(QStringLiteral("end")).toString();
                            if (!simpleQuoteEnd.isEmpty()) { // crash when we have old data with bug.
                                mTypographicSingleQuotes.end = simpleQuoteEnd.at(0);
                            }
                            xml.skipCurrentElement();
                        } else {
                            xml.skipCurrentElement();
                        }
                        xml.skipCurrentElement();
                    }
                } else {
                    xml.skipCurrentElement();
                }
            } else if (xmlName == QLatin1String("SuperScript")) {
                if (loadAttribute == All || loadAttribute == SuperScript) {
                    while (xml.readNextStartElement()) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        const QStringRef tagname = xml.name();
#else
                        const QStringView tagname = xml.name();
#endif
                        if (tagname == QLatin1String("item")) {
                            const QString find = xml.attributes().value(QStringLiteral("find")).toString();
                            const QString super = xml.attributes().value(QStringLiteral("super")).toString();
                            mSuperScriptEntries.insert(find, super);
                            xml.skipCurrentElement();
                        } else {
                            xml.skipCurrentElement();
                        }
                    }
                } else {
                    xml.skipCurrentElement();
                }
            } else if (xmlName == QLatin1String("items")) {
                if (loadAttribute == All) {
                    while (xml.readNextStartElement()) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        const QStringRef tagname = xml.name();
#else
                        const QStringView tagname = xml.name();
#endif
                        if (tagname == QLatin1String("item")) {
                            const QString find = xml.attributes().value(QStringLiteral("find")).toString();
                            const QString replace = xml.attributes().value(QStringLiteral("replace")).toString();
                            const int findLenght(find.length());
                            mMaxFindStringLength = qMax(findLenght, mMaxFindStringLength);
                            mMinFindStringLength = qMin(findLenght, mMinFindStringLength);
                            mAutocorrectEntries.insert(find, replace);
                            xml.skipCurrentElement();
                        } else {
                            xml.skipCurrentElement();
                        }
                    }
                } else {
                    xml.skipCurrentElement();
                }
            } else {
                // TODO verify
                xml.skipCurrentElement();
            }
        }
    }
    return true;
}
