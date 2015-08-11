var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function getMarketsData() {
  // Construct URL
  var url = "http://marketsapi.appspot.com/api/Markets";

  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET',
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);

      var dictionary = {
        "KEY_N225": json.results[0].Price,
        "KEY_YENDOLLAR": json.results[1].Price
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("Markets data sent to Pebble successfully!");
        },
        function(e) {
          console.log("Error sending Markets data to Pebble!");
        }
      );
    }
  );
}


// Listen for when the watchface is opened
Pebble.addEventListener('ready',
  function(e) {
    console.log("PebbleKit JS ready!");

    // Get the initial weather
    getMarketsData();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    getMarketsData();
  }
);
