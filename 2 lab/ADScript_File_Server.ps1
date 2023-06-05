Install-WindowsFeature FS-SyncShareService,Web-WHC

Add-WindowsFeature FS-SyncShareService

New-SyncShare Shared –path C:\Shares\Shared –User FORMIDABILE\AccountManagers -RequireEncryption $true –RequirePasswordAutoLock $true
