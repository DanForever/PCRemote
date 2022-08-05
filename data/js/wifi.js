function updateWifiStatus(request)
{
	if (request.readyState == 4 && request.status == 200)
	{
		const responseObj = JSON.parse(request.responseText);
		element = document.getElementById("wifi-strength");
		description = "Wifi strength: " + responseObj.rssi;
		element.alt = description;
		element.title = description;
		
		if(responseObj.rssi > -70)
		{
			element.src = "/images/wifi-3.png";
		}
		else if(responseObj.rssi > -80)
		{
			element.src = "/images/wifi-2.png";
		}
		else if(responseObj.rssi > -90)
		{
			element.src = "/images/wifi-1.png";
		}
		else
		{
			element.src = "/images/wifi-0.png";
		}
		
		setTimeout(pollWifiStatus, 5000);
	}
}

function pollWifiStatus()
{
	var request = new XMLHttpRequest();
	request.onreadystatechange = (event) => { updateWifiStatus(request); };
	request.open("GET", "/network/rssi", true);
	request.send();
}

pollWifiStatus();