Set objShell = CreateObject("WScript.Shell")

' ִ��ipconfig�������������
Set objExec = objShell.Exec("ipconfig")
strOutput = objExec.StdOut.ReadAll()

' ��ʾ������
WScript.Echo strOutput
