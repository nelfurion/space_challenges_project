# -*- coding: utf-8 -*-
# Generated by Django 1.11.3 on 2017-10-29 05:04
from __future__ import unicode_literals

import django.contrib.postgres.fields.jsonb
from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Chart',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('accountId', models.UUIDField()),
                ('text', models.CharField(max_length=100)),
                ('type', models.CharField(max_length=100)),
                ('dataPoints', django.contrib.postgres.fields.jsonb.JSONField()),
            ],
        ),
    ]
