from public import models

data = [
    models.Chart(
        sensor = "temperature",
        text = "Temperature Sensor",
        type="line", dataPoints = [
        {
            "at": "2017-10-13T13:03:40.359Z",
            "value": 22.6499996185303
        },{
            "at": "2017-10-13T13:04:00.359Z",
            "value": 30.6499996185303
        },{
            "at": "2017-10-13T13:04:24.359Z",
            "value": 35.6499996185303
        },{
            "at": "2017-10-13T13:04:50.359Z",
            "value": 18.6499996185303
        },
    ]),
    models.Chart(
        sensor = "pressure",
        text="Pressure Sensor", type="line", dataPoints=[
        {
            "at": "2017-10-13T13:03:20.359Z",
            "value": 140.6499996185303
        }, {
            "at": "2017-10-13T13:04:00.359Z",
            "value": 60.6499996185303
        }, {
            "at": "2017-10-13T13:04:40.359Z",
            "value": 75.6499996185303
        }, {
            "at": "2017-10-13T13:05:20.359Z",
            "value": 120.6499996185303
        },
    ]),
]

'''
,models.Chart(text = "Boolean Data Sensor", type="column", dataPoints = [
    {
        "at": "2017-10-13T13:03:40.359Z",
        "value": 0
    },{
        "at": "2017-10-13T13:04:00.359Z",
        "value": 1
    },{
        "at": "2017-10-13T13:04:24.359Z",
        "value": 0
    },{
        "at": "2017-10-13T13:04:50.359Z",
        "value": 1
    },
])
'''