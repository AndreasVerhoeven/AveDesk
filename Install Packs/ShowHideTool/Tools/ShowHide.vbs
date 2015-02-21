'------------------------------------------------------------------------------
'	Hide/Show Selected
'	Sample tool for AveDesk 1.2.
'	(c) Andreas Verhoeven 2005
'------------------------------------------------------------------------------

Dim aveDesk 'as AveDesk.Application
set aveDesk = CreateObject("AveDesk.Application")

if Not AveDesk is Nothing then
	Dim objArgs 'as WScript.Arguments
	Set objArgs = WScript.Arguments

	Dim i 'as Integer
	For i = 0 to objArgs.Count - 1
		Dim desklet 'as AveDesk.Desklet
		set desklet = aveDesk.RunningDesklets.Item ( objArgs.Item(i) )
		if Not desklet is Nothing then
			desklet.Visible = Not desklet.Visible
		end if
		set desklet = nothing
	Next

end if
