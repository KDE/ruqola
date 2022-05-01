""" Account Info Class """

import sys
class AccountInfo:
    def __init__(self):
        self.accountName = ''
        self.userName = ''
        self.serverUrl = ''

    # redefine print info
    def __str__(self):
        return (f"account name={self.accountName}, userName={self.userName}, serverUrl={self.serverUrl}")

    """ accountname """
    @property
    def accountName(self)->str:
        return self.__accountName

    @accountName.setter
    def accountName(self, val):
        self.__accountName = val

    """ userName """
    @property
    def userName(self)->str:
        return self.__userName

    @userName.setter
    def userName(self, val):
        self.__userName = val

    """ serverUrl """
    @property
    def serverUrl(self)->str:
        return self.__serverUrl

    @serverUrl.setter
    def serverUrl(self, val):
        self.__serverUrl = val
