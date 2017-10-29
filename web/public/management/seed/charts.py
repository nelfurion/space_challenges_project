from public import models

def createChart(asset, text, type):
    return models.Chart(asset=asset, text=text, type=type)

def generate():
    return [createChart(asset, asset.title, "line") for asset in models.Asset.objects.all()]
