# 提示用户输入文件或目录的路径，可以是相对路径或当前目录下的文件或目录
$path = Read-Host "请输入要修改时间戳的文件或目录的路径"

# 提示用户输入时间
$newTimeStr = Read-Host "请输入新的时间戳（格式为 yyyy-MM-dd HH:mm:ss）"

# 检查文件或目录路径是否有效
if (-not (Test-Path $path)) {
    Write-Host "输入的文件或目录路径无效，请检查输入。"
    return
}

# 检查时间戳格式是否有效
try {
    # 将输入的时间字符串转换为 DateTime 对象
    $newTime = [DateTime]::ParseExact($newTimeStr, "yyyy-MM-dd HH:mm:ss", $null)
} catch {
    Write-Host "输入的时间戳格式无效，请使用 yyyy-MM-dd HH:mm:ss 格式。"
    return
}

try {
    # 获取文件或目录对象
    $item = Get-Item $path

    # 修改文件或目录的创建时间
    $item.CreationTime = $newTime

    # 修改文件或目录的修改时间
    $item.LastWriteTime = $newTime

    # 修改文件或目录的访问时间
    $item.LastAccessTime = $newTime

    Write-Host "文件或目录时间戳修改成功！"
} catch [System.UnauthorizedAccessException] {
    Write-Host "文件或目录时间戳修改失败，权限不足。"
} catch [System.IO.FileNotFoundException] {
    Write-Host "文件或目录时间戳修改失败，文件或目录不存在。"
} catch {
    Write-Host "文件或目录时间戳修改失败，发生未知错误。错误信息：$_"
}