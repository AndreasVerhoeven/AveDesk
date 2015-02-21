//-------------------------------------------------------------------------------
//
// Nu.NL Tunes Desklet for AveScripter and AveDesk 1.3 +
// coded by Andreas Verhoeven.
// The code in this file can be used freely, without the need for written
// permission.
//
// Version:
//	1.0 initial version.
//
//
//-------------------------------------------------------------------------------

maxSlideFrame = 35;
slideTimerInterval = 10;

var slideTimer = null;
var currentSlideFrame = maxSlideFrame;


var slidingDown = true;

slideLength = 254;
slideStart = 44;
flippyOffset = 28;
itemHeight = 20;

function adjustItem(name, newMid)
{
	var l = this.layers(name);

	var topY = this.layers("bg_top").height + newMid;
	var lBot = l.y + l.height;

	var newHeight = itemHeight;
	newHeight = Math.min(Math.max(topY - l.y,0), itemHeight);

	if(newHeight != l.height)
	{
		//l.ImageHeight = 20;
		l.height = newHeight;
		l.ClipSource = "0,0," + l.width + "," + newHeight;
	}
}

function setSlideFrame(frameNum, hiding)
{
	currentSlideFrame = frameNum;
	var ratio = frameNum / maxSlideFrame;

	if(hiding)
		var yPos = Math.sin((1-ratio) * Math.PI / 2.0 + Math.PI / 2.0) * slideLength;
	else
		var yPos = Math.sin(ratio * Math.PI / 2.0) * slideLength;


	yPos = parseInt(yPos);
	var newHeight = yPos + this.layers("bg_top").height + this.layers("bg_bot").height;
	this.layers("bg_mid").ClipSource = "0,0," + this.layers("bg_mid").width + "," + yPos;
	this.layers("bg_mid").height = yPos;
	this.layers("bg_bot").y = this.layers("bg_top").height + yPos;
	this.layers("flipit").y = newHeight - flippyOffset;

	for(var i=1; i<=9;++i)
		adjustItem("item" + i, yPos);
}

function nextSlideFrame(hiding)
{
	if(hiding)
	{
		currentSlideFrame--;
		if(currentSlideFrame <= 0)
		{
			currentSlideFrame = 0;
			slideTimer.StopTimer();
		}
	}
	else
	{
		currentSlideFrame++;
		if(currentSlideFrame >= maxSlideFrame)
		{
			currentSlideFrame = maxSlideFrame;
			slideTimer.StopTimer();
		}
	}

	setSlideFrame(currentSlideFrame, hiding);
}


function startSlideDown()
{
	slidingDown = true;
	this.parameters("SlideState").value = 1;
	slideTimer =  this.Timers.New("slideTimer",slideTimerInterval,"nextSlideFrame(false)",0);
	slideTimer.ResetTimer();
}

function startSlideUp()
{
	slidingDown = false;
	this.parameters("SlideState").value = 0;
	slideTimer =  this.Timers.New("slideTimer",slideTimerInterval,"nextSlideFrame(true)",0);
	slideTimer.ResetTimer();
}

function initSlide()
{
	if(this.parameters("SlideState").value==0)
	{
		startSlideUp();
	}
	else
	{
		slidingDown = true;
		setSlideFrame(maxSlideFrame, false);
	}
}

function doSlide()
{
	if(slidingDown)
		startSlideUp();
	else
		startSlideDown();
}

function onInitDesklet()
{
	initSlide();
}


maxFlippyFrame = 25;
flippyTimerInterval = 10;

var flippyTimer = null;
var currentFlippyFrame = maxFlippyFrame;

function nextFlippyFrame(hiding)
{
	if (hiding)
	{
		currentFlippyFrame--;
		if (currentFlippyFrame <= 0)
		{
			currentFlippyFrame = 0;
			flippyTimer.StopTimer();
		}
	}
	else
	{
		currentFlippyFrame++;
		if (currentFlippyFrame >= maxFlippyFrame)
		{
			currentFlippyFrame = maxFlippyFrame;
			flippyTimer.StopTimer();
		}
	}

	var ratio = currentFlippyFrame / maxFlippyFrame;
	var alpha = Math.sin(ratio * Math.PI / 2.0) * 255.0;
	this.layers("flipit").alpha = alpha;
}


