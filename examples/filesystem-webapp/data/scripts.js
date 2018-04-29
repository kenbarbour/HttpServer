const stateUrl = '/led';
const toggleUrl = '/led/toggle';

var stateElem = document.getElementById("led-state");
var toggleBtn = document.getElementById("toggle-btn");

function loadState() {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', stateUrl, true);
    xhr.onreadystatechange = stateLoaded;
    xhr.send();
}

function toggle() {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', toggleUrl, true);
    xhr.onreadystatechange = stateLoaded;
    xhr.send();
}

function stateLoaded(e) {
    var state = e.target.response;
    stateElem.innerHTML = state;
}

toggleBtn.addEventListener('click',toggle);

loadState();
