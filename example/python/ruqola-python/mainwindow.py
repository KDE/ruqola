""" MainWindows """
from PySide2 import QtWidgets, QtGui
from PySide2 import QtCore
from lineedit import LineEdit
from addaccount import AddAccountDialog

class RuqolaMainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle(self.tr("Ruqola"))
        self.initializeMenu()
        self.inializeWidget()
        
    def initializeMenu(self):
        menu = self.menuBar().addMenu(self.tr('File'))
        addAccountAction = menu.addAction(self.tr('Add Account'))
        addAccountAction.triggered.connect(self.open_add_account_dialog)

        menu.addSeparator()

        quitAction = menu.addAction(self.tr('Quit'))
        quitAction.triggered.connect(qApp.closeAllWindows)
        
    def inializeWidget(self):
        mainWindow = QtWidgets.QWidget()
        mainLayout = QtWidgets.QVBoxLayout(mainWindow)  
        self.textEdit = QtWidgets.QTextEdit()
        mainLayout.addWidget(self.textEdit)
        
        hLayout = QtWidgets.QHBoxLayout()
        hLayout.setContentsMargins(QtCore.QMargins(left=0, right=0, top=0, bottom=0))
        mainLayout.addLayout(hLayout)
        
        self.lineEdit = LineEdit()
        hLayout.addWidget(self.lineEdit)
        pushButton = QtWidgets.QPushButton(self.tr("Send message"))
        hLayout.addWidget(pushButton)
        self.setCentralWidget(mainWindow)  
        pushButton.clicked.connect(self.send_message_clicked)
        
    def open_add_account_dialog(self):
        addaccount = AddAccountDialog()
        if addaccount.exec():
            print(f"account name {addaccount.accountInfo()}")

    def send_message_clicked(self):
        self.textEdit.setText(self.lineEdit.text())
        
