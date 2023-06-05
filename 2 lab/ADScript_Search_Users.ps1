# входной параметр
param([int]$number = 5)


$users_expire_date = &Get-ADUser -filter {Enabled -eq $True -and PasswordNeverExpires -eq $False} `
–Properties "SamAccountName", "msDS-UserPasswordExpiryTimeComputed", "PasswordLastSet" | `
Select-Object –Property “SamAccountName",@{Name="ExpiryDate";`
Expression={[datetime]::FromFileTime($_."msDS-UserPasswordExpiryTimeComputed")}}`


# проходимся по каждому пользователю из списка
foreach($item in $users_expire_date)
{
    # получаем дату истечения действия пароля в US
    $expire_date = $item.ExpiryDate 

    # получаем текущую дату в формате US
    $current_date = &Get-Date

    # получаем имя пользователя для входа
    $DisplayName = $item.SamAccountName

    # -gt (Great than) - если больше
    if($current_date -gt $item.ExpiryDate)
    {
       Write-Host "The password of user" $DisplayName "has expired!"
       Exit
    }

    # получаем время до истечения действия пароля
    $lasts = $expire_date - $current_date

    # -lt (Less than) - если меньше
    if($lasts.Days -lt $number)
    {
        Write-Host "The password of user" $DisplayName "expires less then" $Number "day(s)!"
        Write-Host "The password of user" $DisplayName "expires: " $expire_date
        Write-Host " "
    }
}
