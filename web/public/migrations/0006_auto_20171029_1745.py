# -*- coding: utf-8 -*-
# Generated by Django 1.11.3 on 2017-10-29 17:45
from __future__ import unicode_literals

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('public', '0005_auto_20171029_1741'),
    ]

    operations = [
        migrations.RenameField(
            model_name='chart',
            old_name='sensor',
            new_name='asset',
        ),
    ]
