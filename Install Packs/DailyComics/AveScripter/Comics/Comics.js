// A map<Key, Value> of all abbrevations with their full name of
// the comics that can be picked.
var names = new Array();
names["ch"] = "Calvin and Hobbes";
names["ga"] = "Garfield";
names["tmdic"] = "Dick Tracy";
names["ft"] = "Foxtrot";

// a map for the "yearfix", for example, the Calvin & Hobbes comic is eleven years
// old (probably cheaper ;-), so we define that the year needs to be 'fixed' by 11.
// to keep the option open for other comics as well, we don't make the C&H comic
// a special case, but just give the possibility to fix the year for every comic.
// map<Key, Value> Key=Abbrevation, Value=yearfix
var yearFix = new Array();
yearFix["ch"] = 11;
yearFix["ga"] = 0;
yearFix["tmdic"] = 0;
yearFix["ft"] = 0;


// variables for fading animation
var logoFadedOut = false;
var fadeTimer = null;
var isFadingOut = false;
var alphaDelta = 20;
var minAlpha = 50;
var maxAlpha = 255;

// variables for the URL of each comic
// sample url: http://images.ucomics.com/comics/ch/1994/ch941127.gif
var comicBaseUrl = "http://images.ucomics.com/comics/";
var comicExt = ".gif";

// variables keeping track of the scrolling status
var scroll = 0;
var scrollerShouldBeVisible = false;


// variable keeping track of the current mode and sliding

var miniheight = 36;
var miniStart = 20;

var SMALLMODE = 0;
var FULLMODE  = 1;
var currentMode = FULLMODE;
var animProgress = miniheight + miniStart;
var slideUpTimer = null;
var slideDownTimer = null;
var animInterval = 20;
var imgPath = "";


var topHeight = 62;
var midHeight = 174;
var comHeight = 190;
var botHeight = 31;

var flipSmallX = 308;
var flipSmallY = 48;

var flipFullX = 634;
var flipFullY = 252;

// initializes the dropdownlist from the abbrevation-fullname map/array.
function buildList()
{
	var str = "";
	for(x in names)
	{
		str += "" + names[x] + "=" + x + ";";
	}
	str = "" + str.substr(0,str.length-1);
	this.controls("comicList").listvalues = str;
}

//Update the position of the scroll
function UpdateScrollPosition()
{
	if(scrollerShouldBeVisible && FULLMODE == this.parameters("currentMode").value)
	{
		this.LockAllUpdates();

		var scroller = this.controls("scroller");
		var comic = this.layers("comic");
		var val = scroll;
		val -= this.GetMouseScrollValue()*10;
		val = Math.max(val, 0);
		val = Math.min(val, scroller.ScrollbarSize);
		scroller.value = val;
		scroll = val;
		var h = comHeight + scroll;
		comic.ClipSource = "0,"+ scroll +"," + comic.width + "," + h;

		this.Refresh();
	}
}

function adjustLayersForMode()
{
	var isFull = this.parameters("currentMode").value == FULLMODE;
	this.layers("top").visible = isFull;
	this.layers("mid").visible = isFull;
	this.layers("bot").visible = isFull;
	this.layers("comic").visible = isFull;
	this.controls("scroller").visible = isFull;
	this.layers("mini").visible = !isFull;

	if(isFull)
	{
		animProgress = this.sides(0).height - botHeight;
		this.layers("flipit").x = flipFullX;
		this.layers("flipit").y = flipFullY;
	}
	else
	{
		animProgress = miniheight + miniStart;
		this.layers("flipit").x = flipSmallX;
		this.layers("flipit").y = flipSmallY;
	}
}

function betweenLayer(layer, y)
{
	return y > layer.y && y < layer.y + layer.height;
}

function doSlideAdjustment(layer, y)
{
	var h = y - layer.y;
	if(h <= 0)
		layer.visible = false;
	else
	{
		layer.height = h;
		layer.clipSource = "0,0," + layer.width + "," + h;
	}
}

