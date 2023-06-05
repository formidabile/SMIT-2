# получаем список всех пользователей в домене
$Users = Get-ADUser -Filter * -SearchBase "DC=formidabile, DC=com"

# проходимся по каждому пользователю из списка
foreach ($User in $Users) 
{
	# если пользователь относится к группе "GeneralAdmins"
	if (((Get-ADUser $User -Properties memberof).memberof -like "cn=GeneralAdmins*"))
	{
		# создаем массив байт
		[byte[]]$hours = @(255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255)
		# создаем объект типа хеш-таблица
		$replaceHashTable = New-Object HashTable
		# добавляем хеш-таблицу в свойство "logonHours"
		$replaceHashTable.Add("logonHours", $hours)
		# удаляем у данного пользователя текущие часы входы и заменяем их содержимым $replaceHashTable 
		Set-ADUser $User -Replace $replaceHashTable
		Write-Host "GeneralAdmin"
	}
	# если пользователь относится к группам "AccountManagers", "HelpDesk", "ResourceAdmins" 
	elseif (((Get-ADUser $User -Properties memberof).memberof -like "cn=AccountManagers*")-or((Get-ADUser $User -Properties memberof).memberof -like "cn=HelpDesk*")-or((Get-ADUser $User -Properties memberof).memberof -like "cn=ResourceAdmins*"))
	{
		[byte[]]$hours = @(224,255,7,224,255,7,224,255,7,224,255,7,224,255,7,224,255,7,224,255,7)
		$replaceHashTable = New-Object HashTable
		$replaceHashTable.Add("logonHours", $hours)
		Set-ADUser $User -Replace $replaceHashTable
		Write-Host "HelpDesk, AccountManager or ResourceAdmin"
	}
	# в остальных случаях
	else
	{
		[byte[]]$hours = @(0,0,0,192,127,0,192,127,0,192,127,0,192,127,0,192,127,0,0,0,0)
		$replaceHashTable = New-Object HashTable
		$replaceHashTable.Add("logonHours", $hours)
		Set-ADUser $User -Replace $replaceHashTable
		Write-Host "Other"
	}
}
