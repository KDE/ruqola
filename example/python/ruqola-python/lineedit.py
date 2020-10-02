from PySide2 import QtWidgets, QtGui

class LineEdit(QtWidgets.QLineEdit):
    def __init__(self):
        super().__init__()
        self.setPlaceholderText(self.tr("Add text here..."))
        