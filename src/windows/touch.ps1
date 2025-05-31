Param(
	[Parameter(Mandatory=$true)]
	[string]$Path
)

if (Test-Path -LiteralPath $Path) {
	(Get-Item -Path $Path).LastWriteTime = Get-Date
} else {
	New-Item -Type File -Path $Path
}