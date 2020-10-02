#!/usr/bin/env python3
""" main file for creating ruqola python"""

import sys
from mainwindow import RuqolaMainWindow

from PySide2.QtWidgets import QApplication, QMainWindow, QPushButton

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = RuqolaMainWindow()
    window.show()
    sys.exit(app.exec_())
