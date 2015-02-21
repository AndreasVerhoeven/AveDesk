'-------------------------------------------------------------------------------
'
' Somatic Tunes Desklet for AveScripter and AveDesk 1.3 +
' coded by Andreas Verhoeven.
' The code in this file can be used freely, without the need for written
' permission.
'
' Version:
'	1.1 - update
'		+ added crossfading between 2 albumarts
'	1.0 - initial
'			+ smart iTunes detection (not accidently restarting)
'			+ sliding albumart
'			+ fast forward/ rewind
'			+ purple guy frame-sequence animation
'			+ the purple guy's eyes are opened when a song is played
'			+ albumart is retrieved from iTunes if available, or from
'				folder.jpg in the song's folder.
'			+ the frame and albumart layers position are hardcoded :-(
'
'
'-------------------------------------------------------------------------------

'animation and timing constants
Const maxFadeFrames = 15
Const maxAlbumFrame = 25
Const maxPgFrame = 5
Const maxFlippyFrame = 25

Const mouseDownDelay = 200
Const purpleGuyAnimInterval = 100
Const crossFadeAlbumArtInterval = 10
Const albumArtSlideTimerInterval = 10
Const CheckerTimerInterval = 2000
Const FlippyTimerInterval = 10

'position constants
Const AlbumArtYOffset = 10
Const AlbumArtFrameYoffset = 0
Const AlbumArtFrameHeight = 115.0

'Constant strings
LANHAM = "http://www.dlanham.com/"
MENUET = "http://www.menuetapp.com/"

'filename constants
Const DefaultAlbumArtFileName = "art.png"

'other constants
Const PI = 3.1415926535897932384626433832795

'variables for keeping track of the iTunes loading status
Dim tunesLoaded
Dim keepOff
Dim iTunes
tunesLoaded = false
keepOff = false
set iTunes = nothing

'variable keeping track of the current albumart layer being used
Dim albumArtNum
albumArtNum = 0

''' Initializes the desklet
sub InitDesklet
	Dim Timer
	Set Timer =  this.Timers.New("DetectITunes",CheckerTimerInterval,"CheckTunes()",0)
	Timer.ResetTimer()

	if this.parameters("albumartvisible").value = 0 then
		HideAlbumArt()
	end if

	CheckTunes()
	if IsConnectedToApp then
		ShowPurpleGuy()
		if this.parameters("albumartvisible").value = 1 then
			ShowAlbumArt()
		end if
		UpdatePlayerStatus()
	else
		HidePurpleGuy()
		if this.parameters("albumartvisible").value = 1 then
			HideAlbumArt()
		end if
	end if

	Call SetCoverArtComboValue
end sub

''' returns true iff the script is connected to iTunes
function IsConnectedToApp
	IsConnectedToApp = Not (iTunes is nothing)
end function

''' binds this script to iTunes: the script will receive events and can use
''' the global iTunes object.
sub bindTunes
On error resume Next
  if iTunes is Nothing then
    Set iTunes = this.createobject("iTunes.Application","itune_")
	if IsConnectedToApp then
		Call OnConnect()
	end if
  end if
end sub

''' unbinds the script from iTunes: the script will no longer receive events
''' and cannot use the global iTunes object any longer.
sub unbindTunes
On error resume Next
  if not(iTunes is Nothing) then
    this.DisconnectObject(iTunes)
    Set ITunes = Nothing
	
	' we want to unbind from iTunes,  so the auto-connecter must not try to connect to iTunes
	' again: we tell it to keepOff
	keepOff = true
	
	OnDisconnect()

  end if
end sub

''' this function checks if iTunes is running, and tries to bind the script to
''' iTunes if it should keep off.
sub CheckTunes
	'iTunes is loaded if we can find its window
	tunesLoaded = this.UI.FindWindow("iTunes", "") > 0
	if tunesLoaded And keepOff then
		keepOff = true 'if loaded but we need to keep off, we are dealing with an 
						'iTunes instance we just unbinded from: we don't connect thus and 
						'still need to keep off from iTunes.
	elseif (Not tunesLoad) And keepOff then
		keepOff = false	' if iTunes is not loaded and we had to keep off from running instances
						' we no iTunes was closed since the last unbind(): it's now safe
						' to reconnect on newly found iTunes instances so we don't need to keep
						' off anymore.
	elseif tunesLoaded And (Not keepOff) And (Not IsConnectedToApp) then
		Call bindTunes() ' if iTunes is loaded and we don't need to keep off and we aren't 
		Call Update()	 ' already binded, we can safely bind the script to iTunes.
	end if

