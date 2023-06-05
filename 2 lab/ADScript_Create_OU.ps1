Import-Module ActiveDirectory

$name_dc = "formidabile"
$name_dc_com = "com"

New-ADOrganizationalUnit -Name Others -Path "dc=$name_dc,dc=$name_dc_com"

New-ADOrganizationalUnit -Name Developers -Path "dc=$name_dc,dc=$name_dc_com"

New-ADOrganizationalUnit -Name Designers -Path "dc=$name_dc,dc=$name_dc_com"

New-ADOrganizationalUnit -Name Testers -Path "dc=$name_dc,dc=$name_dc_com"
