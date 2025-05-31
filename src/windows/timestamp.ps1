Get-ChildItem -Recurse -Force .\ * | ForEach-Object{$_.CreationTime = ("01 January 1998 08:00:00")}
Get-ChildItem -Recurse -Force .\ * | ForEach-Object{$_.LastWriteTime = ("01 January 1998 08:00:00")}
Get-ChildItem -Recurse -Force .\ * | ForEach-Object{$_.LastAccessTime = ("01 January 1998 08:00:00")}