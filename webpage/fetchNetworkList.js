// the network list is fetched and filled here
async function getNetworkList(){
  fetch('/scan_networks', {method: 'GET'})
  .then(result => result.json())
  .then(data => fillNetworkList(data))
}

function fillNetworkList(data){
  console.log('ssids: ' + data);
  var networkDiv = document.getElementById('network_list')
  var ssids = data.network_list
  var disabled = false;
  if(ssids.length === 0){
    ssids = ["No Networks Found"]
    disabled = true;
    disableConnectButton();
  }

  networkListConstructor(ssids, networkDiv, 'radio', 'saveNetList');

  if(disabled){document.getElementsByName("ssid")[0].disabled = true;}
  // todo: check last attempted network (if needed for refreshing the networks)

  var attempted_network = data.attempted_network
  document.getElementById(attempted_network).checked = true
}

function setLabelCheck(element){
  if(element.type === "radio"){
    for (el of document.querySelectorAll("input[type=radio]")){
      el.parentElement.id = "not_chosen";
    }
  }
  const parent = element.parentElement;
  console.log('setLabelCheck:', parent);
  parent.id = element.checked ? "chosen" : "not_chosen";
}

function networkListConstructor(ssids, targetDiv, type, className){
  targetDiv.innerHTML = "";
  check = type == "radio" ? "" : "X";
  for (ssid of ssids){
    const cam_name = `${ssid.replaceAll(/\W/ig, "")}_${className}`;
    html = `<div class="ssidButton ${className}"">` + 
      `<input id="${cam_name}" type=${type} name='ssid' value=\"${ssid}\" required>` +
      `<label for="${cam_name}">` +
      `<div class='checkmark'><div class='checked'>${check}</div></div>` +
      `${ssid}</label>` + 
      `</div>`
    targetDiv.insertAdjacentHTML("afterbegin", html);
  }
}

function disableConnectButton(){
  var connectButton = document.getElementById('connectButton')
  connectButton.disabled = true;
  // connectButton.input = 'button'
  connectButton.style.background = '#bdbdbd';
  // connectButton.onclick = () => {window.confirm("There aren't any networks to connect to!")}
}

getNetworkList();

function show_password() {
  const checkbox = document.getElementById("checkbox")
  const checkboxTypes = { true: "text", false: "password"}
  const checkboxLabels = { true: "Hide Password?", false: "Show Password?"}
  checkbox.checked = !checkbox.checked;
  document.getElementById("pwd").type = checkboxTypes[checkbox.checked]
  document.getElementById('checkboxLabel').innerText = checkboxLabels[checkbox.checked]
}