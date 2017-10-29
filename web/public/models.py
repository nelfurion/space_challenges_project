from django.db import models
from django.contrib.postgres.fields.jsonb import JSONField
import uuid

class Asset(models.Model):
    name = models.CharField(max_length=100)
    title = models.CharField(max_length=100)
    dataPoints = JSONField()

    def as_json(self):
        return dict(
            name = self.name,
            dataPoints = self.dataPoints)

class Chart(models.Model):
    asset = models.OneToOneField(
        Asset,
        on_delete=models.CASCADE,
    )

    y_unit = models.CharField(max_length=10, null=True)
    text = models.CharField(max_length=100)
    type = models.CharField(max_length=100)

    def as_json(self):
        return dict(
            text = self.text,
            type = self.type,
            dataPoints = self.asset.dataPoints,
            sensor = self.asset.as_json())

# Create your models here.