end sub

''' called on successfull connect to iTunes
Sub OnConnect
	' when connected, we also make the purple guy show 
	' and display albumart in case it was hidden
	ShowPurpleGuy()
	ShowAlbumArt()
end sub

''' called on successfull discconnect from iTunes
sub OnDisconnect
	' we also hide the purply guy
	HidePurpleGuy()
	HideAlbumArt()
end sub

''' the following function crossfades two albumart layers:
''' albumart0 and albumart1. the previous layer should be passed as
''' variable previousNum.
Dim crossFadeTimer
set crossFadeTimer = nothing
Dim currentFadeFrame
currentFadeFrame = 0
sub nextCrossFadeAlbumArtFrame(previousNum)
	Dim b
	b = (previousNum + 1) mod 2
	Dim first
	prev = "albumart" & previousNum
	Dim sec
	cur = "albumart" & b

	currentFadeFrame = currentFadeFrame + 1

	Dim ratio 'as float
	Dim ratio2 'as float
	ratio  = sin(currentFadeFrame / maxFadeFrames * PI / 2.0)
	ratio2 = sin(currentFadeFrame / maxFadeFrames * PI / 2.0 + PI/2.0)
	this.layers(prev).alpha = ratio2 * 255
	this.layers(cur).alpha = ratio * 255

	if currentFadeFrame >= maxFadeFrames then
		currentFadeFrame = 0
		crossFadeTimer.StopTimer()
	end if
end sub

''' this function starts crossfading from one albumart layer to another.
''' the prevnum parameter should hold the value of the previous albumart layer number.
''' the fname parameter should hold the new album-art filename.
sub StartCrossFadingAlbumArt(prevNum, fname)
	this.layers("albumart" & albumArtNum).alpha = 0
	this.layers("albumart" & albumArtNum).src = fname
	Set crossFadeTimer =  this.Timers.New("crossFadeTimer",crossFadeAlbumArtInterval,"nextCrossFadeAlbumArtFrame(" & prevNum & ")",0)
	crossFadeTimer.ResetTimer()
end sub

