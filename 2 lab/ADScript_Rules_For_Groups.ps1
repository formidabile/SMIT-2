dsacls.exe "dc=formidabile,dc=com" /I:T /G "formidabile\AccountManagers:CCDC;user"
dsacls.exe "dc=formidabile,dc=com" /I:S /G "formidabile\AccountManagers:RCWDWO;;user"
dsacls.exe "dc=formidabile,dc=com" /I:S /G "formidabile\AccountManagers:CALO;;user"
dsacls.exe "dc=formidabile,dc=com" /I:S /G "formidabile\AccountManagers:RPWP;userAccountControl;user"
dsacls.exe "dc=formidabile,dc=com" /I:S /G "formidabile\AccountManagers:RPWP;pwdLastSet;user"

dsacls.exe "dc=formidabile,dc=com" /I:S /G "formidabile\HelpDesk:RPWP;lockoutTime;user"
