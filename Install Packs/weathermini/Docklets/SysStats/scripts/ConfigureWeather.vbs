Const change_zip = 0
Const toggle_units = 1

' Handle message from SysStats
Function ScriptController_HandleMessage(wparam, lparam, x, y)
'	url = "http://xoap.weather.com/weather/local/01450?cc=*&unit=e&dayf=10"
	url = SysStatsModel.Meters("Weather").URL
	zip = Mid(url, InStr(url, "local/") + 6, InStr(url, "?cc") - InStr(url, "local/") - 6)
	unit = Mid(url, InStr(url, "unit=") + 5, 1)

	Select Case lparam
		Case change_zip	
			newzip = InputBox("Enter ZIP Code", "SysStats Weather", zip)
			
			If newzip <> "" Then
				zip = newzip
			End If

		Case toggle_units
			If unit = "m" Then
				unit = "s"
			Else
				unit = "m"
			End If
	End Select
	
	' Set new URL
	SysStatsModel.Meters("Weather").URL = "http://xoap.weather.com/weather/local/" & zip & "?cc=*&unit=" & unit & "&dayf=10"
	
	'Force SysStats to reload XML document
	SysStatsModel.UpdateMeters()
End Function