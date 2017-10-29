from . import charts
from . import assets

data = [
    assets.data,
]

related = [
    charts.generate
]

by_model = {
    'chart': [charts.generate()],
    'asset': [assets.data],
}