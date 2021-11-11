// form POSTs will get handled here

document.getElementById('pwd').addEventListener('keypress', (e) => handleFormEnter(e, 'pwd'))

function handleFormEnter(event, elementID){
  const keyCode = event.code;
  const postForms = {'pwd': postWifiSaveForm} // which form-sending-function to call for which elementID
  if(keyCode === "Enter"){
    event.preventDefault();
    postForms[elementID]();
    return false;
  }
  return true;
}

function waitingForFormResponse(waiting, formID){
  // this should work for wifiSaveForm, saveDeviceName, and any other forms that may be added along the way
  const form = document.getElementById(formID)
  if (waiting){ fillErrorMessage("none") }
  hideOrShowForm(form, waiting)
  for (el of form.elements){ el.disabled = waiting }
}

function networkBeingAttempted(){
  // it might be a good idea to put the post stuff into a class,
  // so that this function isn't needed
  return document.getElementById('wifiSaveForm').elements.ssid.value
}

function postWifiSaveForm(){
  // this COULD be refactored into a generic postForm function, but should it?
  // A: sort of.
  const formData = new FormData();
  const wifiForm = document.getElementById('wifiSaveForm')
  const chosenSSID = networkBeingAttempted();
  if(chosenSSID === ""){
    fillErrorMessage("noNetworkSelected")
    return false;
  }
  formData.append('ssid', chosenSSID)
  formData.append('pwd', wifiForm.elements.pwd.value)
  console.log('formData', formData);
  waitingForFormResponse(true, 'wifiSaveForm')
  postForm(formData);
}

function postForm(formData, reattempt = 3, route = '/wifisave', errorID="#wifiSaveForm"){
  // reattempt signals whether this function should be called again
  // if the fetch attempt fails
  fetch(route, {method: 'POST', body: formData})
    .then(result => result.json())
    .then(data => {
      fillErrorMessage(data.error, errorID);
      const dataTrue = ["none", "storageFull"]; // a check to make sure a bad attempt isn't filled as being successfull
      fillConnectionStatus({'networkName': dataTrue.includes(data.error) && networkBeingAttempted()}); // keep this statement as is, incase some dickhead calls their network "none"
      waitingForFormResponse(false, 'wifiSaveForm');
      if(data.error === "storageFull"){ deleteNetworksPopup(data);} //TODO update with correct function call
    })
    .catch(error => {
      if(reattempt > 1){postForm(formData, reattempt - 1, route)}
      else{
        waitingForFormResponse(false, 'wifiSaveForm');
        fillErrorMessage("other", errorID);
        console.log(error);
      }
    })
}
