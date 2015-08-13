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
      var n225 = "0";
      if(json.results[0].Diff.indexOf("-") === -1){
          n225 = "1";
      }
      var yendoller = "0";
      if(json.results[1].Diff.indexOf("-") === -1){
          yendoller = "1";
      }
      var dictionary = {
        "KEY_N225_PRICE": json.results[0].Price,
        "KEY_N225_DIFF": json.results[0].Diff,
        "KEY_N225_DIFFPERCENT": json.results[0].DiffPercent,
        "KEY_N225_NP": n225,
        "KEY_YENDOLLAR_PRICE": json.results[1].Price,
        "KEY_YENDOLLAR_DIFF": json.results[1].Diff,
        "KEY_YENDOLLAR_DIFFPERCENT": json.results[1].DiffPercent,
        "KEY_YENDOLLAR_NP": yendoller
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
