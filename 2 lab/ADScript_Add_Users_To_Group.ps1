Import-Module ActiveDirectory

Add-ADGroupMember "GeneralAdmins" WednesdayAddams
Add-ADGroupMember "AccountManagers" KaarijaPoehonen
Add-ADGroupMember "HelpDesk" AlexeyGubanov
Add-ADGroupMember "ResourceAdmins" SamSmith

Add-ADGroupMember Администраторы "GeneralAdmins"
Add-ADGroupMember IIS_IUSRS "ResourceAdmins"