function doSlideAdjustmentCom(layer, y)
{
	var h = y - layer.y;
	if(h <= 0)
		layer.visible = false;
	else
	{
		layer.height = h;
		var h2 = h + scroll;
		layer.clipSource = "0," + scroll + "," + layer.width + "," + h2;
	}
}


function doSlideAdjustmentDown(layer, y)
{
	var h = y - layer.y;
	if(h > 0)
	{
		if(!layer.visible)
			layer.visible = true;
		layer.height = h;
		layer.clipSource = "0,0," + layer.width + "," + h;
	}
}

function doSlideAdjustmentDownCom(layer, y)
{
	var h = y - layer.y;
	if(h > 0)
	{
		if(!layer.visible)
			layer.visible = true;
		layer.height = h;
		var h2 = h + scroll;
		layer.clipSource = "0," + scroll + "," + layer.width + "," + h2;
	}
}

function isBetween(a, b, val)
{
	return val >= a && val <= b;
}

function toggleMode()
{
	if(this.parameters("currentMode").value == FULLMODE)
		toSmallMode();
	else
		toFullMode();
}

function onSlideDownTimer()
{
	this.LockAllUpdates();

	var delta = 8;
	var top = this.layers("top");
	var mid = this.layers("mid");
	var bot = this.layers("bot");
	var com = this.layers("comic");

	if(animProgress < this.sides(0).height - botHeight)
	{
		if(isBetween(top.y, top.y + topHeight, animProgress))
			doSlideAdjustmentDown(top, animProgress);
		else if(animProgress >= top.y + topHeight)
		{
			top.height = topHeight;
			top.clipSource = "0,0," + top.width + "," + top.height;
		}

		if(isBetween(com.y, com.y + comHeight, animProgress))
			doSlideAdjustmentDownCom(com, animProgress);
		else if(animProgress >= com.y + comHeight)
		{
			com.height = comHeight;
			var h = com.height + scroll;
			com.clipSource = "0," + scroll + "," + com.width + "," + h;
		}

		if(isBetween(mid.y, mid.y + midHeight, animProgress))
			doSlideAdjustmentDown(mid, animProgress);
		else if(animProgress >= mid.y + midHeight)
		{
			mid.height = midHeight;
			mid.clipSource = "0,0," + mid.width + "," + mid.height;
		}

		if(animProgress > top.y + topHeight && animProgress < top.y + topHeight + botHeight)
		{
			//doSlideAdjustmentDown(bot, animProgress);
			if(!bot.visible)
			{
				bot.y = top.y + topHeight;
				bot.visible = true;
			}
		}
		else 
			if(animProgress > top.y + topHeight && animProgress < mid.y + midHeight)
		{
			if(bot.height != botHeight)
				bot.height = botHeight;
			bot.y = animProgress;
		}

		animProgress += delta;
	}
	else
	{
		mid.height = midHeight;
		mid.clipSource = "0,0," + mid.width + "," + mid.height;
		bot.y = mid.y + midHeight;
		com.height = comHeight;
		var h = com.height + scroll;
		com.clipSource = "0," + scroll + "," + com.width + "," + h;
		this.controls("scroller").visible =  scrollerShouldBeVisible;
		this.layers("flipit").x = flipFullX;
		this.layers("flipit").y = flipFullY;

		if(slideDownTimer != null)
			slideDownTimer.StopTimer();
	}

	this.Refresh();
}

