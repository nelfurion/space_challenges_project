# -*- coding: utf-8 -*-
# Generated by Django 1.11.3 on 2017-10-29 12:28
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('public', '0002_auto_20171029_0506'),
    ]

    operations = [
        migrations.AddField(
            model_name='chart',
            name='sensor',
            field=models.CharField(default=None, max_length=100),
            preserve_default=False,
        ),
    ]
