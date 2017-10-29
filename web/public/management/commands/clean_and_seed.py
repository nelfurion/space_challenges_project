from django.core.management.base import BaseCommand

from public.management.seed import initial_data
from public import models

class Command(BaseCommand):
    def handle(self, *args, **options):
        data = args[0] if args else initial_data.data

        if not args:
            self._clean_and_seed(data)
        else:
            self._clean_and_seed(data)

        self._check_if_seeded(data)

    def _clean(self, data):
        for documents in data:
            type(documents[0]).objects.all().delete()


    def _seed(self, data):
        for models in data:
            for model in models:
                model.save()

    def _clean_and_seed(self, data):
        self._clean(data)
        self._seed(data)
        print (models.Chart.objects.all())

    def _check_if_seeded(self, data):
        counts = map(
            lambda models: (
                self._get_seeded_models_count(models), len(models)
            ),
            data)

        different_counts = list(filter(lambda x: x[0] != x[1], counts))

        print("Seed successful...") if not different_counts else print("Seed failed...")

    def _get_seeded_models_count(self, objects):
        model = type(objects[0])
        return model.objects.all().count()