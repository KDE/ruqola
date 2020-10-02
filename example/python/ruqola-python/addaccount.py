""" Add Account """
from PySide2 import QtWidgets, QtGui
from PySide2.QtWidgets import QDialog, QDialogButtonBox, QFormLayout, QVBoxLayout, QLineEdit
from accountinfo import AccountInfo

class AddAccountDialog(QDialog):
    def __init__(self):
        super().__init__()
        self.setWindowTitle(self.tr("Add Account"))
        self.initializeWidget()

    def initializeWidget(self):
        mainLayout = QVBoxLayout(self)
        self.formLayout = QFormLayout()
        self.buttonBox = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)

        # Account Name
        self.accountName = QLineEdit();
        self.formLayout.addRow(self.tr("Account Name:"), self.accountName);

        #Server Url
        self.serverUrl = QLineEdit();
        self.formLayout.addRow(self.tr("Server Url:"), self.serverUrl);

        mainLayout.addLayout(self.formLayout)
        mainLayout.addWidget(self.buttonBox)
        self.buttonBox.accepted.connect(self.accept)
        self.buttonBox.rejected.connect(self.reject)


    def accountInfo(self)->AccountInfo:
        info = AccountInfo()
        info.accountName(self.accountName.text())
        return info

    def get_accountName(self)->str:
        return self.accountName.text()

    def get_serverUrl(self)->str:
        return self.serverUrl.text()