function onSlideUpTimer()
{
	var delta = 8;
	var top = this.layers("top");
	var mid = this.layers("mid");
	var bot = this.layers("bot");
	var com = this.layers("comic");

	this.LockAllUpdates();

	if(animProgress > top.y + miniheight)
	{
		if(betweenLayer(top, animProgress))
				doSlideAdjustment(top, animProgress);
		else if(animProgress < top.y)
			top.visible = false;

		if(betweenLayer(mid, animProgress))
			doSlideAdjustment(mid, animProgress);
		else if(animProgress < mid.y)
			mid.visible = false;

		if(betweenLayer(com, animProgress))
			doSlideAdjustmentCom(com, animProgress);
		else if(animProgress < com.y)
			com.visible = false;

		if(animProgress < top.y + miniheight + bot.height)
		{
			//doSlideAdjustment(bot, animProgress);
			if(bot.visible)
				bot.visible = false;
		}
		else if(animProgress < bot.y && animProgress > top.height + top.y)
			bot.y = animProgress;

		animProgress -= delta;
	}
	else
	{
		this.layers("mini").visible = true;
		top.visible = false;
		this.layers("flipit").x = flipSmallX;
		this.layers("flipit").y = flipSmallY;

		/*
			NO RESIZING IN THIS VERSION YET
		this.sides(0).height = 120;
		this.sides(0).width  = 360;
		this.controls("comicList").x = 140;
		this.controls("comicList").y = 36;
		this.controls("comicList").width = 160;
		this.controls("comicList").height = 25;
		*/

		if(slideUpTimer != null)
			slideUpTimer.StopTimer();
	}

	this.Refresh();
}

function toFullMode()
{
	if(FULLMODE == this.parameters("currentMode").value)
		return;

	this.parameters("currentMode").value = FULLMODE;

	var top = this.layers("top");
	var mid = this.layers("mid");
	var bot = this.layers("bot");
	var com = this.layers("comic");

//	this.lockAllUpdates();
//	mid.height = 1;
//	bot.height = 1;
//	com.height = 1;
//	mid.visible = true;
//	bot.visible = true;
//	com.visible = true;
	//this.layers("mini").visible = false;
//	this.Refresh();

	if(slideUpTimer != null)
		slideUpTimer.StopTimer();

	//animProgress = this.sides(0).height - bot.height;
	this.lockAllUpdates();
	if(animProgress == miniheight + miniStart)
		bot.y = top.y + miniheight;
	this.layers("mini").visible = false;
	onSlideDownTimer();
	this.Refresh();
	if(null == slideDownTimer)
		slideDownTimer = this.Timers.New("slideDownTimer",animInterval,"onSlideDownTimer();",0);

	slideDownTimer.ResetTimer();

}

function toSmallMode()
{
	if(SMALLMODE == this.parameters("currentMode").value)
		return;

	this.parameters("currentMode").value = SMALLMODE;

	var top = this.layers("top");
	var mid = this.layers("mid");
	var bot = this.layers("bot");
	var com = this.layers("comic");

	this.controls("scroller").visible = false;

	if(slideDownTimer != null)
		slideDownTimer.StopTimer();

	onSlideUpTimer();
	if(null == slideUpTimer)
		slideUpTimer = this.Timers.New("slideUpTimer",animInterval,"onSlideUpTimer();",0);

	slideUpTimer.ResetTimer();
}

// called when the comic is changed thru the dropdownlist
function onComicChange()
{
	var ctrl = this.controls("comicList");
	var newComic = ctrl.GetSelectValue(ctrl.newindex);
	this.parameters("comicType").value = newComic;

	// change the logo to the comics logo
	this.layers("logo").src = "images/logos/" + newComic + ".png";
	loadNewComic(new Date(), "comic");

	if(this.parameters("currentMode").value != FULLMODE)
		toFullMode();
}

// reacts to changes in the scrollbar.
function onScrollbar()
{
	var l = this.layers("comic");
	var scroller = this.controls("scroller");
	var val = parseInt(scroller.value);
	if(scroll != val)
	{
		var val = parseInt(scroller.value);
		scroll = val;
		var bot = val + comHeight;
		l.ClipSource = "0,"+ val +"," + l.width + "," + bot;
	}
}

