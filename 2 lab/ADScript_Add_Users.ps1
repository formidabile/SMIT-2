Import-Module ActiveDirectory

New-ADUser -Name "WednesdayAddams" -GivenName "Wednesday" -Surname "Addams" -SamAccountName "WednesdayAddams" -Path "OU=Others,DC=formidabile,DC=com" -AccountPassword(ConvertTo-SecureString "WednesdayGirl" -AsPlainText -Force) -Enabled $true -ScriptPath "script.bat"

New-ADUser -Name "KaarijaPoehonen" -GivenName "Kaarija" -Surname "Poehonen" -SamAccountName "KaarijaPoehonen" -Path "OU=Testers,DC=formidabile,DC=com" -AccountPassword(ConvertTo-SecureString "Kaarija_376" -AsPlainText -Force) -Enabled $true -ScriptPath "script.bat"

New-ADUser -Name "AlexeyGubanov" -GivenName "Alexey" -Surname "Gubanov" -SamAccountName "AlexeyGubanov" -Path "OU=Designers,DC=formidabile,DC=com" -AccountPassword(ConvertTo-SecureString "_Hesus+Avgn" -AsPlainText -Force) -Enabled $true -ScriptPath "script.bat"

New-ADUser -Name "SamSmith" -GivenName "Sam" -Surname "Smith" -SamAccountName "SamSmith" -Path "OU=Developers,DC=formidabile,DC=com" -AccountPassword(ConvertTo-SecureString "SamSmithPass-1" -AsPlainText -Force) -Enabled $true -ScriptPath "script.bat"
