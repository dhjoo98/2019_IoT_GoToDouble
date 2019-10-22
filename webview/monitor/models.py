from django.db import models

# Create your models here.
class Fposition(models.Model):
    angle = models.FloatField()
    distance = models.FloatField()
    time = models.DateTimeField(auto_now_add=True)

class Sposition(models.Model):
    angle = models.FloatField()
    distance = models.FloatField()
    time = models.DateTimeField(auto_now_add=True)
