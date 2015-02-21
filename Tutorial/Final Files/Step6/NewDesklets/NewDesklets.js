///
/// NewDesklets.js, script for the NewDesklets AveScripter desklet for AveDesk.
/// © copyright Andreas Verhoeven, 2005.
/// May be freely used as part of the 'Creating a desklet with AveScripter'-
/// tutorial.
///

function onClickDone()
{
	var oldUpdateInterval = this.parameters("UpdateInterval").value;
	var oldRssType = this.parameters("RssType").value;
	var newUpdateInterval = this.controls("editUpdateInterval").value;
	var newRssType = this.controls("droplistType").value;

	if(oldUpdateInterval != newUpdateInterval || oldRssType != newRssType)
	{
		this.parameters("UpdateInterval").value = parseInt(newUpdateInterval);
		this.parameters("RssType").value = newRssType;
		this.xmls(0).Reset();
	}
	
	this.Redraw(true);
	this.FlipTo(0);
}

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
		layer.FontColor = "clWhite";
		layer.FontStyle = "Bold";
	}
	layer.Redraw();
}

function onClickItem(number)
{
	var link = this.xmls(0).GetValue("XMLTXT:0[rss//channel//item[" + number + "]//link]");
	this.IO.ShellExecutes(link);
}