'''this function stops crossfading albumart.
sub StopCrossFadingAlbumArt
	if Not crossFadeTimer is nothing then
		crossFadeTimer.StopTimer()
	end if
end sub

''' this function updates the data of the desklet. Currently, it checks
''' for new albumart and crossfades it.
sub Update
	Dim fname
	fname = DefaultAlbumArtFileName

	' extract embedded albumart if available in the current track
	if IsConnectedToApp then
		if Not (iTunes.CurrentTrack is nothing) then
			if Not (iTunes.CurrentTrack.ArtWork is nothing) then
				if iTunes.CurrentTrack.ArtWork.Count > 0 then
					Dim artwork
					set artwork = iTunes.CurrentTrack.Artwork(1)
					if Not (artwork is nothing) then
						fname = this.io.GetTmpPath() + this.io.GenerateUniqueName() + ".png"
						artwork.SaveArtWorkToFile(fname)
					end if
				end if
			end if
		end if
	end if

	' if there was no embedded albumart in the current track, try to use
	' folder.jpg in the track's  folder.
	if fname = DefaultAlbumArtFileName then
		if IsConnectedToApp then
			if Not (iTunes.CurrentTrack is nothing) then
				Dim fs
				Set fs = CreateObject("Scripting.FileSystemObject")
				Dim tryName
				tryName = fs.BuildPath(fs.GetParentFolderName(iTunes.CurrentTrack.Location), "folder.jpg")
				if fs.FileExists(tryName) then
					fname = tryName
				end if
			end if
		end if 
	end if

	' if the albumart should only be visible when there IS albumart, then
	' hide the albumart or show it depending on the status.
	Dim gonnaSlide
	gonnaSlide = false
	if this.parameters("showartonlywhenavailable").value = 1 and this.parameters("albumartvisible").value = 1 then
		if fname = DefaultAlbumArtFileName then
			HideAlbumArt()
			gonnaSlide = true
		else
			gonnaSlide = AlbumArtIsHidden()
			ShowAlbumArt()
		end if
	end if

	' crossfade the old and the new albumart, if there is new a albumart file.
	' Also, we don't crossfade if we hide or show the albumart thru sliding.
	if this.layers("albumart" & albumArtNum).src <> fname then
		Dim prevNum
		prevNum = albumArtNum
		albumArtNum = (albumArtNum + 1) mod 2
		if gonnaSlide then
			StopCrossFadingAlbumArt()
			this.layers("albumart" & prevNum).alpha = 0
			this.layers("albumart" & albumArtNum).alpha = 255
			this.layers("albumart" & albumArtNum).src = fname
		else 
			StartCrossFadingAlbumArt prevNum, fname
		end if
	end if
	

end sub

'Track Change
sub itune_OnPlayerPlayEvent(track)
	Call Update()
	UpdatePlayerStatus()
end sub

'About to close the ITunes
sub itune_OnAboutToPromptUserToQuitEvent
  unbindTunes()
end sub

'On Stop.
sub itune_OnPlayerStopEvent(track)
	UpdatePlayerStatus()
end sub

''' called when the player status is changed.
sub UpdatePlayerStatus()
	Dim fname
	fname = "bg2.png"
	Dim but
	but = "play"
	if IsConnectedToApp then
		if iTunes.PlayerState = 1 then
			fname = "bg1.png"
			but = "pause"
		end if
	end if

	this.layers("togglePlayPause").src = but & "norm.png"
	this.layers("togglePlayPause").dsrc = but & "press.png"

	this.layers("bg").src = fname
end sub


''' the following functions animate the purple guy: he crawls down if iTunes is loaded
''' and crawls up again if iTunes is closed.
Dim hidePurpleGuyTimer
set hidePurpleGuyTimer = nothing
Dim showPurpleGuyTimer
set showPurpleGuyTimer = nothing
Dim pgFrame
pgFrame = 0
sub HidePurpleGuy
	Set hidePurpleGuyTimer =  this.Timers.New("hidePurpleGuyTimer",purpleGuyAnimInterval,"NextPurpleGuyFrame(true)",0)
	hidePurpleGuyTimer.ResetTimer()
end sub

sub ShowPurpleGuy
	Set showPurpleGuyTimer =  this.Timers.New("showPurpleGuyTimer",purpleGuyAnimInterval,"NextPurpleGuyFrame(false)",0)
	showPurpleGuyTimer.ResetTimer()
end sub

sub NextPurpleGuyFrame(hiding)
	if hiding then
		pgFrame = pgFrame + 1
		if pgFrame >= maxPgFrame then
			pgFrame = maxPgFrame
			hidePurpleGuyTimer.StopTimer()
		end if
	else
		pgFrame = pgFrame - 1
		if pgFrame <= 0 then
			pgFrame = 0
			showPurpleGuyTimer.StopTimer()
		end if
	end if

	this.layers("next").visible = (6-pgFrame) > 5
	this.layers("prev").visible = (6-pgFrame) > 5 
	this.layers("togglePlayPause").visible = (6-pgFrame) > 5 
 
	' animation frames are bg[5,4,3,2].png. If we are at frame 1, we display
	' a placeholder (backg.png)
	' if we are at frame 6, the animation needs to be ended and we 
	' set back the real background thru the UpdatePlayerStatus() function
	' which sets the correct background on the player state.
	if (6-pgFrame) < 2 then
		this.layers("bg").src = "backg.png"
	elseif (6-pgFrame) > 5 then
		UpdatePlayerStatus()
	else
		this.layers("bg"). src = "load" & (6 - pgFrame) & ".png"
	end if

end sub

