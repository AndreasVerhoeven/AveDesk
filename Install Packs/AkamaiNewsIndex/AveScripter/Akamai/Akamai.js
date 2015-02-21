///////////////////////////////////////////////////////////////////////////////
/// Code For the Akamai Net Usage Index for News Widget for AveDesk.
///
/// Ported by Andreas Verhoeven
///
/// Version:
///  + 1.0: initial
///
///////////////////////////////////////////////////////////////////////////////


/// Some global variables used throughout the script.

// data for the continents.
var polys = null;
var names = null;
var abbr = null;

// variables for the rotation animation
var startRotation = 0;
var endRotation = 0;
var rotationAnimTimer = null;
var rotationProgress = 0;
var MAX_ROTATION_ANIM_FRAMES = 15;
var currentRotation = 0;

// variables for keeping track of the mouse-hovering of the map.
var prevIndex = -1;
var clickedIndex = -2;

//-----------------------------------------------------------------------------

/// This function initializes the data and some layers for this widget.
function initDesklet()
{
	polys = new Array();
	polys[0] = new Array(54,70,65,48,53,23,1,24,0,48,35,74,47,76);
	polys[1] = new Array(48,74,44,85,47,110,56,111,75,83,53,69);
	polys[2] = new Array(100,104,119,98,118,75,106,63,94,58,84,61,77,73);
	polys[3] = new Array(167,88,176,86,193,28,144,0,106,27,104,43,111,51,104,57,105,63,118,74,149,88,154,86);
	polys[4] = new Array(107,27,105,4,92,6,74,35,84,62,94,58,105,62,105,57,111,51,104,44);
	polys[5] = new Array(148,95,151,106,182,114,186,101,165,86,155,86,145,91);

	names = new Array();
	names[0] ="North America";
	names[1] = "South America";
	names[2] = "Africa";
	names[3] = "Asia (Pacific)";
	names[4] = "Europe";
	names[5] = "Australia";
	names[6] = "The World";

	abbr = new Array();
	abbr[0] = "namerica";
	abbr[1] = "samerica";
	abbr[2] = "africa";
	abbr[3] = "asia";
	abbr[4] = "europe";
	abbr[5] = "australia";
	abbr[6] = "world";

	// setting clickable doesn't work in the XML file yet (scripter bug).
	this.layers("infobox").clickable = false;
	this.layers("infotext").clickable = false;
	this.layers("gloss").clickable = false;

	// we also set the selection to the saved location.
	ChangeSelection(this.Parameters("Location").value);
}

//-----------------------------------------------------------------------------

/// This function highlights the active layer.
function highlight(mouseIsOn)
{
	var layer = this.GetActiveLayer();

	layer.lockUpdates();
	if(mouseIsOn)
	{
		layer.FontColor = "clBlue";
		layer.Fontstyle = "Bold+Underlinded";
	}
	else
	{
		layer.FontColor = "clYellow";
		layer.FontStyle = "Bold";
	}
	layer.Redraw();
}

//-----------------------------------------------------------------------------


/// This function responds to the rotationAnimTimer messages.
/// It adjust the rotation of the dial, based on MAX_ROTATION_ANIM_FRAMES, using
/// a sinus() to make the meter move smoothly (non-linear).
function onRotationAnimTimer()
{
	var prevRotation = currentRotation;

	if(MAX_ROTATION_ANIM_FRAMES == rotationProgress)
	{
		currentRotation = endRotation;
		this.layers("dial").Rotation = currentRotation;
		rotationAnimTimer.StopTimer();
	}
	else
	{
		var rotation = endRotation - startRotation;
		var newRotation = Math.sin( rotationProgress / MAX_ROTATION_ANIM_FRAMES * Math.PI/2) * rotation;
		currentRotation = parseInt(startRotation + newRotation);
		this.layers("dial").Rotation = currentRotation;
		rotationProgress++;	
	}
}

/// This function starts to rotate the meter to a new value.
function rotateMeterToValue(value)
{
	if(null == rotationAnimTimer)
		rotationAnimTimer = this.Timers.New("rotationAnimTimer",20,"onRotationAnimTimer();",0);

	startRotation = currentRotation;
	endRotation = value;
	rotationProgress = 0;
	rotationAnimTimer.ResetTimer();
}

//-----------------------------------------------------------------------------

/// This function gets a string of text for a certain location by index.
function getStringForIndex(index)
{
	var percentage = this.XMLS(0).GetValue("[data/region[name='" + names[index] + "']/percentage]");
	var visitors = this.XMLS(0).GetValue("[data/region[name='" + names[index]  + "']/visitors]");
	var name = this.XMLS(0).GetValue("[data/region[name='" + names[index]  + "']/name]");
	if(parseInt(percentage) > 0)
		percentage = "+" + percentage;

	var str = name + ": " + percentage + "%\n" + visitors + " visitors/min";
	return str;
}

/// This function changes the selection of the location to a new index:
/// The map, infotext and the meter are rotated.
function ChangeSelection(index)
{
	if(index != -1)
	{
		this.Parameters("Location").value = index;
		this.layers("map").src = "images/map/sel_" + abbr[index] + ".png";
		this.layers("infotext").text = getStringForIndex(index);
		var percentage = this.XMLS(0).GetValue("[data/region[name='" + names[index] + "']/percentage]");
		rotateMeterToValue(parseInt(percentage));
	}
}


