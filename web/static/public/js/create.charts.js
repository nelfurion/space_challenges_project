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
        for(var i = 0; i < charts.length; i++) {
            var chartsContainer = $('#chartsContainer')
            .append('<div id="chart-' + i + '" style="height:400px;" class="col-md-4 col-sm-4 col-lg-4"></div>');
            console.log(charts[i]);
            console.log(charts[i].dataPoints);
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
    });
})(jQuery, Chart);