'''toggles albumart visibility.
sub ToggleAlbumArt
	if this.parameters("albumartvisible").value = 1 then
		this.parameters("albumartvisible").value = 0
		HideAlbumArt()
		this.layers("fakeclicky").visible = true
	else
		this.parameters("albumartvisible").value = 1
		ShowAlbumArt()
		this.layers("fakeclicky").visible = false
	end if
	Call SetCoverArtComboValue
end sub


''' toggles 'show albumart only when avaiable'
sub ToggleShowAlbumartWhenAvailable
	if this.parameters("showartonlywhenavailable").value = 1 then
		this.parameters("showartonlywhenavailable").value = 0
		if this.parameters("albumartvisible") = 1 then
			ShowAlbumArt()
		end if
	else
		this.parameters("showartonlywhenavailable").value = 1
		Update()
	end if
	Call SetCoverArtComboValue
end sub

''' the following functions show/hide albumart by doing
''' a slide animation.
Dim albumHideTimer
set albumHideTimer = nothing
Dim albumShowTimer
set albumShowTimer = nothing
Dim albumFrame
albumFrame = 0
sub ShowAlbumArt
	if albumShowTimer is nothing then
		Set albumShowTimer =  this.Timers.New("albumShowTimer",albumArtSlideTimerInterval,"NextAlbumFrame(false)",0)
	end if
	if this.parameters("albumartvisible").value = 1 then
		albumShowTimer.ResetTimer()
	end if
end sub

sub HideAlbumArt
	if albumHideTimer is nothing then
		Set albumHideTimer =  this.Timers.New("albumHideTimer",albumArtSlideTimerInterval,"NextAlbumFrame(true)",0)
	end if
	albumHideTimer.ResetTimer()
end sub

function AlbumArtIsHidden
	AlbumArtIsHidden = this.layers("albumart0").y <> AlbumArtYOffset
end function

sub NextAlbumFrame(goingUp)
	if goingUp then
		albumFrame = albumFrame + 1
		if albumFrame >= maxAlbumFrame then
			albumFrame = maxAlbumFrame
			albumHideTimer.StopTimer()
		end if
	else
		albumFrame = albumFrame - 1
		if albumFrame <= 0 then
			albumFrame = 0
			albumShowTimer.StopTimer()
		end if
	end if

	Dim ratio 'as float
	ratio = sin(albumFrame/maxAlbumFrame * PI / 2.0)
	this.layers("frame").y = AlbumArtFrameYoffset - ratio * AlbumArtFrameHeight
	this.layers("albumart0").y =  AlbumArtYOffset - ratio * AlbumArtFrameHeight
	this.layers("albumart1").y =  AlbumArtYOffset - ratio * AlbumArtFrameHeight

end sub

''' this function does a 'play/pause' button action.
sub TogglePlayPause()
  if Not IsConnectedToApp() then
	  bindTunes()
  end if
  if IsConnectedToApp() then
    if (iTunes.PlayerState=1) then
      iTunes.Pause()
    else
      iTunes.Play()
    end if
  end if
end sub


''' the following functions simulate the next and prev buttons as they work in iTunes:
''' holding the mouse down on a button for a certain interval causes a fastforward. 
''' A click actually causes a nextTrack [goes for previous also].
Dim prevMouseDownTimer
set prevMouseDownTimer = nothing
Dim prevTicker
prevTicker = 0
sub PrevDown()
	if IsConnectedToApp() then
		this.layers("prev").CaptureMouse()
		prevTicker = 0
		Set prevMouseDownTimer =  this.Timers.New("prevMouseDownTimer",mouseDownDelay,"OnPrevMouseDownTimer()",0)
		prevMouseDownTimer.ResetTimer()
	end if
end sub

sub OnPrevMouseDownTimer()
	prevTicker = prevTicker + 1
	if IsConnectedToApp() and prevTicker > 1 then
		iTunes.Rewind()
		prevMouseDownTimer.StopTimer()
	end if
end sub

sub PrevUp()
	if IsConnectedToApp() then
		this.layers("prev").UncaptureMouse()
		prevMouseDownTimer.StopTimer()
		if prevTicker < 1 then
			iTunes.BackTrack()
		else
			iTunes.Resume()
		end if
	end if
	this.layers("prev").src = "prevnorm.png"
