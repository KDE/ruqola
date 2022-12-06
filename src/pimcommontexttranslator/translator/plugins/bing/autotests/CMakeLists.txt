ecm_add_test(bingengineclienttest.cpp bingengineclienttest.h ../bingengineclient.cpp ../bingengineplugin.cpp
    TEST_NAME bingengineclienttest
    NAME_PREFIX pimcommon-
    LINK_LIBRARIES Qt::Test Qt::Gui KF5::PimCommonTextTranslator KF5::I18n
)
