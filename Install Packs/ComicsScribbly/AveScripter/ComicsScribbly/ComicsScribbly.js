///
/// ComicsScribbly.js, Script file for the Comics (Scribbly) Desklet for AveScripter.
///	(c) copyright Andreas Verhoeven, 2005
/// Feel free to use.
///


// array holding the name of the months in dutch (acts like a map datastructure)
var months = new Array();
months[0] = "januari";
months[1] = "februari";
months[2] = "maart";
months[3] = "april";
months[4] = "mei";
months[5] = "juni";
months[6] = "juli";
months[7] = "augustus";
months[8] = "september";
months[9] = "oktober";
months[10]= "november";
months[11]= "december";

var lastDate = null;

function visitSite()
{
	this.io.shellexecutes("http://www.scribbly.nl");
}

function comicExists(date)
{
	var xmlHttp = new ActiveXObject('Microsoft.XMLHTTP');
	xmlHttp.open("GET","http://scribbly.nl/beheer/upload/" + getComicFilename(date),false);
	xmlHttp.send(null);
	return xmlHttp.status == 200;
}

function getComicFilename(date)
{
	var filename = "" + date.getDate() + "-" +  months[date.getMonth()] + "-krant.gif";
	return filename;
}

function doPrev()
{
	if(null == lastDate)
		lastDate = new Date();

	lastDate.setDate(lastDate.getDate() - 1);
	changeToComic(lastDate);
}

function doNext()
{
	var today = new Date();

	if(null == lastDate)
		lastDate = today;

	var date = lastDate;
	while(date <= today)
	{
		date.setDate(date.getDate() + 1);
		if(comicExists(date))
		{
			lastDate = date;
			this.layers("comic").src = "!WEBIMG:http://scribbly.nl/beheer/upload/" + getComicFilename(date);
			return;
		}
	}
	
	changeToComic(today);
}

function changeToComic(date)
{
	var comicLayer = this.layers("comic");
	if(comicLayer != null)
	{
		var weekDay = date.getDay();
		if(0 == weekDay) // on sundays no new comics
			date.setDate(date.getDate() - 2);
		else if(6 == weekDay) // on saturdays no new comics neither
			date.setDate(date.getDate() - 1);

		var fileName =  getComicFilename(date);
		if(!comicExists(date))
		{
			date.setDate(date.getDate() - 1);
			fileName =  getComicFilename(date);

			if(!comicExists(date))
			{
				date.setDate(date.getDate() - 1);
				fileName =  getComicFilename(date);
			}

			if(!comicExists(date))
			{
				comicLayer.src="!WEBIMG:http://scribbly.nl/beheer/upload/strookje.gif";
				return;
			}
		}

		lastDate = date;

		comicLayer.src="!WEBIMG:http://scribbly.nl/beheer/upload/" + getComicFilename(date);
	}
}