end sub

Dim nextMouseDownTimer
set nextMouseDownTimer = nothing
Dim nextTicker
nextTicker = 0
sub NextDown()
	if IsConnectedToApp() then
		this.layers("next").CaptureMouse()
		nextTicker = 0
		Set nextMouseDownTimer =  this.Timers.New("nextMouseDownTimer",mouseDownDelay,"OnNextMouseDownTimer()",0)
		nextMouseDownTimer.ResetTimer()
	end if
end sub

sub OnNextMouseDownTimer()
	nextTicker = nextTicker + 1
	if IsConnectedToApp() and nextTicker > 1 then
		iTunes.FastForward()
		nextMouseDownTimer.StopTimer()
	end if
end sub

sub NextUp()
	if IsConnectedToApp() then
		this.layers("next").UncaptureMouse()
		nextMouseDownTimer.StopTimer()
		if nextTicker < 1 then
			iTunes.NextTrack()
		else
			iTunes.Resume()
		end if
	end if
	this.layers("next").src = "nextnorm.png"
end sub



''' Does a highlight of a link
sub HighLightLink(mouseIsOn)
	Dim layer
	set layer = this.GetActiveLayer

	'layer.lockUpdates()
	if mouseIsOn then
		layer.FontColor = "clRed"
		layer.Fontstyle = "Bold+Underlinded"
	else
		layer.FontColor = "#578EE1"
		layer.FontStyle = "Bold+Underlined"
	end if
	'layer.Redraw()
	this.invalidate(false)
end sub

''' sets the coverart combobox
sub SetCoverArtComboValue
	Dim ctrl
	set ctrl = this.controls("comboArt")
	if this.parameters("albumartvisible").value = 0 then 'never visible
		ctrl.value = 1
	else
		'always visible: except
		if this.parameters("showartonlywhenavailable").value = 1 then 'not visible if no coverart found
			ctrl.value = 2
		else
			ctrl.value = 0
		end if
	end if
		
end sub

''' reacts to changes on the backside combobox for coverart settings
sub OnCoverArtComboChange
	Dim ctrl, newSel
	set ctrl = this.controls("comboArt")
	newSel = ctrl.GetSelectValue(ctrl.newindex)

	if newSel = 0 then 'coverart always visible
		this.parameters("albumartvisible").value = 1
		this.parameters("showartonlywhenavailable").value = 0
		ShowAlbumArt()
	elseif newSel = 1 then 'coverart never visible
		this.parameters("albumartvisible").value = 0
		HideAlbumArt()
	elseif newSel = 2 then 'coverart only visible if available
		this.parameters("albumartvisible").value = 1
		this.parameters("showartonlywhenavailable").value = 1
		Update()
	end if

	ctrl.value = newSel
end sub


Dim flippyTimer
set flippyTimer = nothing
Dim currentFlippyFrame
currentFlippyFrame = 0

''' does a next flippy frame
sub NextFlippyFrame(hiding)
	Dim ratio, alpha

	if hiding then
		currentFlippyFrame = currentFlippyFrame - 1
		if currentFlippyFrame <= 0 then
			currentFlippyFrame = 0
			flippyTimer.StopTimer()
		end if
	else
		currentFlippyFrame = currentFlippyFrame + 1
		if currentFlippyFrame >= maxFlippyFrame then
			currentFlippyFrame = maxFlippyFrame
			flippyTimer.StopTimer()
		end if
	end if

	ratio = currentFlippyFrame / maxFlippyFrame
	alpha = sin(ratio * PI / 2.0) * 255.0
	this.layers("flipit").alpha = alpha

end sub

''' reacts to the desklet being entered
sub OnDeskletEnter
	Set flippyTimer =  this.Timers.New("flippyTimer",FlippyTimerInterval,"NextFlippyFrame(false)",0)
	flippyTimer.ResetTimer()
end sub

''' reacs to the the desklet being exited
sub OnDeskletExit
Set flippyTimer =  this.Timers.New("flippyTimer",FlippyTimerInterval,"NextFlippyFrame(true)",0)
	flippyTimer.ResetTimer()
end sub