# .\ADScript_Unlock_Users.ps1 Sales

# входной параметр
param([string]$unit = $(throw "Enter organization unit"))

if ($unit)
{
    # делим строку на подстроки, разделенные '/' 
    $outmp = $unit -split '/'

    # строим путь в подраздел 
    $outmp | ForEach-Object {$path = "OU=$_," + $path}
    
    # в конце добавляем домен
    $path += "DC=formidabile, DC=com"
}

# -UsersOnly - только пользователи
# -LockedOut - заблокированные
# -SearchBase - поиск в заданном подразделении
# Search-ADAccount возвращает список учетных записей
$list = Search-ADAccount -SearchBase $path -UsersOnly -LockedOut

if ($list)
{
	foreach ($user in $list)
	{
	    # разблокировка учетной записи по имени
	    Unlock-ADAccount -Identity $($user.Name)

	    Write-Host $user.Name "unlocked"
	}
}
else
{
    Write-Host No find locked users
}
