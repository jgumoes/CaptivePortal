/* eslint-disable no-unused-vars */
/* eslint-disable no-undef */
// deal with the deleteNetworksPopup here
function showDelNetPopup(show){
	document.getElementById("deleteNetworksPopup").style.display = show ? "block" : "none";
	fillErrorMessage("none", "#wifiForgetForm");
}

function showDeletePopupButton(show){
	document.querySelector(".wrong#saveNetworkWarning").style.display = show? "block" : "none";
}

function deleteNetworksPopup(data){
	showDelNetPopup(true);
	const chosenSSID = networkBeingAttempted();
	const delListDiv = document.getElementById("deleteNetworkList");
	var networkList = data["storedNetworks"].filter((n) => {
		console.log(n);
		if(n !== chosenSSID){ return n; }
	});
	networkListConstructor(networkList, delListDiv, "checkbox", "delNetList");
}

function postWifiForgetForm(){
	fillErrorMessage("none", "#wifiForgetForm");
	const formData = new FormData();
	let forgetNetworkList = [];
	// todo note: this query selector should be the basis of a generic form-filling function
	document.querySelectorAll("#wifiForgetForm .ssidButton input:checked").forEach((x) => {forgetNetworkList.push(x.value);});
	if (forgetNetworkList.length === 0){
		fillErrorMessage("noNetworksSelected", "#wifiForgetForm");
		return false;
	}
	formData.append("forget", forgetNetworkList);
	showDelNetPopup(false);
	waitingForFormResponse(true, "wifiSaveForm");
	postForm(formData, 3, route = "/wififorget", errorID="#saveNetworkWarning");
}

let DeleteNetworksPopup = class {
	// TODO: replace all popup function calls with those in this class
	constructor () {
		this.networkList; // TODO: do I actually need to store the state as a class variable if it's already stored in HTML?
	}
	set setNetworkList(list){
		// TODO: this needs to be called everywhere maybe i dunno
		this.networkList = list;
	}

	showErrorButton(show){
		// TODO: replaces showDeletePopupButton
		document.querySelector(".wrong#saveNetworkWarning").style.display = show? "block" : "none";
	}

	showPopup(show=true, data=null, showErrorButton=false) {
		// TODO: replaces showDelNetPopup, deleteNetworksPopup
		// Opens and closes the popup. If data isn't given, it's
		// presumed that fillPopup has already been called elsewhere
		if (data !== null){fillPopup(data);}
		document.getElementById("deleteNetworksPopup").style.display = show ? "block" : "none";
		fillErrorMessage("none", "#wifiForgetForm");
		showErrorButton(!show && showErrorButton); // hide button if show is true, otherwise set to showErrorButton
	}

	fillPopup(data){
		const chosenSSID = networkBeingAttempted();
		const delListDiv = document.getElementById("deleteNetworkList");
		var networkList = data["storedNetworks"].filter((n) => {
			console.log(n);
			if(n !== chosenSSID){ return n; }
		});
		networkListConstructor(networkList, delListDiv, "checkbox", "delNetList");
	}
};