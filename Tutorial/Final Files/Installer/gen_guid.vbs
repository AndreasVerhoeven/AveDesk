Dim TypeLib 'As Scriptlet.TypeLib
Dim tg 'as GUID
Dim createGUID 'as String

Set TypeLib = CreateObject("Scriptlet.TypeLib")
tg = TypeLib.Guid
createGuid = left(tg, len(tg)-3)
createGuid = right(createGuid,len(createGuid)-3)
Set TypeLib = Nothing
Msgbox createGuid