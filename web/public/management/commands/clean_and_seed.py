from django.core.management.base import BaseCommand

from public.management.seed import initial_data
from public import models

class Command(BaseCommand):
    def add_arguments(self, parser):
        parser.add_argument('model', type=str, nargs='?', default=None)

    def handle(self, *args, **options):
        model = options.get('model', None)
        if model:
            data = initial_data.by_model[model]
            print(data)
            self._clean_and_seed(data)
        else:
            data = initial_data.data
            self._clean_and_seed(data)

            related_data = args[1] if args else initial_data.related
            related_data = [f() for f in related_data]
            self._clean_and_seed(related_data)

        print('Seed completed...')

    def _clean(self, data):
        for documents in data:
            if len(documents) > 0:
                print(documents)
                print('cleaning ', documents[0].__class__)
                type(documents[0]).objects.all().delete()


    def _seed(self, data):
        for models in data:
            if len(models) > 0:
                print(models)
                print('seeding ', models[0].__class__)
                for model in models:
                    model.save()

    def _clean_and_seed(self, data):
        self._clean(data)
        self._seed(data)

    '''def _check_if_seeded(self, data, data_type):
        counts = map(
            lambda models: (
                self._get_seeded_models_count(models), len(models)
            ),
            data)

        different_counts = list(filter(lambda x: x[0] != x[1], counts))

        print("Seed of ", data_type, " successful...") if not different_counts else print("Seed failed...")
    '''
    def _get_seeded_models_count(self, objects):
        model = type(objects[0])
        return model.objects.all().count()