//-----------------------------------------------------------------------------

/// Event function for when the XML data has been updated: the visual info will be updated.
function onUpdateXML()
{
	ChangeSelection(parseInt(this.parameters("Location").value));
}

//-----------------------------------------------------------------------------

/// Event function for when the map is clicked. When the click occured on
/// a continent, we select the continent and go into "clicked" mode.
function onMapClick()
{
	var x = this.GetLayerMouseXPos();
	var y = this.GetLayerMouseYPos();
	var index = polyFromPoint(x,y);
	ChangeSelection(index);
	onMapExit();
	this.layers("globe").alpha = 0;
	clickedIndex = index;
}

/// Event function for when the mouse has moved on the map. We check if the mouse
/// status has changed (over a continents polygon or not) and update the images
/// accordingly.
function onMapMouseMove()
{

	var x = this.GetLayerMouseXPos();
	var y = this.GetLayerMouseYPos();
	var index = polyFromPoint(x,y);
	if(prevIndex != index && clickedIndex != index)
	{
		// make sure we go into 'selection mode' when we were in clicked mode.
		if(clickedIndex != -2)
		{
			onMapMouseEnter();
			clickedIndex = -2;
		}

		if(-1 == index)
			this.layers("infotext").text = "Click to select region";
		else
			this.layers("infotext").text = names[index] + "\nClick to select region";

		if(-1 == index)
		{
			this.layers("map").src = "images/map/hov_none.png";
			this.layers("map").cursor = "crDefault";
		}
		else
		{
			this.layers("map").src = "images/map/hov_" + abbr[index] + ".png";
			this.layers("map").cursor = "IDC_HAND";
		}
	}

	prevIndex = index;
}

/// Event function for when the mouse enters the map. We simply show the 'world' button.
function onMapMouseEnter()
{
	this.layers("globe").alpha = 255;
}

/// Helper function to decide if the mouse is over the globe, from the spectrum
/// of the map.
function isOnGlobe()
{
	var map   = this.layers("map");
	var globe = this.layers("globe");

	var x = this.GetLayerMouseXPos() + map.x;
	var y = this.GetLayerMouseYPos() + map.y;

	var treshold = 1;
	var onGlobe =	x >= (globe.x - treshold) && x < (globe.x + globe.width  + treshold) &&
					y >= (globe.y - treshold) && y < (globe.y + globe.height + treshold);
}

/// Event function for when the mouse has left the map. Go back into 'display mode' if
/// the mouse is not on the globe button.
function onMapMouseExit()
{
	if(!isOnGlobe())
		onMapExit();
}

/// Helper function to go into 'display mode'.
function onMapExit()
{
	var map   = this.layers("map");
	var globe = this.layers("globe");

	globe.alpha = 0;

	//this.LockAllUpdates();

	var index = this.Parameters("Location").value;
	map.src = "images/map/sel_" + abbr[index] + ".png";
	this.layers("infotext").text = getStringForIndex(index);

	//this.UnLockAllUpdates();
}
//-----------------------------------------------------------------------------

/// Event function for when the globe has been clicked. We select the world as 
/// the location.
function onGlobeClick()
{
	// 6 = world
	ChangeSelection(6);
	onMapExit();
}

/// Event function for when the mouse enters the globe. We make sure the globe is really
/// visible and change the map to reflect that the world will be selected when clicked.
function onGlobeEnter()
{
	this.layers("globe").alpha = 255;
	this.layers("map").src = "images/map/hov_none.png";
	this.layers("infotext").text = names[6] + "\nClick to select region";
}

/// Event function for when the globe is exited. Simply go into 'display mode'.
function onGlobeExit()
{
	onMapMouseExit();
}

//-----------------------------------------------------------------------------

/// Function that gets the index of the polygon from polys that includes the 
/// point (x,y)
function polyFromPoint(x,y)
{
	for(var index = 0; index < polys.length; ++index)
	{
		if(insidePolygon(polys[index], x, y))
		{
			return index;
		}
	}

	return -1;
}


/**
 * Checks if a point is inside a polygon.
 *@param polygon: an array to (p1,p2) points describing the polygon
 *@param x: the x-coord of the point to check.
 *@param y: the y-coord of the point to check.
 *@return: true iff (x,y) is inside polygon.
 */
function insidePolygon(polygon, x, y)
{
	var N = polygon.length;
	var i;
	var xinters;
	var p1x, p1y, p2x, p2y;
	var px = x; py = y;
	var oddNodes = false;

	p1x = polygon[0];
	p1y = polygon[1];

	for(i = 2; i <= N; i += 2)
	{
		p2x = polygon[i % N];
		p2y = polygon[(i % N)+1];
		if(py > Math.min(p1y, p2y) )
		{
			if(py <= Math.max(p1y, p2y) )
			{
				if(px <= Math.max(p1x, p2x) )
				{
					if(p1y != p2y)
					{
						xinters = (py - p1y) * (p2x - p1x) / (p2y - p1y) + p1x;
						if(p1x == p2x || px <= xinters)
							oddNodes = !oddNodes;	
					}
				}
			}
		}
		p1x = p2x;
		p1y = p2y;
	}

	return oddNodes;
}

//-----------------------------------------------------------------------------