// Power Switch
function pressPowerSwitch()
{
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.onreadystatechange = handlePressPowerSwitchResponse;
	xmlHttp.open( "GET", "/computer/press", true );
	xmlHttp.send( null );
}

function handlePressPowerSwitchResponse()
{
	
}

// Power Status
function updatePowerStatus(request)
{
	if (request.readyState == 4 && request.status == 200)
	{
		const responseObj = JSON.parse(request.responseText);
		powerButton = document.getElementById("power-button");
		
		if(responseObj.power)
		{
			powerButton.innerHTML = "Turn off";
			powerButton.title = "PC is currently on";
			powerButton.classList.remove("power-button-on");
			powerButton.classList.add("power-button-off");
		}
		else
		{
			powerButton.innerHTML = "Turn on";
			powerButton.title = "PC is currently off";
			powerButton.classList.remove("power-button-off");
			powerButton.classList.add("power-button-on");
		}
		
		setTimeout(pollPowerStatus, 5000);
	}
}

function pollPowerStatus()
{
	var request = new XMLHttpRequest();
	request.onreadystatechange = (event) => { updatePowerStatus(request); };
	request.open("GET", "/computer/status", true);
	request.send( null );
}

pollPowerStatus();