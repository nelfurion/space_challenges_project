(function($, Chart) {
    var chartsData = [{
            container: "columnChart",
            text: "Test column chart",
            dataPoints: [
                {label: "A", y: 5},
                {label: "B", y: 7},
                {label: "C", y: 1},
            ],
            type: "column"
        }, {
            container: "columnChart",
            text: "Test column chart",
            dataPoints: [
                {label: "A", y: 5},
                {label: "B", y: 7},
                {label: "C", y: 1},
            ],
            type: "column"
        }
    ];

    function convertDataPoints(point, horizontalLabel, j) {
        point[horizontalLabel] = new Date(point.at);
        point.y = point.value;
        return point;
    }

    $.get('/public/charts', function(data) {
        var charts = data.charts;
        console.log('charts:');
        console.log(charts);

        for(var i = 0; i < charts.length; i++) {
            if (charts[i].dataPoints != "null") {
                if (i % 3 == 1 || (i - 1) % 3 == 1) {
                    console.log(i);
                    var chartsContainer = $('#chartsContainer')
                    .append('<div id="chart-' + i + '" style="height:300px; margin-bottom: 10px;" class="col-md-3 col-md-offset-1 col-sm-offset-1 col-lg-offset-1 col-sm-3 col-lg-3"></div>');
                } else {
                    var chartsContainer = $('#chartsContainer')
                    .append('<div id="chart-' + i + '" style="height:300px;" class="col-md-3 col-sm-3 col-lg-3"></div>');
                }

                for(var j = 0; j < charts[i].dataPoints.length; j++) {
                    convertDataPoints(charts[i].dataPoints[j], 'x', j);
                }

                Chart.display(
                    "chart-" + i,
                    charts[i].text,
                    charts[i].dataPoints,
                    charts[i].type
                );
            }
        }
    });
})(jQuery, Chart);