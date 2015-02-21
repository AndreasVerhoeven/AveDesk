 Set TypeLib = CreateObject("Scriptlet.TypeLib")
 tg = TypeLib.Guid
 createGuid = mid(tg,2, len(tg)-4)
 Set TypeLib = Nothing
 Msgbox createGuid