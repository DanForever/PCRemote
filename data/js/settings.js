function uploadUpdate()
{
	document.getElementById("upload-update").submit();
	document.getElementById("content").hidden = true;
	document.getElementById("loading-display").hidden = false;
	
	setTimeout(reloadPage, 20 * 1000);
}

function reloadPage()
{
	window.location.reload()
}

function queryVersion(path, elementId)
{
	var request = new XMLHttpRequest();
	request.onreadystatechange = (event) => { handleQueryVersion(request, elementId); };
	request.open( "GET", path, true );
	request.send( null );
}

function handleQueryVersion(request, elementId)
{
	if (request.readyState == 4 && request.status == 200)
	{
		const responseObj = JSON.parse(request.responseText);
		document.getElementById(elementId).innerHTML = `${responseObj.version.major}.${responseObj.version.minor}.${responseObj.version.patch}`;
	}
}

function doFactoryReset()
{
	document.getElementById("content").hidden = true;
	document.getElementById("loading-display").hidden = false;

	var request = new XMLHttpRequest();
	request.onreadystatechange = (event) => { handleFactoryResetResponse(request); };
	request.open("POST", "/settings/reset", true);
	request.send(null);
}

function handleFactoryResetResponse()
{
	if (request.readyState == 4 && request.status == 200)
	{
		reloadPage();
	}
}

queryVersion("/version/firmware", "firmware-version");
queryVersion("/version/filesystem", "filesystem-version");