// returns true iff a file exists at the url
function httpFileExists(url)
{
	var xmlHttp = new ActiveXObject('Microsoft.XMLHTTP');
	xmlHttp.open("GET",url,false);
	xmlHttp.send(null);
	return xmlHttp.status != 404;
}

// creates the url for a date for the current comictype.
function getUrl(date)
{
	var comicType = "" + this.parameters("comicType").value;
	var month = date.getMonth() + 1;
	var year = date.getYear() - yearFix[comicType];
	var year2 = year % 100;
	var day = date.getDate();
	if(day < 10)
		day  = "0" + day;
	if(year2 < 10)
		year2 = "0" + year2;
	if(month < 10)
		month = "0" + month;
	var url = comicBaseUrl + comicType + "/" + year + "/" + comicType + year2 + month + day + comicExt;
	return url;
}

function saveComicAsTempFile(url)
{
	var tmpPath = this.io.GetTmpPath();
	var fileName = this.io.GenerateUniqueName();
	imgPath = tmpPath + fileName + ".png";
	return this.io.URLToFile(url, imgPath);
}

// loads a new comic into a layer
function loadNewComic(date, layerName)
{
	var l = this.layers(layerName);
	var url = getUrl(date);

	// if there is no comic for date, try to get the comic
	// from the previous day.
	if(!httpFileExists(url))
	{
		date.setDate(date.getDate() - 1);
		url = getUrl(date);
	}
	
	this.lockAllUpdates();

	saveComicAsTempFile(url);

	// currently, we are not able to figure out
	// the width of the image we load from the web (AVESCRIPTER BUG, v 1.1),
	// so we load the webimage first in an invisible, dummy layer which get automatically
	// sized and then use the dummy layers dimensions to get the dimensions of the webimage
	//this.layers("comic2").src="!WEBIMG:" + url;
	this.layers("comic2").src = imgPath;
	var imgHeight = this.layers("comic2").height;

	// load the image in the real layer
	//l.src="!WEBIMG:" + url;
	l.src = imgPath;

	// and adjust the scrollbar and viewing area
	l.ClipSource = "0,0," + l.width + "," + comHeight;
	var scroller = this.controls("scroller");
	scrollerShouldBeVisible = imgHeight > comHeight; 
	scroller.visible =  scrollerShouldBeVisible && (FULLMODE == this.parameters("currentMode").value);
	scroller.value = 0;
	scroll = 0;
	scroller.ScrollbarSize = imgHeight - l.height;

	this.Refresh();
}

function makeSureFadeTimerExists()
{
	if(null == fadeTimer)
		fadeTimer = this.Timers.New("fadeTimer",10,"onFadeTimer();",0);

	fadeTimer.ResetTimer();
}

function onFadeTimer()
{
	if(isFadingOut)
	{
		if(this.layers("logo").alpha - alphaDelta >= minAlpha)
			this.layers("logo").alpha -= alphaDelta;
		else
		{
			this.layers("logo").alpha = minAlpha;
			fadeTimer.StopTimer();
		}
	}
	else
	{
		if(this.layers("logo").alpha + alphaDelta < maxAlpha)
			this.layers("logo").alpha += alphaDelta;
		else
		{
			this.layers("logo").alpha = maxAlpha;
			fadeTimer.StopTimer();
		}

	}
}

function onLogoMouseMove()
{
	var y = this.GetLayerMouseYPos();
	if(y >= this.layers("comic").Y && this.parameters("currentMode").value == FULLMODE)
	{
		if(!isFadingOut)
		{
			isFadingOut = true;
			makeSureFadeTimerExists();
			fadeTimer.ResetTimer();
		}
	}
	else 
	{
		if(isFadingOut)
		{
			isFadingOut = false;
			makeSureFadeTimerExists();
			fadeTimer.ResetTimer();
		}
	}
}

function onLogoMouseOut()
{
	isFadingOut = false;
	makeSureFadeTimerExists();
	fadeTimer.ResetTimer();
}