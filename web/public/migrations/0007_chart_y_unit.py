# -*- coding: utf-8 -*-
# Generated by Django 1.11.3 on 2017-10-29 18:28
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('public', '0006_auto_20171029_1745'),
    ]

    operations = [
        migrations.AddField(
            model_name='chart',
            name='y_unit',
            field=models.CharField(max_length=10, null=True),
        ),
    ]
