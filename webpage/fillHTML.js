/* eslint-disable no-undef */
/* eslint-disable no-unused-vars */
// html elements are filled and adjusted here
function hideOrShowForm(form, hide){
	if(hide){ form.setAttribute("class", "hide"); }
	else { form.setAttribute("class", "show"); }
}

async function getServerInfo(){
	fetch("/server_info", {method: "GET"})
		.then(result => result.json())
		.then(data => fillFlaggedElements(data));
}

function fillNameChange(flags){
	// this has been removed from server flags, but will need to be re-implemented somewhere
	const {deviceName, nameChange} = flags;
	var nameChangeSpan = document.getElementById("device_name_change");
	if (nameChange === true){
		nameChangeSpan.insertAdjacentHTML("afterbegin", `<b class='NameChange'>Device name changed to <em>${deviceName}</em></b><br>`);
	}
}

function fillDeviceName(flags){
	const {deviceName} = flags;
	var deviceNameSpan = document.getElementById("device_name");
	deviceNameSpan.innerText = deviceName;
}

function fillConnectionStatus(flags){
	const {networkName, networkSaved} = flags;
	console.log("fillConnectionStatus", networkName);
	var connectionStatusSpan = document.getElementById("connection_status");
	if (!networkName){
		connectionStatusSpan.innerText = "not connected to a network";
	}
	else{
		connectionStatusSpan.innerText = `connected to ${networkName}`;
		if(!networkSaved){showDeletePopupButton(true);} // TODO: update with correct call
	}
}

function fillErrorMessage(result, formID = "#wifiSaveForm"){
	// The function formally known as fillWrongPassword. Fills the error message
	// maybe convert this into a pop-up? https://www.w3schools.com/howto/howto_js_popup.asp
	var errorMessageSpan = document.querySelector(`${formID} #errSpan`);

	const htmlOptions = {
		"none": "",
		"wrongPass": "Wrong Password",
		"networkLost": "Lost the Network",
		"other": "Server Error",
		"noNetworkSelected": "Please Select a Network Above",
		"noNetworksSelected": "Please Select one or more Networks Above",
		"storageFull": "Network not Saved: Device Storage is Full"
	};
	errorMessageSpan.innerText = htmlOptions[result];
}

function fillFlaggedElements(flags){
	console.log(flags);
	fillDeviceName(flags);
	// fillNameChange(flags)
	fillConnectionStatus(flags);
}

getServerInfo();
