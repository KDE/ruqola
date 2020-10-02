""" Account Info Class """
import sys
class AccountInfo:
    def __init__(self):
        self.accountName = ''
        self.serverName = ''
        self.serverUrl = ''

    @property
    def accountName(self)->str:
        return self.accountName

    @accountName.setter
    def accountName(self, str: accountName):
        self.accountName = accountName
