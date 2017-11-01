import requests
import json

from django.core.management.base import BaseCommand
from public import models

headers = {
    'Auth-ClientId': 'rado_aVcsdniW',
    'Auth-ClientKey': '7zSyr3dP',
    'Content-Type': 'application/json',
}

class Command(BaseCommand):
    def handle(self, *args, **options):
        device_data = self._get_device_data(headers)
        asset_data = self._extract_assets_data(device_data)

        

        if not asset_data:
            return

        owned_assets = [asset for asset in asset_data if self._asset_is_owned(asset)]
        owned_assets_count = len(owned_assets)
        self._seed_assets(asset_data)
        self._check_if_seeded(models.Asset, owned_assets)

    def _get_device_data(self, headers):
        r = requests.get('https://api.allthingstalk.io/device/VkY0nvrdSl92SaVhRPcNAq6W', headers = headers)
        print(r.headers)

        if r.status_code != 200:
            print('Request Error: status code: ', r.status_code)
            return None

        return r.json()

    def _seed_assets(self, assets_data):
        for asset in assets_data:
            query_set = models.Asset.objects.all().filter(name=asset['name'], title=asset['title'])
            if len(query_set) == 0:
                if self._asset_is_owned(asset):
                    new_asset = models.Asset(
                        name=asset['name'],
                        title=asset['title'],
                        dataPoints=asset['dataPoints'])
                    new_asset.save()
            else:
                db_asset = list(query_set)[0]
                if not db_asset.dataPoints or db_asset.dataPoints == 'null':
                    db_asset.dataPoints = asset['dataPoints']
                else:
                    db_asset.dataPoints.extend(asset['dataPoints'])

                db_asset.save()

    def _extract_assets_data(self, data):
        sensor_data = []

        sensors = data['assets']
        for sensor in sensors:
            sensor_name = sensor['name']
            sensor_data.append({
                'name': sensor_name,
                'title': sensor['title'],
                'dataPoints': [sensor['state']]
            })

        return sensor_data

    def _check_if_seeded(self, model, asset_data):
        for asset in asset_data:
            found = model.objects.all().filter(name=asset['name'], title=asset['title'])
            print(asset['name'], ' ', asset['title'])
            print(list(found))
            if len(list(found)) <= 0:
                print('Seed failed...')
                return

        print("Seed successful...")

    def _get_seeded_models_count(self, model):
        return model.objects.all().count()

    def _asset_is_owned(self, asset):
        name = asset['name']
        return 'scs2' in name.lower()