function onDeskletEnter()
{
	flippyTimer =  this.Timers.New("flippyTimer",flippyTimerInterval,"nextFlippyFrame(false)",0);
	flippyTimer.ResetTimer();
}

function onDeskletExit()
{
	flippyTimer =  this.Timers.New("flippyTimer",flippyTimerInterval,"nextFlippyFrame(true)",0);
	flippyTimer.ResetTimer();
}



maxFadeLinksFrames = 5;
fadeLinksTimerInterval = 10;

var fadeLinksTimer = null;
var currentFadeLinksFrame = 0;

var waitingForData = false;

function nextFadeLinksFrame(hiding)
{
	if (hiding)
	{
		currentFadeLinksFrame--;
		if (currentFadeLinksFrame <= 0)
		{
			currentFadeLinksFrame = 0;
			fadeLinksTimer.StopTimer();
			if(!waitingForData)
				startFadingLinksIn();

		}
	}
	else
	{
		currentFadeLinksFrame++;
		if (currentFadeLinksFrame >= maxFadeLinksFrames)
		{
			currentFadeLinksFrame = maxFadeLinksFrames;
			fadeLinksTimer.StopTimer();
		}
	}

	var ratio = currentFadeLinksFrame / maxFadeLinksFrames;
	var alpha = Math.sin(ratio * Math.PI / 2.0) * 255.0;

	// alpha all layers
	for(var c = 1; c <= 9; ++c)
		this.layers("item" + c).alpha = alpha;
}

function startFadingLinksOut()
{
	fadeLinksTimer =  this.Timers.New("fadeLinksTimer",fadeLinksTimerInterval,"nextFadeLinksFrame(true)",0);
	fadeLinksTimer.ResetTimer();
}

function startFadingLinksIn()
{
	fadeLinksTimer =  this.Timers.New("fadeLinksTimer",fadeLinksTimerInterval,"nextFadeLinksFrame(false)",0);
	fadeLinksTimer.ResetTimer();
}

function onGetXMLData()
{
	waitingForData = false;
	this.xmls(0).OnGetData = "";
	if(0 == currentFadeLinksFrame)
		startFadingLinksIn();
}

function onSourceListChange()
{
	waitingForData = true;
	currentFadeLinksFrame = maxFadeLinksFrames;
	//startFadingLinksOut();

	this.xmls(0).OnGetData = "onGetXMLData();";
	var ctrl = this.controls("sourceList");
	var newVal = ctrl.GetSelectValue(ctrl.newindex);
	this.parameters("sourceType").value = newVal;

	 this.xmls(0).Reset();
	 this.xmls(0).ForceGetData();

	 if(!slidingDown)
		 startSlideDown();
}


function onRefreshIntervalChange()
{
	var ctrl = this.controls("refreshInterval");
	var newVal = ctrl.GetSelectValue(ctrl.newindex);
	this.parameters("refreshDelay").value = newVal;

	 this.xmls(0).Reset();
	 this.Redraw();
}

function ClickNews(i)
{
	GoToLink();
	HideShowCase();
}

function HideShowCase()
{
	try
	{
		var ad = new ActiveXObject('AveDesk.Application');
		if(ad != null)
		{
			if(ad.ShowCase && ad.ShowCase.Active)
			{
				// AVEDESK BUG WORKAROUND: on desklet out doesn't come thru:
				onDeskletExit();

				ad.ShowCase.DeActivate();
			}
			ad = null;
		}
	}
	catch(err)
	{
		// bababoooo!
	}
}

//Go to the link
function GoToLink()
{
  var layer = this.GetActiveLayer();
  var link = this.ConvertToValue(layer.data);
  if (link != '')
    this.IO.ShellExecutes(link);
}

//Highlight text
function Highlight(TurnOn)
{
  var layer = this.GetActiveLayer();
  if (layer)
  {
    if (TurnOn==1)
	{
      layer.FontColor = 'clBlue';
	  layer.FontStyle = "U";
	}
    else
	  {
      layer.FontColor = 'clBlack';
	  layer.FontStyle = "";
	  }
  }
}
