from django.db import models
from django.contrib.postgres.fields.jsonb import JSONField
import uuid

class Chart(models.Model):
    accountId = models.UUIDField(default=uuid.uuid4, editable=False)
    text = models.CharField(max_length=100)
    type = models.CharField(max_length=100)
    dataPoints = JSONField()


    def as_json(self):
        return dict(
            accountId=self.accountId,
            text=self.text,
            type=self.type,
            dataPoints=self.dataPoints)

# Create your models here.
