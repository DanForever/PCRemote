function queryWifi()
{
	var request = new XMLHttpRequest();
	request.onreadystatechange = (event) => { handleQueryWifi(request); };
	request.open( "GET", "/network/wifi", true );
	request.send( null );
}

function handleQueryWifi(request)
{
	if (request.readyState == 4 && request.status == 200)
	{
		const responseObj = JSON.parse(request.responseText);
		document.getElementById("ssid").value = responseObj.network.wifi.ssid;
		document.getElementById("wifi-enabled").checked = responseObj.network.wifi.enabled;
	}
}

function queryHostname()
{
	var request = new XMLHttpRequest();
	request.onreadystatechange = (event) => { handleQueryHostname(request); };
	request.open( "GET", "/network/hostname", true );
	request.send( null );
}

function handleQueryHostname(request)
{
	if (request.readyState == 4 && request.status == 200)
	{
		const responseObj = JSON.parse(request.responseText);
		document.getElementById("hostname").value = responseObj.network.wifi.hostname;
	}
}

function queryAccessPoint()
{
	var request = new XMLHttpRequest();
	request.onreadystatechange = (event) => { handleQueryAccessPoint(request); };
	request.open( "GET", "/network/ap", true );
	request.send( null );
}

function handleQueryAccessPoint(request)
{
	if (request.readyState == 4 && request.status == 200)
	{
		const responseObj = JSON.parse(request.responseText);
		document.getElementById("ap-ssid").value = responseObj.network.ap.ssid;
		document.getElementById("ap-enabled").checked = responseObj.network.ap.enabled;
	}
}

queryWifi();
queryHostname();
queryAccessPoint();