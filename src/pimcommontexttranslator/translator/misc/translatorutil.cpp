/*
  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "translatorutil.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QComboBox>
using namespace PimCommonTextTranslator;

TranslatorUtil::TranslatorUtil() = default;

QVector<QPair<QString, QString>> TranslatorUtil::bingSpecificLanguages()
{
    TranslatorUtil translatorUtil;
    QVector<QPair<QString, QString>> langLanguage;
#if 0
    { QLatin1String("bs-BG"),  QLatin1String("bs-Latn")  },         //                                            NOTE: Bing translator only
    { QLatin1String("sr-RS"),  QLatin1String("sr-Cyrl")  },         //                                            NOTE: Bing translator only
    { QLatin1String("zh-CN"),  QLatin1String("zh-Hans")  },         // SimplifiedChinese                        ; NOTE: Bing translator only
    { QLatin1String("zh-TW"),  QLatin1String("zh-Hant")  }          // TraditionalChinese                       ; NOTE: Bing translator only
#endif
    return langLanguage;
}

QVector<QPair<QString, QString>> TranslatorUtil::googleSpecificLanguages()
{
    TranslatorUtil translatorUtil;
    QVector<QPair<QString, QString>> langLanguage;
    langLanguage.append(translatorUtil.pair(TranslatorUtil::zh_cn_google)); // For google only
    langLanguage.append(translatorUtil.pair(TranslatorUtil::zh_tw_google)); // For google only
    return langLanguage;
}

QVector<QPair<QString, QString>> TranslatorUtil::yandexSpecificLanguages()
{
    TranslatorUtil translatorUtil;
    QVector<QPair<QString, QString>> langLanguage;
    langLanguage.append(translatorUtil.pair(TranslatorUtil::jv_yandex));
    langLanguage.append(translatorUtil.pair(TranslatorUtil::zn_yandex));
    return langLanguage;
}

QVector<QPair<QString, QString>> TranslatorUtil::genericLanguages()
{
    TranslatorUtil translatorUtil;
    QVector<QPair<QString, QString>> fullListLanguage;
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::automatic));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::en));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::nl));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::fr));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::de));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::el));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::it));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ja));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ko));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::pt));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ru));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::es));

    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::af));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::sq));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ar));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::hy));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::az));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::eu));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::be));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::bg));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ca));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::hr));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::cs));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::da));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::et));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::tl));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::fi));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::gl));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ka));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ht));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::iw));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::hi));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::hu));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::is));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::id));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ga));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::lv));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::lt));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::mk));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ms));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::mt));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::no));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::fa));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::pl));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ro));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::sr));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::sk));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::sl));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::sw));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::sv));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::th));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::tr));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::uk));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::ur));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::vi));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::cy));
    fullListLanguage.append(translatorUtil.pair(TranslatorUtil::yi));
    return fullListLanguage;
}

QPair<QString, QString> TranslatorUtil::pair(TranslatorUtil::languages lang)
{
    QPair<QString, QString> ret;
    switch (lang) {
    case automatic:
        ret = QPair<QString, QString>(i18n("Detect language"), QStringLiteral("auto"));
        break;
    case en:
        ret = QPair<QString, QString>(i18n("English"), QStringLiteral("en"));
        break;
    case zh:
        ret = QPair<QString, QString>(i18n("Chinese (Simplified)"), QStringLiteral("zh"));
        break;
    case zt:
        ret = QPair<QString, QString>(i18n("Chinese (Traditional)"), QStringLiteral("zt"));
        break;
    case nl:
        ret = QPair<QString, QString>(i18n("Dutch"), QStringLiteral("nl"));
        break;
    case fr:
        ret = QPair<QString, QString>(i18n("French"), QStringLiteral("fr"));
        break;
    case de:
        ret = QPair<QString, QString>(i18n("German"), QStringLiteral("de"));
        break;
    case el:
        ret = QPair<QString, QString>(i18n("Greek"), QStringLiteral("el"));
        break;
    case it:
        ret = QPair<QString, QString>(i18n("Italian"), QStringLiteral("it"));
        break;
    case ja:
        ret = QPair<QString, QString>(i18n("Japanese"), QStringLiteral("ja"));
        break;
    case ko:
        ret = QPair<QString, QString>(i18n("Korean"), QStringLiteral("ko"));
        break;
    case pt:
        ret = QPair<QString, QString>(i18n("Portuguese"), QStringLiteral("pt"));
        break;
    case ru:
        ret = QPair<QString, QString>(i18n("Russian"), QStringLiteral("ru"));
        break;
    case es:
        ret = QPair<QString, QString>(i18n("Spanish"), QStringLiteral("es"));
        break;
    case af:
        ret = QPair<QString, QString>(i18n("Afrikaans"), QStringLiteral("af"));
        break;
    case sq:
        ret = QPair<QString, QString>(i18n("Albanian"), QStringLiteral("sq"));
        break;
    case ar:
        ret = QPair<QString, QString>(i18n("Arabic"), QStringLiteral("ar"));
        break;
    case hy:
        ret = QPair<QString, QString>(i18n("Armenian"), QStringLiteral("hy"));
        break;
    case az:
        ret = QPair<QString, QString>(i18n("Azerbaijani"), QStringLiteral("az"));
        break;
    case eu:
        ret = QPair<QString, QString>(i18n("Basque"), QStringLiteral("eu"));
        break;
    case be:
        ret = QPair<QString, QString>(i18n("Belarusian"), QStringLiteral("be"));
        break;
    case bg:
        ret = QPair<QString, QString>(i18n("Bulgarian"), QStringLiteral("bg"));
        break;
    case ca:
        ret = QPair<QString, QString>(i18n("Catalan"), QStringLiteral("ca"));
        break;
    case zh_cn_google: // For google only
        ret = QPair<QString, QString>(i18n("Chinese (Simplified)"), QStringLiteral("zh-CN")); // For google only
        break;
    case zh_tw_google: // For google only
        ret = QPair<QString, QString>(i18n("Chinese (Traditional)"), QStringLiteral("zh-TW")); // For google only
        break;
    case hr:
        ret = QPair<QString, QString>(i18n("Croatian"), QStringLiteral("hr"));
        break;
    case cs:
        ret = QPair<QString, QString>(i18n("Czech"), QStringLiteral("cs"));
        break;
    case da:
        ret = QPair<QString, QString>(i18n("Danish"), QStringLiteral("da"));
        break;
    case et:
        ret = QPair<QString, QString>(i18n("Estonian"), QStringLiteral("et"));
        break;
    case tl:
        ret = QPair<QString, QString>(i18n("Filipino"), QStringLiteral("tl"));
        break;
    case fi:
        ret = QPair<QString, QString>(i18n("Finnish"), QStringLiteral("fi"));
        break;
    case gl:
        ret = QPair<QString, QString>(i18n("Galician"), QStringLiteral("gl"));
        break;
    case ka:
        ret = QPair<QString, QString>(i18n("Georgian"), QStringLiteral("ka"));
        break;
    case ht:
        ret = QPair<QString, QString>(i18n("Haitian Creole"), QStringLiteral("ht"));
        break;
    case iw:
        ret = QPair<QString, QString>(i18n("Hebrew"), QStringLiteral("iw"));
        break;
    case hi:
        ret = QPair<QString, QString>(i18n("Hindi"), QStringLiteral("hi"));
        break;
    case hu:
        ret = QPair<QString, QString>(i18n("Hungarian"), QStringLiteral("hu"));
        break;
    case is:
        ret = QPair<QString, QString>(i18n("Icelandic"), QStringLiteral("is"));
        break;
    case id:
        ret = QPair<QString, QString>(i18n("Indonesian"), QStringLiteral("id"));
        break;
    case ga:
        ret = QPair<QString, QString>(i18n("Irish"), QStringLiteral("ga"));
        break;
    case lv:
        ret = QPair<QString, QString>(i18n("Latvian"), QStringLiteral("lv"));
        break;
    case lt:
        ret = QPair<QString, QString>(i18n("Lithuanian"), QStringLiteral("lt"));
        break;
    case mk:
        ret = QPair<QString, QString>(i18n("Macedonian"), QStringLiteral("mk"));
        break;
    case ms:
        ret = QPair<QString, QString>(i18n("Malay"), QStringLiteral("ms"));
        break;
    case mt:
        ret = QPair<QString, QString>(i18n("Maltese"), QStringLiteral("mt"));
        break;
    case no:
        ret = QPair<QString, QString>(i18n("Norwegian"), QStringLiteral("no"));
        break;
    case fa:
        ret = QPair<QString, QString>(i18n("Persian"), QStringLiteral("fa"));
        break;
    case pl:
        ret = QPair<QString, QString>(i18n("Polish"), QStringLiteral("pl"));
        break;
    case ro:
        ret = QPair<QString, QString>(i18n("Romanian"), QStringLiteral("ro"));
        break;
    case sr:
        ret = QPair<QString, QString>(i18n("Serbian"), QStringLiteral("sr"));
        break;
    case sk:
        ret = QPair<QString, QString>(i18n("Slovak"), QStringLiteral("sk"));
        break;
    case sl:
        ret = QPair<QString, QString>(i18n("Slovenian"), QStringLiteral("sl"));
        break;
    case sw:
        ret = QPair<QString, QString>(i18n("Swahili"), QStringLiteral("sw"));
        break;
    case sv:
        ret = QPair<QString, QString>(i18n("Swedish"), QStringLiteral("sv"));
        break;
    case th:
        ret = QPair<QString, QString>(i18n("Thai"), QStringLiteral("th"));
        break;
    case tr:
        ret = QPair<QString, QString>(i18n("Turkish"), QStringLiteral("tr"));
        break;
    case uk:
        ret = QPair<QString, QString>(i18n("Ukrainian"), QStringLiteral("uk"));
        break;
    case ur:
        ret = QPair<QString, QString>(i18n("Urdu"), QStringLiteral("ur"));
        break;
    case vi:
        ret = QPair<QString, QString>(i18n("Vietnamese"), QStringLiteral("vi"));
        break;
    case cy:
        ret = QPair<QString, QString>(i18n("Welsh"), QStringLiteral("cy"));
        break;
    case yi:
        ret = QPair<QString, QString>(i18n("Yiddish"), QStringLiteral("yi"));
        break;
    }
    return ret;
}

void TranslatorUtil::addItemToFromComboBox(QComboBox *combo, const QPair<QString, QString> &pair)
{
    combo->addItem(pair.first, pair.second);
}

QString TranslatorUtil::groupTranslateName()
{
    return QStringLiteral("Translate");
}

QString TranslatorUtil::engineTranslateName()
{
    return QStringLiteral("engine");
}

QString TranslatorUtil::defaultEngineName()
{
    return QStringLiteral("google");
}

QString TranslatorUtil::loadEngine()
{
    KConfigGroup myGeneralGroup(KSharedConfig::openConfig(), groupTranslateName());
    const QString engineTypeStr = myGeneralGroup.readEntry(engineTranslateName(), defaultEngineName()); // Default google
    return engineTypeStr;
}

void TranslatorUtil::saveEngineSettings(const QString &engineName)
{
    KConfigGroup myGroup(KSharedConfig::openConfig(), groupTranslateName());
    myGroup.writeEntry(engineTranslateName(), engineName);
    myGroup.sync();
}
