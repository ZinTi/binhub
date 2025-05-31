Set objShell = CreateObject("WScript.Shell")

' 执行ipconfig命令并捕获输出结果
Set objExec = objShell.Exec("ipconfig")
strOutput = objExec.StdOut.ReadAll()

' 显示输出结果
WScript.Echo strOutput
