/* datapoints = [
    { label: "apple",  y: 10  },
    { label: "orange", y: 15  },
    { label: "banana", y: 25  },
    { label: "mango",  y: 30  },
    { label: "grape",  y: 28  }]*/

// TODO: Make chartEngine a class

function assetIs(title, type) {
    return title.toLowerCase().indexOf(type.toLowerCase()) >= 0;
}

function getType(title) {
    //if
}

function getStripLines(asset_title) {
    console.log(asset_title);
    if (assetIs(asset_title, "humidity")) {
        return [{
                thickness: 3,
                showOnTop: true,
                label : "Minimal Normal Level",
                value:20,
            }, {
                thickness: 3,
                showOnTop: true,
                label : "Maximum Normal Level",
                value:60,
        }];
    } else if (assetIs(asset_title, "airquality")) {
        return [{
                thickness: 3,
                showOnTop: true,
                label : "High Pollution",
                value: 700,
            }, {
                thickness: 3,
                showOnTop: true,
                label : "Low Pollution",
                value: 100,
        }];
    } else if (assetIs(asset_title, "sensor")){
        return [{
                thickness: 3,
                showOnTop: true,
                label : "Horizontal Line",
                value: 20,
            }];
    } else {
        return [];
    }
}

var Chart = (function (chartEngine) {
    var getSign = function(text) {
        if (text.indexOf("temperature") >= 0) {
            return "°C";
        } else {
            return "";
        }
    };

    return {
        create: function(containerId, text, dataPoints, chartType) {
            var chart = new chartEngine.Chart(containerId, {
                //backgroundColor: "#CCCCCC",
                title:{
                    text: text
                },
                data: [{
                    type: text.indexOf("temperature") > 0 ? "spline": "splineArea",
			        color: "rgba(83, 223, 128, .6)",
                    // Change type to  "column", "doughnut", "line", "splineArea", etc.
                    //type: chartType,
                    dataPoints: dataPoints
                }],
                axisY: [{
                    gridThickness: 0,
                    /*ticks: {
                        // Include a dollar sign in the ticks
                        callback: function(value, index, values) {
                            return value + " " + getSign(text);
                        }
                    }*/
                    stripLines: getStripLines(text),
                }],
            });
            return chart;
        },
        render: function(chart) {
            chart.render();
        },
        display(containerId, text, dataPoints, chartType) {
            var chart = this.create(containerId, text, dataPoints, chartType);
            console.log(chart);
            this.render(chart);
        }
    };
})(CanvasJS);
