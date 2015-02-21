dim av
set av = createobject("avedesk.application")

dim d' as Desklet
dim s
for each d in av.runningdesklets
  s = s + typename(d.objectmodel) + vbcrlf
next

msgbox s
