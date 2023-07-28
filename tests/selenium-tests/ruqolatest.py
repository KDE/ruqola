#!/usr/bin/env python3
# SPDX-License-Identifier: LGPL-2.0-or-later
# SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

import unittest
from appium import webdriver
from appium.webdriver.common.appiumby import AppiumBy
import selenium.common.exceptions
from selenium.webdriver.support.ui import WebDriverWait

class SmokeTests(unittest.TestCase):
    @classmethod
    def setUpClass(self):
        desired_caps = {}
        desired_caps["app"] = "org.kde.ruqola.desktop"
        self.driver = webdriver.Remote(command_executor='http://127.0.0.1:4723', desired_capabilities=desired_caps)
        self.driver.implicitly_wait = 10

    @classmethod
    def tearDownClass(self):
        self.driver.quit()

    def setUp(self):
            pass

    def test_addTrainTrip(self):
        self.driver.find_element(by=AppiumBy.ACCESSIBILITY_ID, value="QApplication.MainWindow#1.mainToolBar.QToolButton").click()

if __name__ == '__main__':
    unittest.main()

