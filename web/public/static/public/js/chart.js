/* datapoints = [
    { label: "apple",  y: 10  },
    { label: "orange", y: 15  },
    { label: "banana", y: 25  },
    { label: "mango",  y: 30  },
    { label: "grape",  y: 28  }]*/

// TODO: Make chartEngine a class

var Chart = (function (chartEngine) {
    return {
        create: function(containerId, text, dataPoints, chartType) {
            var chart = new chartEngine.Chart(containerId, {
                title:{
                    text: text
                },
                data: [{
                    // Change type to  "column", "doughnut", "line", "splineArea", etc.
                    type: chartType,
                    dataPoints: dataPoints
                }],
                options: {
                    scales: {
                        xAxes: [{
                            type: 'time',
                            time: {
                                displayFormats: {
                                    unit: 'MMM YYYY'
                                }
                            }
                        }]
                    }
                }
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
