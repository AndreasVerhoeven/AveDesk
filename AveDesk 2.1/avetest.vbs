dim av
set av = createobject("avedesk.application")

dim d' as Desklet
dim s
s = "There are " + cstr(av.runningdesklets.count) + " desklets:" + vbcrlf
for each d in av.runningdesklets
  s = s + GetDesc(d) + vbcrlf
  d.Style = styleTop
  d.Style = styleNormal
next

dim a

s = s + vbcrlf + "And " + cstr(av.availabledesklets.count) + " are available:" + vbcrlf
for each a in av.availabledesklets
  s = s + a.name + vbtab + "by " + a.author + vbcrlf  
next
msgbox s


dim ra, rn
randomize

dim iRet
do
	rn = Int(av.availabledesklets.count * Rnd + 1)
	set ra = av.availabledesklets.item(CLng(rn))
	iRet = msgbox("next: create random " + CStr(rn) + " " + ra.name + "?", 4)
	if iRet = 6 then
	  dim nd
	  Set nd = av.runningdesklets.add(ra, rnd*(1024-128), rnd*(768-128), 128, 128)
	  nd.visible = true
	  nd.Style = styleTop
	  nd.Style = styleNormal
	  'must set this to nothing because av.close would report leak
	  Set nd = nothing
	end if
loop while iRet = 6

'must set this to nothing because av.close would report leak
set ra = nothing

if msgbox("next: rotate all?", 4) = 6 then
  dim i
  for i = 0 to 360 step 3
    rotateall i
  next
  rotateall 0
end if

if msgbox("next: move desklet 1?", 4) = 6 then
  set d = av.runningdesklets.item(1)
  d.Style = styleTop
  d.redraw
  dim ox, oy, x, y
  ox = d.left
  oy = d.top
  for i = 0 to (768-d.width)
    d.move i, i
	av.Sleep 10
  next
  d.left = ox
  d.top = oy
  d.Style = styleNormal
  set d = nothing
end if

if msgbox("next: size desklet 1?", 4) = 6 then
  set d = av.runningdesklets.item(1)
  d.Style = styleTop
  d.redraw
  ox = d.width
  oy = d.height
  for i = 0 to 192
    d.width = i
	d.height = i
	av.Sleep 10
  next
  d.width = ox
  d.height = oy
  d.Style = styleNormal
  set d = nothing
end if


if msgbox("next: controlpanel?", 4) = 6 then
  av.showcontrolpanel
end if

if msgbox("next: Dumpskins?", 4) = 6 then
  dim ifo, h
  for each ifo in av.availabledesklets
    if ifo.skins.count then
      h = h + vbcrlf + ifo.name + ":" + vbcrlf
      dim sk
      for each sk in ifo.skins
        h = h + sk.Name + vbcrlf
      next
      h = h + vbcrlf
    end if
  next
  msgbox h
end if

if msgbox("next: close?", 4) = 6 then
  av.close
end if

'utility
function GetDesc(d)
  dim s
  s = d.about.name + vbtab + "by " + d.about.author + vbtab + "("
  s = s + cstr(d.left) + ","
  s = s + cstr(d.top) + ","
  s = s + cstr(d.width) + ","
  s = s + cstr(d.height) + ")"

  if d.Layers.Count > 1 then
	for each l in d.layers
	  if left(l.name, 8) <> "Default." then
	    s = s + vbtab + "[" + l.name + "]" + vbcrlf
	  end if
	next
  end if

  GetDesc = s
end function

sub rotateall(degree)
  dim d
  for each d in av.runningdesklets
    d.rotation = degree
	d.Redraw
	av.Sleep 15
  next
end sub