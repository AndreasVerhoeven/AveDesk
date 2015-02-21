dim av
set av = createobject("avedesk.application")

'dim d' as Desklet
'for each d in av.runningdesklets
'  d.visible = false
'next


Set objArgs = WScript.Arguments
For I = 1 to objArgs.Count - 1
   av.runningdesklets.item(objargs.item(i)).visible=cbool(objargs.item(0))
Next
