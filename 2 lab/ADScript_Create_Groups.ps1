Import-Module ActiveDirectory

$name_ou = "Others"
$name_dc = "formidabile"
$name_dc_com = "com"

New-ADGroup -Name "GeneralAdmins" -GroupCategory Security -GroupScope Global -Path "ou=$name_ou,dc=$name_dc,dc=$name_dc_com"

New-ADGroup -Name "AccountManagers" -GroupCategory Security -GroupScope Global -ManagedBy "GeneralAdmins" -Path "ou=$name_ou,dc=$name_dc,dc=$name_dc_com"

New-ADGroup -Name "HelpDesk" -GroupCategory Security -GroupScope Global -ManagedBy "GeneralAdmins" -Path "ou=$name_ou,dc=$name_dc,dc=$name_dc_com"

New-ADGroup -Name "ResourceAdmins" -GroupCategory Security -GroupScope Global -ManagedBy "GeneralAdmins" -Path "ou=$name_ou,dc=$name_dc,dc=$name_dc_com"
